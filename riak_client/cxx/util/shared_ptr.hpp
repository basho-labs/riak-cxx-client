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

#ifndef RIAKCXX_SHARED_PTR_HPP_
#define RIAKCXX_SHARED_PTR_HPP_
#include "config.h"

#if defined(HAVE_BOOST_SHARED_PTR) && defined(RIAKC_USE_BOOST_SHARED_PTR)
#include <boost/shared_ptr.hpp>
namespace riak { 
    using boost::shared_ptr;
}
#elif defined(HAVE_TR1_SHARED_PTR)
#include <tr1/memory>
namespace riak { 
    using std::tr1::shared_ptr;
}
#else
#error "No shared_ptr class available"
#endif

#endif // include guard
