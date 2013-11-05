/*  
 Copyright 2012 Serge Slipchenko <Serge.Slipchenko@gmail.com>

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

#ifndef RIAKCXX_INDEX_HPP_
#define RIAKCXX_INDEX_HPP_

#include <riak_client/cxx/riak_client_fwd.hpp>
#include <riak_client/cxx/error.hpp>
#include <riak_client/cxx/object/riak_version.hpp>
#include <string>
#include <vector>

namespace riak { 

class RIAKC_API index
{
public:
    index(const std::string& key, const std::string& value)
        : key_(key), value_(value) 
    { 
        if (key.rfind("_bin") == std::string::npos &&
            key.rfind("_int") == std::string::npos)
            throw riak::exception("Invalid index key '" + key + "'");
    }

public:
    const std::string& key() const { return key_; }
    const std::string& value() const { return value_; }
private:
    std::string key_;
    std::string value_;
};

typedef std::vector<index> index_vector;

} // ::riak

#endif // include guard
