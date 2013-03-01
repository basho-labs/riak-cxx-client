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

#include <riak_client/string_list.h>
#include "internal.h"

string_list string_list_new()
{
    string_list s = new string_list_t();
    s->v = new riak::string_vector();
    return s;
}


void string_list_free(string_list l)
{
    delete l->v;
    free(l);
}

size_t string_list_size(string_list l)
{
    if (l->v != 0)
        return l->v->size();
    return 0;
}

const char* string_list_get(string_list l, size_t idx)
{
    if (string_list_size(l))
        return (*l->v)[idx].c_str();
    return 0;
}


