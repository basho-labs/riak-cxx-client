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

void print_content(riak::riak_content c) 
{
    std::cout << "content type: " <<c.metadata().content_type() << std::endl;
    std::cout << "charset: " << c.metadata().charset() << std::endl;
    std::cout << "encoding: " << c.metadata().encoding() << std::endl;
    std::cout << "vtag: " << c.metadata().vtag() << std::endl;
    std::cout << "value: " << c.value() << std::endl;
}

void print_object(riak::object_ptr o) 
{
    std::cout << "bucket: " << o->bucket() << std::endl;
    std::cout << "key: " << o->key() << std::endl;
    print_content(o->content());
}

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
    properties.n_val(3);
    return c->set_bucket(TEST_BUCKET, properties);
}

bool test_fetch_bucket() 
{
    TEST_INIT t(__FUNCTION__, __FILE__, __LINE__);
    riak::client_ptr c = riak::new_client("127.0.0.1", "8087");
    riak::bucket_properties result = c->fetch_bucket(TEST_BUCKET);
    assert(result.allow_mult() == true);
    assert(result.n_val() == 3);
    return true;

}

bool test_fetch()
{
    TEST_INIT t(__FUNCTION__, __FILE__, __LINE__);
    riak::client_ptr c = riak::new_client("127.0.0.1", "8087");
    for (int i=0;i<100;++i) {
        std::string strkey = boost::lexical_cast<std::string>(i);
        riak::fetch_result fr = c->fetch(TEST_BUCKET, strkey, 3, 3);
        assert(fr.key() == strkey);
        assert(fr.contents().size() == 1);
        assert(fr.contents()[0].value() == strkey);
        riak::object_ptr o = fr.choose_sibling(0);
    }
    return true;
}

bool test_put()
{
    TEST_INIT t(__FUNCTION__, __FILE__, __LINE__);
    riak::client_ptr c = riak::new_client("127.0.0.1", "8087");
    riak::store_params sp;
    sp.w(3);
    sp.dw(3);
    for (int i=0;i<100;++i) {
        std::string strkey = boost::lexical_cast<std::string>(i);        
        riak::object_ptr o(riak::make_object(TEST_BUCKET, strkey, strkey));
        riak::object_ptr r = c->store(o, sp);
    }
    return true;
}

bool test_client()
{
    TEST_INIT t(__FUNCTION__, __FILE__, __LINE__);
    riak::cluster cluster;
    riak::client c(cluster.make_client());
    riak::basic_bucket<> bucket = c.bucket<riak::object_ptr>("fuckyou");
    riak::object_ptr o = bucket.fetch("foo").r(3)();
    return true;
}

int main(int argc, char *argv[]) {
       if (
           test_client() && 
           test_pbc_client() &&
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
