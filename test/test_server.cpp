#include "asio.hpp"
#include "connection.hpp"
#include "pbc_operations.hpp"
#include "pbc_header.hpp"
#include <riak_client/cxx/error.hpp>
#include <map>
using boost::asio::ip::tcp;

const int max_length = 4096;

typedef boost::shared_ptr<tcp::socket> socket_ptr;

namespace r = riak;
namespace pbc = riak::pbc;

/*
static const msgcode_t ERROR  = 0;
static const msgcode_t GET_SERVER_INFO      = 7;
static const msgcode_t GET_SERVER_INFO_RESP = 8;
static const msgcode_t GET    = 9;
static const msgcode_t GET_RESP      = 10;
static const msgcode_t PUT    = 11;
static const msgcode_t PUT_RESP      = 12;
static const msgcode_t DEL    = 13;
static const msgcode_t DEL_RESP      = 14;
static const msgcode_t LIST_BUCKETS  = 15;
static const msgcode_t LIST_BUCKETS_RESP    = 16;
static const msgcode_t LIST_KEYS     = 17;
static const msgcode_t LIST_KEYS_RESP= 18;
static const msgcode_t GET_BUCKET    = 19;
static const msgcode_t GET_BUCKET_RESP      = 20;
static const msgcode_t SET_BUCKET    = 21;
static const msgcode_t SET_BUCKET_RESP      = 22;
static const msgcode_t MAPRED = 23;
*/

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
  test_server(socket_ptr sock)
    : sock_(sock), client_id_(0) {} 

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
    if (error_ && (error_ != boost::asio::error::eof))
      throw r::exception(r::riak_error(error_.value(), error_.message()));
  }

  void start() 
  {
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
  printf("client id: %d\n", client_id_);
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

}

template <>
void test_server::handle_operation<pbc::ops::set_bucket>(pbc::ops::set_bucket& op)
{
  std::map<std::string, test_bucket>::iterator it = data_.find(op.request().bucket());
  test_bucket* bucket = 0;
  if (it != data_.end()) {    
    bucket = &it->second;
  }
  else {
    test_bucket b;
    data_[op.request().bucket()] = b;
    it = data_.find(op.request().bucket());
    bucket = &it->second;
  }
  bucket->allow_mult = op.request().props().allow_mult();
  bucket->n_val = op.request().props().n_val();

}

void server_loop(short port)
{
  boost::asio::io_service io_service;
  tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
  for (;;)
  {
    socket_ptr sock(new tcp::socket(io_service));
    a.accept(*sock);
    test_server server(sock);
    server.start();

  }
}

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: blocking_tcp_echo_server <port>\n";
      return 0;
    }
    using namespace std; // For atoi.
    server_loop(atoi(argv[1]));
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}

