/**
  @file    riak_metadata.hpp
  @brief   Riak Object Metadata */
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

#ifndef RIAKCXX_RIAK_METADATA_HPP_
#define RIAKCXX_RIAK_METADATA_HPP_

#include <riak_client/cxx/riak_client_fwd.hpp>
#include <utility>
#include <stdint.h>

namespace riak {

typedef std::pair<uint32_t, uint32_t> lastmod_time_t;

/// A riak_metadata stores metadata associated with a #riak::object.
///
/// <b>Thread safety</b>: not thread safe
///
/// <b>Copying</b>: intentionally copyable
///
class RIAKC_API riak_metadata
{
public:
    /// Construct a an empty metadata object.
    explicit riak_metadata();

    /// Construct a metadata object from user metadata.
    ///
    /// @param usermeta:  User-defined metadata.
    explicit riak_metadata(const string_map& usermeta);
public:
    /// Return the content-type.
    const std::string& content_type() const;
    /// Return the charset.
    const std::string& charset() const;
    /// Return the content-encoding.
    const std::string& encoding() const;
    /// Return a vtag - a hashed representation of a vector clock.
    const std::string& vtag() const;
    /// Return the user metadata.
    const string_map&  usermeta() const;
    /// Return the last-modified time.
    const lastmod_time_t lastmod() const;
public:
    // Set the content-type.
    void content_type(const std::string& content_type);
    // Set the charset.
    void charset(const std::string& charset);
    // Set the content-encoding.
    void encoding(const std::string& encoding);
    // Set the vtag.
    void vtag(const std::string& vtag);
    void usermeta(const string_map& usermeta);
    // Set the last-modified time.
    void lastmod(lastmod_time_t lastmod);
    void lastmod(uint32_t secs, uint32_t usecs);
    void debug_print() const;
private:
    string_map  usermeta_;
    std::string content_type_;
    std::string charset_;
    std::string encoding_;
    std::string vtag_;
    lastmod_time_t last_mod_;
};

inline const std::string&
riak_metadata::content_type() const
{
    return content_type_;
}

inline const std::string&
riak_metadata::charset() const
{
    return charset_;
}

inline const std::string&
riak_metadata::encoding() const
{
    return encoding_;
}

inline const std::string&
riak_metadata::vtag() const
{
    return vtag_;
}

inline const lastmod_time_t
riak_metadata::lastmod() const
{
    return last_mod_;
}

inline const string_map&
riak_metadata::usermeta() const
{
    return usermeta_;
}

inline void
riak_metadata::content_type(const std::string& content_type)
{
    content_type_ = content_type;
}

inline void
riak_metadata::charset(const std::string& charset)
{
    charset_ = charset;
}

inline void
riak_metadata::encoding(const std::string& encoding)
{
   encoding_ = encoding;
}

inline void
riak_metadata::usermeta(const string_map& usermeta)
{
    usermeta_ = usermeta;
}

inline void
riak_metadata::vtag(const std::string& vtag)
{
    vtag_ = vtag;
}

inline void
riak_metadata::lastmod(lastmod_time_t lastmod)
{
    last_mod_ = lastmod;
}

inline void
riak_metadata::lastmod(uint32_t secs, uint32_t usecs)
{
    last_mod_ = lastmod_time_t(secs, usecs);
}

} // ::riak

#endif // include guard
