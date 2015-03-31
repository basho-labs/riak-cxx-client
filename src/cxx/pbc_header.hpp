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

#ifndef RIAKCXX_PBC_HEADER_HPP_
#define RIAKCXX_PBC_HEADER_HPP_

#include <riak_client/cxx/riak_client_fwd.hpp>
#include <stdint.h>
#include "pbc_fwd.hpp"

namespace riak { namespace pbc {

class pbc_header
{
public:
    static const std::size_t HEADER_SIZE = 5;
public: // constructors
    pbc_header() : size_(0), code_(0) {}
    pbc_header(msgcode_t c) : size_(0), code_(c) {}
    pbc_header(msgcode_t c, std::size_t s) : size_(s), code_(c) {}
public: // accessors
    const msgcode_t code() const { return code_; }
    const uint32_t size() const { return size_; }
public: // mutators
    void code(msgcode_t c) { code_ = c; }
    void size(uint32_t s) { size_ = s; }
public: // serialization
    void deserialize(const char *buf, std::size_t len);
    void serialize(char* buf, std::size_t len) const;
private: // intentionally copyable
    uint32_t  size_;
    msgcode_t code_;
};

}} // ::riak::pbc

#endif // include guard
