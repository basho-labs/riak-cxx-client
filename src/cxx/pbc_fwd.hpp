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

#ifndef RIAKCXX_PBC_FWD_HPP_
#define RIAKCXX_PBC_FWD_HPP_

#include <riak_client/cxx/util/shared_ptr.hpp>

namespace google { namespace protobuf {
    class MessageLite;
}} // :: ::google::protobuf;

namespace riak { namespace pbc {

typedef unsigned char msgcode_t;

using google::protobuf::MessageLite;

}} // riak::pbc

class RpbErrorResp;
class RpbGetClientIdResp;
class RpbSetClientIdReq;
class RpbGetServerInfoResp;
class RpbGetReq;
class RpbGetResp;
class RpbPutReq;
class RpbPutResp;
class RpbDelReq;
class RpbListBucketsResp;
class RpbListKeysReq;
class RpbListKeysResp;
class RpbGetBucketReq;
class RpbGetBucketResp;
class RpbSetBucketReq;
class RpbMapRedReq;
class RpbMapRedResp;
class RpbContent;
class RpbPair;
class RpbLink;
class RpbBucketProps;


#endif // include guard
