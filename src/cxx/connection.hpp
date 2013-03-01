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

#ifndef RIAKCXX_CONNECTION_HPP_
#define RIAKCXX_CONNECTION_HPP_

#include "asio.hpp"
#include <riak_client/cxx/riak_client_fwd.hpp>
#include <riak_client/cxx/basic/response.hpp>
#include <string>


namespace riak {

class connection : private noncopyable
{
public:
    connection(const std::string& host, const std::string& port);
    ~connection();
public:
    /// Start the connection
    response<bool>     start();
    const std::string& host() const { return host_; }
    const std::string& port() const { return port_; }
public: // io
    std::size_t write(io::const_buffer buf);
    std::size_t read(io::mutable_buffer buf);
private:
    std::string host_;
    std::string port_;
    io::io_service io_service_;
    io::tcp_socket socket_;
};

} // :: riak

#endif // include guard
