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

#ifndef RIAKCXX_PBC_CLIENT_HPP_
#define RIAKCXX_PBC_CLIENT_HPP_

#include <riak_client/cxx/riak_client_fwd.hpp>
#include <riak_client/cxx/basic.hpp>
#include <string>
#include <stdint.h>

namespace riak { namespace pbc {

class pbc_client : public riak::basic_client
{
public:
    pbc_client(const std::string& host, const std::string& port);
    virtual ~pbc_client();
public:
    response<bool> ping();
    response<server_info> get_server_info();
    response<bool> del(const std::string& bucket, const std::string& key,
                       int dw);
    response<bool> set_bucket(const std::string& bucket,
                              const bucket_properties& properties);
    response<bool> client_id(uint32_t client_id);
    response<uint32_t>      client_id();
    response<result_ptr>  fetch(const std::string& bucket,
                                 const std::string& key, int r);
    response<result_ptr>  store(object_ptr object, const store_params& params);
    response<string_vector> list_buckets();
    response<string_vector> list_keys(const std::string& bucket);
    response<bucket_properties> fetch_bucket(const std::string& bucket);
private:
    connection_ptr connection_;
    uint32_t client_id_;
};

}} // ::riak::pbc

#endif // include guard
