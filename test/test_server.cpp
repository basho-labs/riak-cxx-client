#include "asio.hpp"
#include "connection.hpp"
#include "pbc_operations.hpp"
#include "pbc_header.hpp"
#include <riak_client/cxx/error.hpp>
#include <map>

using boost::asio::ip::tcp;
namespace r = riak;
namespace pbc = riak::pbc;

typedef boost::shared_ptr<tcp::socket> socket_ptr;

struct test_object
{
  RpbPutReq obj;
};

struct test_bucket 
{
  std::string name;
  bool allow_mult;
  int n_val;
  std::map<std::string, test_object> objects;
};

class test_server 
{
public:
  test_server()
    : sock_(), client_id_(0) {} 

  template <class Operation>
  void receive_request(Operation& op)
  {
    error_.clear();
    r::pbc_storage storage(header_.size());
    std::size_t n = boost::asio::read(*sock_, boost::asio::buffer(storage.data(), storage.size()),
                          boost::asio::transfer_all(), error_);
    op.request().deserialize(storage);
  }

  template <class Operation>
  void handle_operation(Operation& op);

  template <class Operation>
  void send_response(Operation& op)
  {
    error_.clear();
    r::pbc_storage storage(op.response().size()+pbc::pbc_header::HEADER_SIZE);
    op.response().serialize(storage);
    std::size_t n = boost::asio::write(*sock_, boost::asio::buffer(storage.data(), storage.size()),
                    boost::asio::transfer_all(), error_);    
  }

  template <class Operation>
  r::riak_error handle_request()
  {
    Operation op;
    receive_request(op);
    maybe_throw();
    handle_operation(op);
    send_response(op);
    maybe_throw();
    return r::riak_error();    
  }

  void recv_header()
  {
    error_.clear();
    size_t length = boost::asio::read(*sock_, boost::asio::buffer(header_buf_), 
                          boost::asio::transfer_all(), error_);

    if (error_) return;
    header_.deserialize(&header_buf_[0], pbc::pbc_header::HEADER_SIZE);
  }

  void maybe_throw()
  {
    if (error_)
      throw r::exception(r::riak_error(error_.value(), error_.message()));
  }

  void start(socket_ptr sock) 
  {
    sock_ = sock;
    while (true)
    {
      recv_header();
      maybe_throw();
      switch (header_.code()) 
      {
      case pbc::PING:
        handle_request<pbc::ops::ping>();
        break;
      case pbc::SET_CLIENT_ID:
        handle_request<pbc::ops::set_client_id>();
        break;
      case pbc::GET_CLIENT_ID:
        handle_request<pbc::ops::get_client_id>();
        break;
      case pbc::GET_BUCKET:
        handle_request<pbc::ops::get_bucket>();
        break;
      case pbc::SET_BUCKET:
        handle_request<pbc::ops::set_bucket>();  
        break;  
      case pbc::LIST_BUCKETS:
        handle_request<pbc::ops::list_buckets>();
        break;  
      case pbc::LIST_KEYS:
        handle_request<pbc::ops::list_keys>();              
        break;
      case pbc::PUT:
        handle_request<pbc::ops::put>();
        break;
      case pbc::GET:
        handle_request<pbc::ops::get>();
        break;
      case pbc::DEL:
        handle_request<pbc::ops::del>();
        break;
      default:
        break;
      }
    }
  }
private:
  socket_ptr sock_;
  pbc::pbc_header header_;
  boost::system::error_code error_;
  char header_buf_[pbc::pbc_header::HEADER_SIZE];
  uint32_t client_id_;
  std::map<std::string, test_bucket> data_;
};

template <class Operation>
void test_server::handle_operation(Operation& op)
{
}

template <>
void test_server::handle_operation<pbc::ops::set_client_id>(pbc::ops::set_client_id& op)
{
  const char* client_id_str = op.request().client_id().c_str();
  uint32_t *id = (uint32_t *)client_id_str;
  client_id_ = *id;
}

