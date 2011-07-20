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

#include <riak_client/cxx/basic/basic_client.hpp>
#include "pbc_client.hpp"
#include <boost/thread/thread.hpp>
#include <functional>
#include <tr1/functional>
#include <iostream>
#include <sstream>
#include <string>

namespace riak {

uint32_t tss_client_id() 
{
    std::stringstream ostr;
    ostr << boost::this_thread::get_id();
    std::tr1::hash<std::string> h;
    return h(ostr.str());
}


client_ptr new_client(const std::string& host, const std::string& port, const protocol p)
{
    return client_ptr(new pbc::pbc_client(host, port));
}

} // ::riak
