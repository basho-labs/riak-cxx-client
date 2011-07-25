/*  
 Copyright 2011 Basho Technologies, Inc.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include "connection.hpp"
#include <riak_client/cxx/error.hpp>

namespace riak { 

namespace error = riakboost::asio::error;
using boost::system::error_code;

connection::connection(const std::string& host, const std::string& port)
    : host_(host),
      port_(port),
      io_service_(),
      socket_(io_service_)
{
}

connection::~connection() 
{
    socket_.close();
    io_service_.stop();
}

response<bool>
connection::start()
{
    io::ip::tcp::resolver r(io_service_);
    io::ip::tcp::resolver::query q(host_, port_);
    io::ip::tcp::resolver::iterator end;
    error_code ec = error::host_not_found;
    io::ip::tcp::resolver::iterator it = r.resolve(q);   
    while (ec && it != end) { 
        socket_.close();
        socket_.connect(*it++, ec);
    }
    if (ec)
        return riak_error(ec.value(), ec.message());
    return true;
}

std::size_t 
connection::write(io::const_buffer buf) {
    error_code ec;
    std::size_t n = io::write(socket_, io::buffer(buf),
                      io::transfer_all(), ec);
    if (ec)
       throw riak::exception(riak_error(ec.value(), ec.message()));
    return n;
}

std::size_t 
connection::read(io::mutable_buffer buf) {
    error_code ec;
    std::size_t n = io::read(socket_, io::buffer(buf), 
                     io::transfer_all(), ec);
    if (ec)
        throw riak::exception(riak_error(ec.value(), ec.message()));
    return n;
}

} // ::riak
