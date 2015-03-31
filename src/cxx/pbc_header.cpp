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
#include "pbc_header.hpp"
#include <cassert>
#include <netinet/in.h>

namespace riak { namespace pbc {

void
pbc_header::serialize(char* buf, std::size_t len) const
{
    assert(len >= 5);
    uint32_t size_nl = htonl(size());
    msgcode_t c = code();
    *(uint32_t*)buf = size_nl;
    *(msgcode_t*)(buf+sizeof(uint32_t)) = c;
}

void
pbc_header::deserialize(const char *buf, std::size_t len)
{
    assert(len >= 5);
    uint32_t *s = (uint32_t *)buf;
    msgcode_t *c = (msgcode_t *)buf+4;
    size(ntohl(*s)-1);
    code(*c);
}

}} //::riak::pbc
