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

#ifndef RIAKCXX_FETCH_OBJECT_HPP_
#define RIAKCXX_FETCH_OBJECT_HPP_

#include <riak_client/cxx/riak_client_fwd.hpp>
#include <riak_client/cxx/object/riak_object.hpp>
#include <riak_client/cxx/basic/basic_client.hpp>
#include <riak_client/cxx/client/resolver.hpp>
#include <string>

namespace riak {

template <typename T=std::string, class resolver=default_resolver>
struct RIAKC_API fetch_object
{
    fetch_object(client_ptr client, const std::string& bucket,
                 const std::string& key)
        : client_(client), bucket_(bucket), key_(key), r_(-1) {}
public:
    fetch_object<T> r(int r)
    {
        r_ = r;
        return *this;
    }

    response<T> operator()() {
        response<result_ptr> result(client_->fetch(bucket_, key_, r_));
        if (result.error()) return result.error();
        riak::object_ptr o(resolver().resolve(result));
        return riak_object_cast<T>(o);
    }

private:
    client_ptr client_;
    std::string bucket_;
    std::string key_;
    int r_;
};

} // ::riak

#endif // include guard
