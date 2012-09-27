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

#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>

#include "connection.hpp"
#include <riak_client/cxx/error.hpp>

namespace riak { 

namespace error = boost::asio::error;
using boost::system::error_code;

using boost::lambda::var;
using boost::lambda::_1;
using boost::lambda::_2;

connection::connection(const std::string& host, const std::string& port,
                       boost::posix_time::time_duration timeout)
    : host_(host),
      port_(port),
      timeout_(timeout),
      io_service_(),
      socket_(io_service_),
      deadline_timer_(io_service_)
{
    deadline_timer_.expires_at(boost::posix_time::pos_infin);
    check_deadline();
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

        ec = boost::asio::error::would_block;
        deadline_timer_.expires_from_now(timeout_);
        socket_.async_connect(*it++, var(ec) = _1);
        do io_service_.run_one(); while (ec == boost::asio::error::would_block);
        if (ec || !socket_.is_open()) {
            ec = ec ? ec : boost::asio::error::operation_aborted;
            break;
        }
    }

    if (ec)
        throw riak::exception(riak_error(ec.value(), ec.message()));

    return true;
}

std::size_t 
connection::write(io::const_buffer buf) {
    error_code ec = boost::asio::error::would_block;
    deadline_timer_.expires_from_now(timeout_);
    std::size_t n = 0;
    io::async_write(socket_, io::buffer(buf), io::transfer_all(),
                    (var(ec) = _1, var(n) += _2));
    do io_service_.run_one(); while (ec == boost::asio::error::would_block);
    if (ec)
       throw riak::exception(riak_error(ec.value(), ec.message()));
    return n;
}

std::size_t 
connection::read(io::mutable_buffer buf) {
    error_code ec = boost::asio::error::would_block;
    deadline_timer_.expires_from_now(timeout_);
    std::size_t n = 0;
    io::async_read(socket_, io::buffer(buf), io::transfer_all(),
                   (var(ec) = _1, var(n) += _2));
    do io_service_.run_one(); while (ec == boost::asio::error::would_block);
    if (ec)
        throw riak::exception(riak_error(ec.value(), ec.message()));
    return n;
}

void connection::check_deadline() {
    // Check whether the deadline has passed. We compare the deadline against
    // the current time since a new asynchronous operation may have moved the
    // deadline before this actor had a chance to run.
    if (deadline_timer_.expires_at() <= io::deadline_timer::traits_type::now())
    {
      // The deadline has passed. The socket is closed so that any outstanding
      // asynchronous operations are cancelled. This allows the blocked
      // connect(), read_line() or write_line() functions to return.
      boost::system::error_code ignored_ec;
      socket_.close(ignored_ec);

      // There is no longer an active deadline. The expiry is set to positive
      // infinity so that the actor takes no action until a new deadline is set.
      deadline_timer_.expires_at(boost::posix_time::pos_infin);
    }

    // Put the actor back to sleep.
    deadline_timer_.async_wait(boost::lambda::bind(&connection::check_deadline, this));
}

} // ::riak
