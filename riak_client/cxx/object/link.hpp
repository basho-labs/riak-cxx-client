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

#ifndef RIAKCXX_LINK_HPP_
#define RIAKCXX_LINK_HPP_

#include <riak_client/cxx/riak_client_fwd.hpp>
#include <riak_client/cxx/object/riak_version.hpp>
#include <string>
#include <vector>

namespace riak { 

class RIAKC_API link
{
public:
    link(const std::string& bucket, const std::string& key)
       : bkey_(bucket, key) { }
    link(const std::string& bucket, const std::string& key, 
        const std::string& tag)
        : bkey_(bucket, key), tag_(tag) { }
public:
    const std::string& bucket() const { return bkey_.bucket(); }
    const std::string& key() const { return bkey_.key(); }
    const std::string& tag() const { return tag_; }
private:
    riak_bkey bkey_;
    std::string tag_;
};

typedef std::vector<link> link_vector;

} // ::riak

#endif // include guard
