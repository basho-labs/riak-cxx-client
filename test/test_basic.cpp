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

#define BOOST_TEST_MODULE pbc_test
#include <boost/test/unit_test.hpp>
#include <riak_client/cxx/riak_client.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <algorithm>
#include <cstdio>

using std::string;

static const std::string TEST_BUCKET("riak-cxx-test");
static const std::string TEST_KEY("riak-cxx-test");

BOOST_AUTO_TEST_CASE (test_pbc_client)
{
    riak::client_ptr c = riak::new_client("127.0.0.1", "8087");
    bool ping = c->ping();
    BOOST_REQUIRE(ping);
    c->client_id(42);
    BOOST_REQUIRE(c->client_id() == 42);
}

BOOST_AUTO_TEST_CASE (test_set_bucket)
{
    riak::client_ptr c = riak::new_client("127.0.0.1", "8087");
    riak::bucket_properties properties;
    properties.allow_mult(true);
    properties.n_val(3);
    BOOST_REQUIRE(c->set_bucket(TEST_BUCKET, properties) == true);
}

BOOST_AUTO_TEST_CASE (test_fetch_bucket)
{
    riak::client_ptr c = riak::new_client("127.0.0.1", "8087");
    riak::bucket_properties result = c->fetch_bucket(TEST_BUCKET);
    BOOST_REQUIRE(result.allow_mult());
    BOOST_REQUIRE(result.n_val() == 3);
}

BOOST_AUTO_TEST_CASE (test_put)
{
    std::cout << riak::tss_client_id() << std::endl;
    riak::client_ptr c = riak::new_client("127.0.0.1", "8087");
    c->client_id(42);
    riak::store_params sp;
    sp.w(3).dw(3).return_body(true);
    riak::result_ptr fetch_result = c->fetch(TEST_BUCKET, TEST_KEY, 2);
    riak::object_ptr o;
    if (fetch_result->not_found()) 
    {        
        o = riak::make_object(TEST_BUCKET, TEST_KEY, TEST_KEY);
        riak::link_vector v = o->update_content().links();
        v.push_back(riak::link("foo", "bar", "baz"));
        o->update_content().links(v);
    }
    else 
        o = fetch_result->choose_sibling(0);
    riak::string_map usermeta(o->update_metadata().usermeta());
    usermeta["foo"] = "bar";
    riak::riak_metadata md(usermeta);
    o->update_metadata(md);
    riak::result_ptr r(c->store(o, sp));
    riak::object_ptr o2(r->choose_sibling(0));
}

BOOST_AUTO_TEST_CASE (test_fetch)
{
    riak::client_ptr c(riak::new_client("127.0.0.1", "8087"));
    riak::result_ptr fr(c->fetch(TEST_BUCKET, TEST_KEY, 3));
    BOOST_REQUIRE(fr->contents()[0].value() == TEST_KEY);
}

BOOST_AUTO_TEST_CASE (test_list_buckets)
{
    riak::client_ptr c = riak::new_client("127.0.0.1", "8087");
    riak::string_vector v = c->list_buckets();
    BOOST_REQUIRE(std::find(v.begin(), v.end(), TEST_BUCKET) != v.end());
}

BOOST_AUTO_TEST_CASE (test_list_keys)
{
    riak::client_ptr c = riak::new_client("127.0.0.1", "8087");
    riak::string_vector v = c->list_keys(TEST_BUCKET);
    BOOST_REQUIRE(v.size() > 0);
}

BOOST_AUTO_TEST_CASE (test_del)
{
    riak::client_ptr c = riak::new_client("127.0.0.1", "8087");
    riak::string_vector v = c->list_keys(TEST_BUCKET);
    for (riak::string_vector::size_type i=0;
         i < v.size(); ++i)
    {
        BOOST_REQUIRE(c->del(TEST_BUCKET, v[i], 3) == true);
    }
}

BOOST_AUTO_TEST_CASE (test_client)
{
    riak::cluster cluster;
    riak::client client(cluster.make_client());
    riak::basic_bucket<std::string> bucket = client.bucket<std::string>("bucket");
    bucket.del("foo").rw(2)();
    std::string value = bucket.fetch("foo").r(3)();
    BOOST_REQUIRE(value == "");
    value = bucket.store("foo", "bar")
        .r(2)
        .w(2)
        .dw(2)();
    value = bucket.fetch("foo").r(3)();
    BOOST_REQUIRE(value == "bar");
}

