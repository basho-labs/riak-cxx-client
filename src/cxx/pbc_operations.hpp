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

#ifndef RIAKCXX_PBC_OPERATIONS_HPP_
#define RIAKCXX_PBC_OPERATIONS_HPP_

#include <riak_client/cxx/riak_client_fwd.hpp>
#include <riak_client/cxx/basic/response.hpp>
#include "pbc_message.hpp"
#include "src/cxx/riakclient.pb.h"

namespace riak { namespace pbc {


template <class Request, class Response>
class operation : private noncopyable
{
public:
    typedef Request request_type;
    typedef Response response_type;
    static const msgcode_t request_code = request_type::message_code;
    static const msgcode_t response_code = response_type::message_code;
public:
    Request&    request() { return request_; }
    Response&   response() { return response_; }
    riak_error  error() const { return error_; }
private:
    Request request_;
    Response response_;
    riak_error error_;
};

typedef pbc_message<ERROR, RpbErrorResp> error_resp;

typedef pbc_message<PING> ping_req;
typedef pbc_message<PING_RESP> ping_resp;

typedef pbc_message<DEL, RpbDelReq> del_req;
typedef pbc_message<DEL_RESP> del_resp;

typedef pbc_message<PUT, RpbPutReq> put_req;
typedef pbc_message<PUT_RESP, RpbPutResp> put_resp;

typedef pbc_message<GET_CLIENT_ID> get_client_id_req;
typedef pbc_message<GET_CLIENT_ID_RESP, RpbGetClientIdResp> get_client_id_resp;

typedef pbc_message<GET, RpbGetReq> get_req;
typedef pbc_message<GET_RESP, RpbGetResp> get_resp;

typedef pbc_message<SET_CLIENT_ID, RpbSetClientIdReq> set_client_id_req;
typedef pbc_message<SET_CLIENT_ID_RESP> set_client_id_resp;

typedef pbc_message<GET_SERVER_INFO> get_server_info_req;
typedef pbc_message<GET_SERVER_INFO_RESP, RpbGetServerInfoResp> get_server_info_resp;

typedef pbc_message<LIST_BUCKETS> list_buckets_req;
typedef pbc_message<LIST_BUCKETS_RESP, RpbListBucketsResp> list_buckets_resp;

typedef pbc_message<LIST_KEYS, RpbListKeysReq> list_keys_req;
typedef pbc_message<LIST_KEYS_RESP, RpbListKeysResp> list_keys_resp;

typedef pbc_message<GET_BUCKET, RpbGetBucketReq> get_bucket_req;
typedef pbc_message<GET_BUCKET_RESP, RpbGetBucketResp> get_bucket_resp;

typedef pbc_message<SET_BUCKET, RpbSetBucketReq> set_bucket_req;
typedef pbc_message<SET_BUCKET_RESP> set_bucket_resp;

namespace ops {
    typedef operation<ping_req, ping_resp> ping;
    typedef operation<get_server_info_req, get_server_info_resp> get_server_info;
    typedef operation<del_req, del_resp> del;
    typedef operation<put_req, put_resp> put;
    typedef operation<get_client_id_req, get_client_id_resp> get_client_id;
    typedef operation<get_req, get_resp> get;
    typedef operation<set_client_id_req, set_client_id_resp> set_client_id;
    typedef operation<list_buckets_req, list_buckets_resp> list_buckets;
    typedef operation<list_keys_req, list_keys_resp> list_keys;
    typedef operation<get_bucket_req, get_bucket_resp> get_bucket;
    typedef operation<set_bucket_req, set_bucket_resp> set_bucket;
} // ::riak::pbc::ops


}} // ::riak::pbc

#endif // include guard
