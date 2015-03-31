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

#ifndef RIAKCXX_STORE_OBJECT_HPP_
#define RIAKCXX_STORE_OBJECT_HPP_

#include <riak_client/cxx/riak_client_fwd.hpp>
#include <riak_client/cxx/basic/basic_client.hpp>
#include <riak_client/cxx/client/resolver.hpp>
#include <riak_client/cxx/client/mutation.hpp>
#include <riak_client/cxx/object.hpp>

namespace riak {

template <typename T=std::string, class mutator=clobber_mutator<T>, class resolver=default_resolver>
class RIAKC_API store_object
{
public:
    store_object(client_ptr c, const std::string& bucket,
                 const std::string& key)
        : client_(c), bkey_(bucket, key), r_(2) {}
public:
    store_object<T>& dw(int dw) { sp_.dw(dw); return *this; }
    store_object<T>& w(int w) { sp_.w(w); return *this; }
    store_object<T>& r(int r) { r_ = r; return *this; }
    store_object<T>& return_body(bool val) { sp_.return_body(val); return *this; }
    store_object<T>& with_value(const T& value) { val_ = value; return *this;  }
 public:
    response<T> operator()() {
        response<result_ptr> fr = client_->fetch(bkey_.bucket(), bkey_.key(), r_);
        if (fr.error()) return fr.error();
        riak::object_ptr obj = resolver().resolve(fr.value());
        if (obj)
        {
            obj->update_value(mutator(val_)(obj->value()));
        }
        else
        {
            obj.reset(new object(bkey_));
            obj->update_value(val_);
        }
        response<result_ptr> result(client_->store(obj, sp_));
        if (result.error()) return result.error();
        return obj->value();
    }
private:
    client_ptr client_;
    riak_bkey bkey_;
    store_params sp_;
    int r_;
    T val_;
};

} // ::riak

#endif // include guard


