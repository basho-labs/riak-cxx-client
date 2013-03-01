/**
 @file    bucket_properties.hpp
 @brief   Bucket Properties */
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

#ifndef RIAKCXX_BUCKET_PROPERTIES_HPP_
#define RIAKCXX_BUCKET_PROPERTIES_HPP_

#include <riak_client/cxx/riak_client_fwd.hpp>

namespace riak {

/// Customizable properties of a Riak Bucket.
///
/// @see http://wiki.basho.com/Basic-Riak-API-Operations.html
class RIAKC_API bucket_properties
{
public:
    bucket_properties()
        : allow_mult_(false), n_val_(-1) { }

    /// Return the value of the <dfn>allow_mult</dfn> bucket property.
    bool allow_mult() const
    {
        return allow_mult_;
    }

    /// Return the <dfn>n-val</dfn> bucket property.
    ///
    /// The n-value is the replication factor of a bucket.
    int n_val() const
    {
        return n_val_;
    }

    /// Set the <dfn>allow_mult</dfn> bucket property.
    void allow_mult(bool allow)
    {
        allow_mult_ = allow;
    }

    /// Set the <dfn>n_val</dfn> bucket property.
    void n_val(int n)
    {
        n_val_ = n;
    }
private:
    bool allow_mult_;
    int n_val_;
};

} // :: riak

#endif // include guard
