/**
 @file    bucket.hpp
 @brief   Riak Buckets and Bucket Properties */
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

#ifndef RIAKCXX_BUCKET_HPP_
#define RIAKCXX_BUCKET_HPP_

#include <riak_client/cxx/riak_client_fwd.hpp>
#include <riak_client/cxx/basic.hpp>
#include <riak_client/cxx/object.hpp>
#include <riak_client/cxx/operations/fetch_object.hpp>
#include <riak_client/cxx/operations/store_object.hpp>
#include <riak_client/cxx/operations/delete_object.hpp>
#include <cstddef>
#include <string>

namespace riak {

template <class T=std::string>
class RIAKC_API basic_bucket 
{
public:
    basic_bucket(const std::string& name, client_ptr c)
    : name_(name), client_(c) { }
public:
    const std::string& name() const { return name_; }
    string_vector      keys() { return client_->list_keys(name_); }
    fetch_object<T>    fetch(const std::string& key) 
    {
        return fetch_object<T>(client_, name_, key);
    }
    store_object<T>    store(const std::string& key, const std::string& value)
    {
        return store_object<T>(client_, name_, key).with_value(value);
    }
    store_object<T>    store(const std::string& key, const char* buf, 
                             std::size_t len);

    delete_object      del(const std::string& key) 
    {
        return delete_object(client_, name_, key);
    }

private:
    std::string name_;
    client_ptr client_;
};

typedef basic_bucket<std::string> string_bucket;

} // ::riak

#endif // include guard
