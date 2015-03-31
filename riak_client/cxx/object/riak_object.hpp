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

#ifndef RIAKCXX_RIAK_OBJECT_HPP_
#define RIAKCXX_RIAK_OBJECT_HPP_

#include <riak_client/cxx/riak_client_fwd.hpp>
#include <riak_client/cxx/object/riak_metadata.hpp>
#include <riak_client/cxx/object/riak_content.hpp>
#include <riak_client/cxx/object/riak_version.hpp>

namespace riak {

class RIAKC_API object : public riak_version
{
public: // constructors
    explicit object(const riak_version& version)
        : riak_version(version) { }
    object(const riak_version& version, const riak_content& content)
        : riak_version(version), content_(content), update_content_(content) { }
public: // accessors
    const riak_content&  content() const { return content_; }
    const std::string&   value() const { return content_.value(); }
    const riak_metadata& metadata() const { return content_.metadata(); }
    const riak_metadata& update_metadata() const { return update_content_.metadata(); }
public: // mutators
    riak_content&        update_content() { return update_content_; }
    void                 update_value(const std::string& value) { update_content_.value(value); }
    void                 update_metadata(const riak_metadata& md) { update_content_.metadata(md); }
    void                 debug_print() const;
private: // intentionally copyable
    riak_content content_;
    riak_content update_content_;
};

inline object_ptr make_object(const std::string& bucket,const std::string& key)
{
    return object_ptr(new object(riak_bkey(bucket, key)));
}

inline object_ptr make_object(const std::string& bucket,const std::string& key,
                              const std::string& value)
{
    riak_version version(riak_bkey(bucket, key));
    riak_content content(value);
    return object_ptr(new object(version, content));
}


template <class T>
T riak_object_cast(object_ptr p);

template <>
inline std::string riak_object_cast<std::string>(object_ptr p)
{
    if (!p)
        return "";
    return p->value();
}

} // ::riak

#endif // include guard
