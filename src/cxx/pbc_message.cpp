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

#include "pbc_message.hpp"
#include "pbc_header.hpp"
#include <google/protobuf/message_lite.h>

namespace riak { namespace pbc {

bool pbc_serialize(msgcode_t code, pbc_storage& storage, const MessageLite* message)
{
    pbc_header header(code, sizeof(code));
    if (message)
    {
        header.size(header.size()+message->ByteSize());
        message->SerializeToArray(storage.data()+pbc_header::HEADER_SIZE,
                                  storage.size());
    }
    header.serialize(storage.data(), pbc_header::HEADER_SIZE);
    return true;
}


bool pbc_deserialize(msgcode_t code, const pbc_storage& storage, MessageLite* message)
{
    if (message)
    {
        message->ParseFromArray(storage.data(), storage.size());
    }
    return true;

}


}} // riak::pbc
