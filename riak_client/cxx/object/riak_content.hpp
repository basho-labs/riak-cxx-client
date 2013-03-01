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

#ifndef RIAKCXX_RIAK_CONTENT_HPP_
#define RIAKCXX_RIAK_CONTENT_HPP_

#include <riak_client/cxx/riak_client_fwd.hpp>
#include <riak_client/cxx/object/link.hpp>
#include <riak_client/cxx/object/riak_metadata.hpp>
#include <string>
#include <vector>

namespace riak {

class RIAKC_API riak_content {
public: // constructors
    explicit riak_content() { }
    explicit riak_content(const std::string& value)
        : value_(value) { }
    riak_content(const riak_metadata& metadata, const std::string& value)
        : metadata_(metadata), value_(value) { }
public: // accessors
    const std::string&   value() const { return value_; }
    const riak_metadata& metadata() const { return metadata_; }
    const link_vector&   links() const { return links_; }
public:  // mutators
    void  value(const std::string& value) { value_ = value; }
    void  metadata(const riak_metadata& md) { metadata_ = md; }
    void  links(const link_vector& links) { links_ = links; }
 public:
    void  debug_print() const;
private: // intentionally copyable
    riak_metadata metadata_;
    std::string value_;
    link_vector links_;
};
typedef std::vector<riak_content> content_vector;

} // ::riak

#endif // include guard
