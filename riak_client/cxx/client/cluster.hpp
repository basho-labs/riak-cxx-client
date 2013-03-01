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

#ifndef RIAKCXX_CLUSTER_HPP_
#define RIAKCXX_CLUSTER_HPP_

#include <riak_client/cxx/client.hpp>
#include <string>
#include <vector>

namespace riak {
class RIAKC_API node
{
public: // constructors
    node()
        : host_("127.0.0.1"), port_("8087") { }
    node(const std::string& host, const std::string& port)
        : host_(host), port_(port) { }
public: // accessors
    const std::string& host() const { return host_; }
    const std::string& port() const { return port_; }
private: // intentionally copyable
    std::string host_;
    std::string port_;
};

class RIAKC_API cluster
{
public: // constructors
    explicit cluster() { }
    cluster(const node& seed_node)
        : seed_node_(seed_node) { }
public:
    client make_client()
    {
        return make_client(seed_node_.host(), seed_node_.port());
    }
    static client make_client(const std::string& host, const std::string& port)
    {
        return client(host, port);
    }

private:
    node seed_node_;
    std::vector<node> nodes_;
};

} // ::riak

#endif // include guard
