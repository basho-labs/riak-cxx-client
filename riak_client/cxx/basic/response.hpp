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

#ifndef RIAKCXX_RESPONSE_HPP_
#define RIAKCXX_RESPONSE_HPP_

#include <riak_client/cxx/riak_client_fwd.hpp>
#include <riak_client/cxx/error.hpp>

namespace riak {

template <class T>
class RIAKC_API response
{
 public: // constructors
    response(const riak_error& error)
        : error_(error), value_()  { }
    response(T t)
        : error_(), value_(t) { }

 public: // accessors
    riak_error error() const { return error_; }

    T value() {
        if (error_) {
            throw exception(error_);
        }
        return value_;
    }
    operator T() { return value(); }
    operator const T() const { return value(); }
private: // intentionally copyable
    riak_error error_;
    T value_;
};

} // ::riak

#endif // include guard
