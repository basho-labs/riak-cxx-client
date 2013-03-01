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
#ifndef RIAKCXX_MUTATION_HPP_
#define RIAKCXX_MUTATION_HPP_

#include <riak_client/cxx/riak_client_fwd.hpp>
#include <riak_client/cxx/object.hpp>

template <class T>
class clobber_mutator
{
public:
    clobber_mutator(const T& new_value)
        : new_value_(new_value) { }
    T operator()(const T& old_value) {
        return new_value_;
    }
private:
    const T& new_value_;
};


#endif // include guard
