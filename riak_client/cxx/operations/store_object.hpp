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

 template <typename T=object_ptr, class mutator=clobber_mutator<T>, class resolver=default_resolver>
class RIAKC_API store_object
{
public:
    store_object(client_ptr c, const std::string& bucket,
                 const std::string& key)
        : client_(c), bkey_(bucket, key), dw_(2), w_(2), r_(2), 
              return_body_(false) { }
public:
    store_object<T>& dw(int dw) { dw_ = dw; return *this; }
    store_object<T>& return_body(bool val) { return_body_ = val; return *this;}
    store_object<T>& r(int r) { r_ = r; return *this; }
    store_object<T>& w(int w) { w_ = w; return *this; }
    store_object<T>& with_value(const T& value) { val_ = value; }
    store_object<T>& with_mutator();
    store_object<T>& with_resolver();
 public:
    response<T> operator()() { 
        response<fetch_result> fr = client_->fetch(bkey_.bucket(), bkey_.key(), r_, r_);
        if (fr.error()) return fr.error();
        riak::object_ptr obj = resolver().resolve(fr.value());
        T newval(mutator(val_)(obj->value()));
        obj->update_value(newval);
        store_params sp;
        sp.dw(dw_);
        sp.w(w_);
        sp.return_body(return_body_);
        response<object_ptr> result(client_->store(obj, sp));
        if (result.error()) throw;
        return obj->value();
    }
private:
    client_ptr client_;
    riak_bkey bkey_;
    int dw_;
    int w_;
    int r_;
    bool return_body_;
    T val_;
};

} // ::riak

#endif // include guard


