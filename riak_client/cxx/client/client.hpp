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

#ifndef RIAKCXX_CLIENT_HPP_
#define RIAKCXX_CLIENT_HPP_

#include <riak_client/cxx/riak_client_fwd.hpp>
#include <riak_client/cxx/basic.hpp>
#include <riak_client/cxx/client/bucket.hpp>
#include <riak_client/cxx/object.hpp>

namespace riak {

class RIAKC_API client
{
public:
    client(const std::string& host, const std::string& port)
        : basic_client_(riak::new_client(host, port)) { }
public:
    template <class T>
    basic_bucket<T> bucket(const std::string& name)
    {
        return basic_bucket<T>(name, basic_client_);
    }

    uint32_t get_client_id()
    {
        return basic_client_->client_id();
    }

    void set_client_id(uint32_t client_id)
    {
        basic_client_->client_id(client_id);
    }

    string_vector list_buckets()
    {
        return basic_client_->list_buckets();
    }
private:
    client_ptr basic_client_;
};

} // ::riak

#endif // include guard
