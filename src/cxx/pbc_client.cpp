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

#include "pbc_client.hpp"
#include "pbc_operations.hpp"
#include "pbc_header.hpp"
#include "connection.hpp"
#include <cassert>
#include <riak_client/cxx/object/riak_object.hpp>

namespace riak { namespace pbc {

using std::string;

void encode_object(object_ptr obj, ops::put::request_type& req)
{
    req.set_bucket(obj->bucket());
    req.set_key(obj->key());
    req.set_vclock(obj->vclock());
    RpbContent* c = req.mutable_content();
    c->set_value(obj->update_content().value());
    c->set_content_type(obj->update_metadata().content_type());
    c->set_content_encoding(obj->update_metadata().encoding());
    c->set_charset(obj->update_metadata().charset());
    c->set_vtag(obj->update_metadata().vtag());
    c->set_last_mod(obj->update_metadata().lastmod().first);
    c->set_last_mod_usecs(obj->update_metadata().lastmod().second);
    for (string_map::const_iterator it = obj->update_metadata().usermeta().begin() ;
         it != obj->update_metadata().usermeta().end();
         ++it)
    {
        RpbPair *entry = c->add_usermeta();
        entry->set_key((*it).first);
        entry->set_value((*it).second);
    }
    for (link_vector::const_iterator it = obj->update_content().links().begin();
         it != obj->update_content().links().end();
         ++it)
    {
        RpbLink* l = c->add_links();
        l->set_bucket((*it).bucket());
        l->set_key((*it).key());
        l->set_tag((*it).tag());
    }
}

template <class T>
void decode_contents(const T& response, content_vector& contents)
{
    for (int i=0;i<response.content_size();++i)
    {
        RpbContent content = response.content(i);
        string_map usermeta;
        for (int j=0;j<content.usermeta_size();++j)
        {
            RpbPair pb_metadata = content.usermeta(j);
            usermeta[pb_metadata.key()] = pb_metadata.value();
        }
        riak_metadata md(usermeta);
        md.content_type(content.content_type());
        md.charset(content.charset());
        md.encoding(content.content_encoding());
        md.vtag(content.vtag());
        md.lastmod(content.last_mod(), content.last_mod_usecs());
        link_vector links;
        for (int j=0;j<content.links_size();++j)
        {
            RpbLink pb_link = content.links(j);
            links.push_back(link(pb_link.bucket(), pb_link.key(), pb_link.tag()));
        }
        riak_content c(md, content.value());
        c.links(links);
        contents.push_back(c);
    }
}

const pbc_header
pbc_recv_header(connection_ptr c, riak_error& error)
{
    pbc_header header;
    char header_buf[5];
    c->read(io::buffer(&header_buf[0], 5));
    header.deserialize(header_buf, pbc_header::HEADER_SIZE);
    if (header.code() == ERROR)
    {
        pbc_storage storage(header.size());
        error_resp err;
        c->read(io::buffer(storage.data(), storage.size()));
        err.deserialize(storage);
        error.code(err.errcode());
        error.message(err.errmsg());
    }
    return header;
}

template <class Message>
std::size_t pbc_recv(connection_ptr c,  Message& m)
{
    riak_error err;
    pbc_header header = pbc_recv_header(c, err);
    if (err) return err;
    pbc_storage storage(header.size());
    std::size_t n = c->read(io::buffer(storage.data(), storage.size()));
    if (n != header.size())
        return riak_error(); // XXX
    m.deserialize(storage);
    return riak_error();
}


template <class Operation>
riak_error execute(connection_ptr c, Operation& op)
{
    pbc_storage storage(op.request().size()+pbc_header::HEADER_SIZE);
    op.request().serialize(storage);
    std::size_t n = c->write(io::buffer(storage.data(), storage.size()));
    assert(n = storage.size());
    riak_error error = pbc_recv(c, op.response());
    return error;
}

pbc_client::pbc_client(const string& host, const string& port)
    : connection_(new connection(host, port))
{
    connection_->start();
    //client_id(tss_client_id());
}

pbc_client::~pbc_client()
{
}

response<bool>
pbc_client::ping()
{
    ops::ping operation;
    riak_error error = execute(connection_, operation);
    if (error) return error;
    return true;
}

response<server_info>
pbc_client::get_server_info()
{
    ops::get_server_info operation;
    riak_error error = execute(connection_, operation);
    if (error) return error;
    server_info s(operation.response().node(),
                  operation.response().server_version());
    return s;
}

response<bool>
pbc_client::del(const string& bucket, const string& key, int dw)
{
    ops::del operation;
    operation.request().set_bucket(bucket);
    operation.request().set_key(key);
    operation.request().set_rw(dw);
    riak_error error = execute(connection_, operation);
    if (error) return error;
    return true;
}


response<result_ptr>
pbc_client::fetch(const string& bucket, const string& key, int r)
{
    ops::get operation;
    operation.request().set_bucket(bucket);
    operation.request().set_key(key);
    operation.request().set_r(r);
    riak_error error = execute(connection_, operation);
    if (error) return error;
    content_vector contents;
    decode_contents(operation.response(), contents);
    riak_version version(riak_bkey(bucket, key), operation.response().vclock());
    return result_ptr(new riak_result(version, contents));
}

response<bool>
pbc_client::set_bucket(const string& bucket, const bucket_properties& properties)
{
    ops::set_bucket operation;
    operation.request().set_bucket(bucket);
    operation.request().mutable_props()->set_allow_mult(properties.allow_mult());
    operation.request().mutable_props()->set_n_val(properties.n_val());
    riak_error error = execute(connection_, operation);
    if (error) return error;
    return true;
}

response<bucket_properties>
pbc_client::fetch_bucket(const string& bucket)
{
    ops::get_bucket operation;
    operation.request().set_bucket(bucket);
    riak_error error = execute(connection_, operation);
    if (error) return error;
    bucket_properties bprops;
    bprops.allow_mult(operation.response().props().allow_mult());
    bprops.n_val(operation.response().props().n_val());
    return bprops;
}

response<uint32_t>
pbc_client::client_id()
{
    ops::get_client_id operation;
    riak_error error = execute(connection_, operation);
    if (error) return error;
    const char* client_id_str = operation.response().client_id().c_str();
    uint32_t *id = (uint32_t *)client_id_str;
    return *id;
}

response<bool>
pbc_client::client_id(uint32_t client_id)
{
    client_id_ = client_id;
    ops::set_client_id operation;
    operation.request().set_client_id(&client_id_, sizeof(client_id));
    riak_error error = execute(connection_, operation);
    if (error) return error;
    return true;
}

response<result_ptr>
pbc_client::store(object_ptr obj, const store_params& params)
{
    ops::put operation;
    encode_object(obj, operation.request());
    operation.request().set_w(params.w());
    operation.request().set_dw(params.dw());
    operation.request().set_return_body(params.return_body());
    riak_error error = execute(connection_, operation);
    if (error) return error;
    content_vector contents;
    decode_contents(operation.response(), contents);
    riak_version version(riak_bkey(obj->bucket(), obj->key()), operation.response().vclock());
    return result_ptr(new riak_result(version, contents));
}

response<string_vector>
pbc_client::list_buckets()
{
    string_vector result;
    ops::list_buckets operation;
    riak_error error = execute(connection_, operation);
    if (error) return error;
    for (int i=0; i<operation.response().buckets_size(); ++i)
        result.push_back(operation.response().buckets(i));
    return result;
}

response<string_vector>
pbc_client::list_keys(const string& bucket)
{
    string_vector result;
    ops::list_keys operation;
    operation.request().set_bucket(bucket);
    riak_error error = execute(connection_, operation);
    if (error) return error;
    do {
        for (int i=0;i<operation.response().keys_size();++i)
            result.push_back(operation.response().keys(i));
        if (operation.response().done()) break;
        pbc_recv(connection_, operation.response());
     } while (true);
    return result;
}

}} // ::riak::pbc
