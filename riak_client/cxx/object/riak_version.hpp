/**
  @file    riak_version.hpp
  @brief   Riak Keys and Object Versions */
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
#ifndef RIAKCXX_RIAK_VERSION_HPP_
#define RIAKCXX_RIAK_VERSION_HPP_

#include <riak_client/cxx/riak_client_fwd.hpp>
#include <string>

namespace riak {

class RIAKC_API riak_bkey
{
public: // constructors
    riak_bkey(const std::string& bucket, const std::string& key)
        : bucket_(bucket), key_(key) { }
public: // accessors
    const std::string& bucket() const { return bucket_; }
    const std::string& key() const { return key_; }
private: // intentionally copyable
    std::string bucket_;
    std::string key_;
};

class RIAKC_API riak_version
{
 public: // constructors
    riak_version(const riak_bkey& bkey)
        : bkey_(bkey) { }
    riak_version(const riak_bkey& bkey, const std::string& vclock)
        : bkey_(bkey), vclock_(vclock) { }
public: // accessors
    const riak_bkey&   bkey() const { return bkey_; }
    const std::string& bucket() const { return bkey_.bucket(); }
    const std::string& key() const { return bkey_.key(); }
    const std::string& vclock() const { return vclock_; }
private: // intentionally copyable
    riak_bkey bkey_;
    std::string vclock_;
};

} // ::riak

#endif // include guard
