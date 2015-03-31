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

#include <riak_client/cxx/object.hpp>
#include <iostream>
#include <sstream>
#include <iterator>
#include <iomanip>

namespace riak {

riak_metadata::riak_metadata()
{
}

riak_metadata::riak_metadata(const string_map& usermeta)
  : usermeta_(usermeta)
{
}

void riak_metadata::debug_print() const
{
    std::cout << "content type: " << content_type() << std::endl;
    std::cout << "charset     : " <<  charset() << std::endl;
    std::cout << "encoding    : " << encoding() << std::endl;
    std::cout << "vtag        : " << vtag() << std::endl;
    std::cout << "usermeta:" << std::endl;
    for (string_map::const_iterator it=usermeta().begin();
         it != usermeta().end();
         ++it)
    {
        std::cout << "    "  << (*it).first << ":" << (*it).second << std::endl;
    }
}

void object::debug_print() const
{
    std::cout << "----- riak::object[bucket=" << bucket();
    std::cout << ",key=" << key() << "] at " << this << "-----" << std::endl;
    std::ostringstream result;
    std::string str_vclock(vclock());
    result << std::setw(2) << std::setfill('0') << std::hex << std::uppercase;
    std::copy(str_vclock.begin(), str_vclock.end(), std::ostream_iterator<uint32_t>(result, " "));
    std::cout << "vclock : " << result.str() << std::endl;
    content().debug_print();
    std::cout << "----------" << std::endl;
    std::cout << std::endl;

}

void riak_content::debug_print() const
{
    metadata().debug_print();
    std::cout << "value: " << value() << std::endl;

}



} // ::riak
