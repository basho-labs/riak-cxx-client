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

#include <riak_client/cxx/riak_client.hpp>
#include <riak_client/client.h>
#include <stdint.h>
#include <cstddef>
#include <cstdlib>
#include <string.h>
#include <sys/errno.h>
#include "internal.h"

struct riak_client_t
{
    riak::client_ptr p;
};


struct riak_object_t
{
    riak::result_ptr p;
};

riak_client riak_client_new(const char* host, const char *port)
{
    riak_client_t* c = (riak_client_t*)malloc(sizeof(riak_client));
    try
    {
        c->p = riak::new_client(host, port);
    }
    catch (riak::exception& e)
    {
        free(c);
        errno = e.error_code();
        return 0;
    }
    catch (std::exception& e)
    {
        free(c);
        errno = ECONNABORTED;
        return 0;
    }
    return c;
}

void riak_client_free(riak_client c) {
    c->p.reset();
    free(c);
}

int riak_ping(riak_client c)
{
    riak::response<bool> response = c->p->ping();
    if (response.error())
    {
        errno = response.error().code();
        return -1;
    }
    if (response.value()) return 1;
    return 0;
}

int riak_set_client_id(riak_client c, uint32_t id)
{
    riak::response<bool> response = c->p->client_id(id);
    if (response.error())
    {
        errno = response.error().code();
        return -1;
    }
    if (response.value()) return 0;
    return -1;
}


uint32_t riak_get_client_id(riak_client c)
{
    riak::response<uint32_t> response = c->p->client_id();
    if (response.error())
    {
        errno = response.error().code();
        return -1;
    }
    return response.value();
}



string_list riak_list_buckets(riak_client c) {
    riak::response<riak::string_vector> response = c->p->list_buckets();
    if (response.error())
    {
        errno = response.error().code();
        return 0;
    }
    string_list l = new string_list_t();
    l->v = new riak::string_vector(response.value());
    return l;
}

string_list riak_list_keys(riak_client c, const char* bucket)
{
    string_list l = new string_list_t();
    riak::response<riak::string_vector> response = c->p->list_keys(bucket);
    if (response.error())
    {
        errno = response.error().code();
        return 0;
    }
    l->v = new riak::string_vector(response.value());
    return l;
}

riak_object riak_get(riak_client c, const char* bucket, const char *key)
{
    riak::response<riak::result_ptr> response(c->p->fetch(bucket, key, 2));
    if (response.error())
    {
        errno = response.error().code();
        return 0;
    }
    riak::result_ptr fr(response.value());
    if (fr->not_found()) return 0;
    riak_object_t* result = new riak_object_t;
    result->p = fr;
    return result;
}

void riak_object_free(riak_object o)
{
    delete o;
}

const char* riak_object_get_key(riak_object o)
{
    return o->p->key().c_str();
}

const char* riak_object_get_bucket(riak_object o)
{
    return o->p->bucket().c_str();
}

const vclock riak_object_get_vclock(riak_object o)
{
    riak::result_ptr obj(o->p);
    std::string vstr(obj->vclock());
    if (vstr.empty())
        return 0;
    size_t vclen = vstr.size();
    vclock vc = (vclock)(malloc(sizeof(size_t)+vclen));
    *(size_t *)(vc) = vclen;
    const char *vcstr = o->p->vclock().c_str();
    memcpy(vc+sizeof(size_t), vcstr, vclen);
    return vc;
}


void vclock_free(vclock v)
{
    free(v);
}
