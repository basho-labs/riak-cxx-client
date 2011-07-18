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

#ifndef RIAKCXX_ASIO_HPP_
#define RIAKCXX_ASIO_HPP_

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>


namespace riak { 
    namespace io { 
        using riakboost::asio::buffer;
        using riakboost::asio::read;
        using riakboost::asio::write;
        using riakboost::asio::transfer_all;
        using riakboost::asio::io_service;
        using riakboost::asio::mutable_buffer;
        using riakboost::asio::const_buffer;
        namespace ip = riakboost::asio::ip;
        typedef ip::tcp::socket tcp_socket;
    } // ::io
} // ::riak

#endif // include guard