template <>
void test_server::handle_operation<pbc::ops::get_client_id>(pbc::ops::get_client_id& op)
{
  op.response().set_client_id(&client_id_, sizeof(client_id_));
}

template <>
void test_server::handle_operation<pbc::ops::get_bucket>(pbc::ops::get_bucket& op)
{
  std::map<std::string, test_bucket>::iterator it = data_.find(op.request().bucket());
  if (it != data_.end()) {
    op.response().mutable_props()->set_n_val((*it).second.n_val);
    op.response().mutable_props()->set_allow_mult((*it).second.allow_mult);
  }
  else {
    op.response().mutable_props()->set_n_val(3);
    op.response().mutable_props()->set_allow_mult(false);
  }
}

template <>
void test_server::handle_operation<pbc::ops::set_bucket>(pbc::ops::set_bucket& op)
{
  std::map<std::string, test_bucket>::iterator it = data_.find(op.request().bucket());
  std::string bucket_name(op.request().bucket());
  data_[bucket_name].n_val = op.request().props().n_val();
  data_[bucket_name].allow_mult = op.request().props().allow_mult();
}

template <>
void test_server::handle_operation<pbc::ops::list_buckets>(pbc::ops::list_buckets& op)
{
  for (std::map<std::string, test_bucket>::iterator it=data_.begin();
       it != data_.end();
       ++it) 
    op.response().add_buckets(it->first);
}

template <>
void test_server::handle_operation<pbc::ops::list_keys>(pbc::ops::list_keys& op)
{
  std::map<std::string, test_bucket>::iterator it = data_.find(op.request().bucket());
  if (it != data_.end())  
  {
    for (std::map<std::string, test_object>::iterator it2=it->second.objects.begin();
         it2 != it->second.objects.end();
         ++it2)        
      {
        std::string key(it2->first);
        op.response().add_keys(key);
      }
  }
  op.response().set_done(true);
}

template <>
void test_server::handle_operation<pbc::ops::put>(pbc::ops::put& op)
{
  test_object o;
  o.obj.CheckTypeAndMergeFrom(op.request());
  data_[op.request().bucket()].objects[op.request().key()] = o;
  op.response().set_vclock("foo");
  if (op.request().return_body())
  {
    RpbContent* c = op.response().add_content();
    c->CheckTypeAndMergeFrom(op.request().content());
  }
}

template<>
void test_server::handle_operation<pbc::ops::get>(pbc::ops::get& op)
{
  std::map<std::string, test_bucket>::iterator it = data_.find(op.request().bucket());
  if (it != data_.end())
  {
    std::map<std::string, test_object>::iterator it2 = it->second.objects.find(op.request().key());
    if (it2 != it->second.objects.end())
    {
      RpbPutReq o = it2->second.obj;
      op.response().set_vclock(o.vclock());
      RpbContent* c = op.response().add_content();
      c->CheckTypeAndMergeFrom(o.content());
    }
  }
}

template<>
void test_server::handle_operation<pbc::ops::del>(pbc::ops::del& op)
{
  std::map<std::string, test_bucket>::iterator it = data_.find(op.request().bucket());
  if (it != data_.end())
  {
    std::map<std::string, test_object>::iterator it2 = it->second.objects.find(op.request().key());
    if (it2 != it->second.objects.end())
    {
      it->second.objects.erase(it2);
    }
  }
}

void server_loop(short port)
{
  boost::asio::io_service io_service;
  tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
  test_server server;
  for (;;)
  {
    socket_ptr sock(new tcp::socket(io_service));
    a.accept(*sock);
    try 
    { 
      server.start(sock);
    }
    catch (...) { }
  }
}

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: riak_cxx_test_server <port>\n";
      return 0;
    }
    while (true)
    server_loop(std::atoi(argv[1]));
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }
  return 0;
}

