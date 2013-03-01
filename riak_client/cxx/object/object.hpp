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

#ifndef RIAKCXX_RIAK_BASIC_OBJECT_HPP_
#define RIAKCXX_RIAK_BASIC_OBJECT_HPP_

#include <riak_client/cxx/riak_client_fwd.hpp>
#include <riak_client/cxx/object/riak_version.hpp>
#include <boost/concept_check.hpp>
#include <boost/concept/assert.hpp>
#include <string>
#include <map>

namespace riak {


template <class ValueType>
struct basic_content
{
    typedef ValueType    value_type;
public:
    basic_content() : value_() { }
    basic_content(const value_type& value) : value_(value) { }
public:
    const value_type    value() const;
    operator const value_type() const { return value_; }
private:
    value_type value_;
};

template <class ValueType=std::string>
struct basic_object : public basic_content<ValueType>
{
    typedef ValueType value_type;
    typedef basic_content<value_type> content_type;
    typedef basic_object<value_type> type;
    basic_object() : content_type() { }
    basic_object(const value_type& value) : content_type(value) { }
};

typedef basic_object<> string_object;

} // ::riak


#endif // include guard
