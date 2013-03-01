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

#ifndef RIAKCXX_RIAK_CLIENT_FWD_HPP_
#define RIAKCXX_RIAK_CLIENT_FWD_HPP_

#include <riak_client/cxx/common.hpp>
#include <riak_client/cxx/util/shared_ptr.hpp>

namespace riak {

    class connection;
    typedef shared_ptr<connection> connection_ptr;

    class basic_client;
    typedef shared_ptr<basic_client> client_ptr;

    class object;
    typedef shared_ptr<object> object_ptr;

} // ::riak

#endif // include guard
