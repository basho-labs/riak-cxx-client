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

#ifndef RIAKCXX_DELETE_OBJECT_HPP_
#define RIAKCXX_DELETE_OBJECT_HPP_


#include <riak_client/cxx/riak_client_fwd.hpp>
#include <riak_client/cxx/basic/basic_client.hpp>
#include <riak_client/cxx/object/riak_object.hpp>

namespace riak {

class RIAKC_API delete_object
{
public:
    delete_object(client_ptr client, const std::string& bucket,
                  const std::string& key)
        : client_(client), bucket_(bucket), key_(key) {}
public:
    delete_object& rw(int rw)
    {
        rw_ = rw;
        return *this;
    }

    response<bool> operator()() {
        response<bool> result(client_->del(bucket_, key_, rw_));
        if (result.error()) throw;
        return true;
    }
private:
    client_ptr client_;
    std::string bucket_;
    std::string key_;
    int rw_;

};

} // ::riak

#endif // include guard
