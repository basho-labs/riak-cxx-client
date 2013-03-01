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

#ifndef RIAKCXX_SERVER_INFO_HPP_
#define RIAKCXX_SERVER_INFO_HPP_

#include <riak_client/cxx/riak_client_fwd.hpp>

namespace riak {

class RIAKC_API server_info
{
public: // constructors
    explicit server_info() { }
    server_info(const std::string& node, const std::string& version)
        : node_(node), version_(version) { }
public:
    const std::string& node() const { return node_; }
    const std::string& version() const { return version_; }
public:
    std::string node_;
    std::string version_;
};

} // ::riak

#endif // include guard


