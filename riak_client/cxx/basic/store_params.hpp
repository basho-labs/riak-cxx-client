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

#ifndef RIAKCXX_STORE_PARAMS_HPP_
#define RIAKCXX_STORE_PARAMS_HPP_

namespace riak { 

class RIAKC_API store_params 
{
public:
    store_params()
        : w_(-1), dw_(-1), pw_(-1), return_body_(false) { }
public: // accessors
    int  w() const { return w_; }
    int  dw() const { return dw_; }
    int  pw() const { return pw_; }
    bool return_body() const { return return_body_; }
public: // mutators
    void w(int w) { w_ = w; }
    void dw(int dw) { dw_ = dw; }
    void pw(int pw) { pw_ = pw; }
    void return_body(bool returnbody) { return_body_ = returnbody; }
private: // intentionally copyable
    int w_;
    int dw_;
    int pw_;
    bool return_body_;
};

} // ::riak

#endif // include guard
