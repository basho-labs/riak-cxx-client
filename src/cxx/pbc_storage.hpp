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

#ifndef RIAKCXX_PBC_STORAGE_HPP_
#define RIAKCXX_PBC_STORAGE_HPP_

#include <riak_client/cxx/riak_client_fwd.hpp>
#include <cstddef>
#include <cstdlib>
#include <cassert>

namespace riak {

template <typename T, std::size_t SIZE=256>
class auto_buffer : private noncopyable
{
public:
    typedef T value_type;
    typedef std::size_t size_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    static const size_t space = SIZE;
public: // construction
    explicit auto_buffer(size_type size)
        : buffer_((space <  size) ? (T*)malloc(size*sizeof(T)) : &internal_[0]),
          size_((buffer_ != 0) ? size : 0) {}
    ~auto_buffer()
    {
        if (space < size_)
        {
            assert(buffer_ != 0);
            assert(&internal_[0] != buffer_);
            free(buffer_);
        }
    }
public: // operators
    operator pointer() { return buffer_; }
    reference operator[](size_type index) { return buffer_[index]; }
    const_reference operator[](size_type index) const { return buffer_[index]; }
    operator const_pointer() const { return buffer_; }
public:  // accessors
    pointer         data() { return buffer_; }
    const_pointer   data() const { return buffer_; }
    size_type       size() const { return size_; }
    bool            empty() const { return size_ == 0; }
private:
    value_type* buffer_;
    size_type   size_;
    value_type  internal_[SIZE];
};

typedef auto_buffer<char> pbc_storage;

}

#endif // include guard
