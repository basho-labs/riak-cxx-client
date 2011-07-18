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

#ifndef TEST_COMMON_HPP_
#define TEST_COMMON_HPP_

#include <riak_client/cxx/riak_client.hpp>
#include <stdio.h>

struct TEST_INIT 
{
    TEST_INIT(const char *test_name, const char *file, int line) 
    {
        printf("[%s:%s:%d]\n", test_name, file, line);
    }
};




#endif





