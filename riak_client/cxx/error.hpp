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

#ifndef RIAKCXX_ERROR_HPP_
#define RIAKCXX_ERROR_HPP_

#include <stdexcept>
#include <string>

namespace riak { 

class RIAKC_API riak_error
{
public: // constructors
    riak_error()
        : code_(0) { }
    riak_error(int code)
        : code_(code) { }
    riak_error(int code, const std::string& message)
        : code_(code), message_(message) { }
public: // accessors
    int                 code() const { return code_; }
    const std::string&  message() const { return message_; }
public:  // mutators
    void        code(int code) { code_ = code; }
    void        message(const std::string& msg) { message_ = msg; }
public:  // conversions
    operator    bool() const { return code_; }
 private: // intentionally copyable
    int code_;
    std::string message_;
};

class RIAKC_API exception : public std::exception
{
public:
    exception(const std::string& message)
        : error_(0, message) { }
    exception(const riak_error& error)
        : error_(error) { }
    virtual ~exception() throw() { }
public:
    const char* what() const throw() { return error_.message().c_str(); }
    int         error_code()  { return error_.code(); }
private:
    riak_error error_;
};



} // ::riak


#endif // include guard
