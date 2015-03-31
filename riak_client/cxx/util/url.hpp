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

#ifndef RIAKCXX_URL_HPP_
#define RIAKCXX_URL_HPP_

#include <riak_client/cxx/riak_client_fwd.hpp>
#include <string>

namespace riak {

class RIAKC_API url {
public:
    url(const std::string& url_str);
public:
    const std::string& protocol() const { return protocol_; }
    const std::string& host() const { return host_; }
    const std::string& port() const { return port_; }
    const std::string& path() const { return path_; }
    const std::string& query() const { return query_; }
private:
    void parse(const std::string& url_str);
private:
    std::string protocol_;
    std::string host_;
    std::string port_;
    std::string path_;
    std::string query_;
};

} // :: riak

#endif // include guard
