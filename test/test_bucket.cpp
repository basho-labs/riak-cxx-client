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



bool test_bucket()
{
    riak::client client("127.0.0.1", "8087");
    std::string bucket_name("mybucket");
    riak::basic_bucket<std::string> bucket = client.bucket<std::string>(bucket_name);
    bucket.store("foo", "bar")();
}


int main(int argc, char *argv[]) {
       if (
       test_bucket()
       )
           return 0;
    return 1;
}
