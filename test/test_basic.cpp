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

#include "test_common.hpp"
#include <riak_client/cxx/riak_client.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <algorithm>
#include <assert.h>
#include <cstdio>

using std::string;

static const std::string TEST_BUCKET("riak-cxx-test");
static const std::string TEST_KEY("riak-cxx-test");

bool test_pbc_client()
{
    TEST_INIT t(__FUNCTION__, __FILE__, __LINE__); 
    riak::client_ptr c = riak::new_client("127.0.0.1", "8087");
    assert(c->ping());
    c->client_id(42);
    assert(c->client_id() == 42);
    return true;
}

bool test_list_buckets() 
{
    TEST_INIT t(__FUNCTION__, __FILE__, __LINE__);
    riak::client_ptr c = riak::new_client("127.0.0.1", "8087");
    riak::string_vector v = c->list_buckets();
    assert(std::find(v.begin(), v.end(), TEST_BUCKET) != v.end());
    return true;
}

bool test_list_keys()
{
    TEST_INIT t(__FUNCTION__, __FILE__, __LINE__);
    riak::client_ptr c = riak::new_client("127.0.0.1", "8087");
    riak::string_vector v = c->list_keys(TEST_BUCKET);
    assert(v.size() > 0);
    return true;

}

bool test_del()
{
    TEST_INIT t(__FUNCTION__, __FILE__, __LINE__);
    riak::client_ptr c = riak::new_client("127.0.0.1", "8087");
    riak::string_vector v = c->list_keys(TEST_BUCKET);
    for (riak::string_vector::size_type i=0;
         i < v.size(); ++i)
    {
        assert(c->del(TEST_BUCKET, v[i], 3));
    }
    return true;
}


bool test_set_bucket() 
{
    TEST_INIT t(__FUNCTION__, __FILE__, __LINE__);
    riak::client_ptr c = riak::new_client("127.0.0.1", "8087");
    riak::bucket_properties properties;
    properties.allow_mult(true);
    properties.n_val(669);
    return c->set_bucket(TEST_BUCKET, properties);
}

bool test_fetch_bucket() 
{
    TEST_INIT t(__FUNCTION__, __FILE__, __LINE__);
    riak::client_ptr c = riak::new_client("127.0.0.1", "8087");
    riak::bucket_properties result = c->fetch_bucket(TEST_BUCKET);
    assert(result.allow_mult() == true);
    assert(result.n_val() == 669);
    return true;

}

bool test_fetch()
{
    TEST_INIT t(__FUNCTION__, __FILE__, __LINE__);
    riak::client_ptr c = riak::new_client("127.0.0.1", "8087");
    riak::riak_result fr = c->fetch(TEST_BUCKET, TEST_KEY, 3, 3);
    assert(fr.contents()[0].value() == TEST_KEY);
    return true;
}

bool test_put()
{
    TEST_INIT t(__FUNCTION__, __FILE__, __LINE__);
    std::cout << riak::tss_client_id() << std::endl;
    riak::client_ptr c = riak::new_client("127.0.0.1", "8087");
    c->client_id(42);
    riak::store_params sp;
    sp.w(3).dw(3).return_body(true);
    riak::riak_result fetch_result = c->fetch(TEST_BUCKET, TEST_KEY, 2, 2);
    riak::object_ptr o;
    if (fetch_result.not_found()) 
        o = riak::make_object(TEST_BUCKET, TEST_KEY, TEST_KEY);
    else 
        o = fetch_result.choose_sibling(0);
    o->debug_print();
    riak::string_map usermeta(o->update_metadata().usermeta());
    usermeta["foo"] = "bar";
    riak::riak_metadata md(usermeta);
    o->update_metadata(md);
    riak::riak_result r = c->store(o, sp);
    riak::object_ptr o2(r.choose_sibling(0));
    o2->debug_print();
    return true;
}

#include <boost/thread.hpp>

bool test_client()
{
    TEST_INIT t(__FUNCTION__, __FILE__, __LINE__);
    riak::cluster cluster;
    riak::client client(cluster.make_client());
    riak::basic_bucket<std::string> bucket = client.bucket<std::string>("bucket");
    bucket.del("foo").rw(2)();
    std::string value = bucket.fetch("foo").r(3)();
    assert(value == "");
    value = bucket.store("foo", "bar")
        .r(2)
        .w(2)
        .dw(2)();
    value = bucket.fetch("foo").r(3)();
    assert(value == "bar");
    return true;
}

int main(int argc, char *argv[]) {
       if (
//           test_client() && 
           test_pbc_client()  &&
           test_set_bucket() &&
           test_fetch_bucket() &&
           test_put() &&
           test_fetch() &&
           test_list_buckets() &&
           test_list_keys() &&
           test_del()  
           )
           return 0;
    return 1;
}
