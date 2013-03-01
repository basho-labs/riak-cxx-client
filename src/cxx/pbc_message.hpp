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

#ifndef RIAKCXX_PBC_MESSAGE_HPP_
#define RIAKCXX_PBC_MESSAGE_HPP_

#include <riak_client/cxx/riak_client_fwd.hpp>
#include "pbc_fwd.hpp"
#include "pbc_storage.hpp"

namespace riak { namespace pbc {

typedef unsigned char msgcode_t;

namespace {

static const msgcode_t ERROR  = 0;
static const msgcode_t PING   = 1;
static const msgcode_t PING_RESP     = 2;
static const msgcode_t GET_CLIENT_ID = 3;
static const msgcode_t GET_CLIENT_ID_RESP   = 4;
static const msgcode_t SET_CLIENT_ID = 5;
static const msgcode_t SET_CLIENT_ID_RESP   = 6;
static const msgcode_t GET_SERVER_INFO      = 7;
static const msgcode_t GET_SERVER_INFO_RESP = 8;
static const msgcode_t GET    = 9;
static const msgcode_t GET_RESP      = 10;
static const msgcode_t PUT    = 11;
static const msgcode_t PUT_RESP      = 12;
static const msgcode_t DEL    = 13;
static const msgcode_t DEL_RESP      = 14;
static const msgcode_t LIST_BUCKETS  = 15;
static const msgcode_t LIST_BUCKETS_RESP    = 16;
static const msgcode_t LIST_KEYS     = 17;
static const msgcode_t LIST_KEYS_RESP= 18;
static const msgcode_t GET_BUCKET    = 19;
static const msgcode_t GET_BUCKET_RESP      = 20;
static const msgcode_t SET_BUCKET    = 21;
static const msgcode_t SET_BUCKET_RESP      = 22;
static const msgcode_t MAPRED = 23;

}

bool pbc_serialize(msgcode_t code, pbc_storage& storage,
                   const MessageLite* message=0);
bool pbc_deserialize(msgcode_t code, const pbc_storage& storage,
                     MessageLite* message=0);

struct null_pbc_type  {};

template <msgcode_t code, class P=null_pbc_type>
struct pbc_message : public P
{
    static const int message_code = code;

    std::size_t size() const { return P::ByteSize(); }

    bool serialize(pbc_storage& storage) const {
        return pbc_serialize(code, storage, this);
    }

    bool deserialize(const pbc_storage& storage) {
        return pbc_deserialize(code, storage, this);
    }
};

template <msgcode_t code>
struct pbc_message<code, null_pbc_type>
{
    static const int message_code = code;

    std::size_t size() const { return 0; }

    bool serialize(pbc_storage& storage) const {
        return pbc_serialize(code, storage);
    }

    bool deserialize(const pbc_storage& storage) {
        return pbc_deserialize(code, storage);
    }
};


}} // ::riak::pbc

#endif // include guard
