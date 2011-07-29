#ifdef HAS_READLINE
#include "riakc_console.hpp"
#endif
#include <riak_client/cxx/riak_client.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <list>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cassert>
#include <sstream>
#include <boost/function.hpp>
#include <boost/functional.hpp>
#include <boost/bind.hpp>

using namespace std;

void print_server_info(riak::client_ptr client);

static std::string DEFAULT_URL = "pbc://127.0.0.1:8087";
static riak::url*  URL;

std::size_t parse_path_part(const std::string& path, std::string& part, std::size_t start=0);
riak::riak_bkey parse_path();
riak::client_ptr make_client(string url_str)
{
    if (url_str.empty())
        url_str = DEFAULT_URL;
    URL = new riak::url(url_str);
    riak::client_ptr client(riak::new_client(URL->host(), URL->port()));
    return client;
}

#ifdef HAS_READLINE
void info_command(riak_console& client, const riak::string_vector& tokens);
void list_command(riak_console& client, const riak::string_vector& tokens);
void get_command(riak_console& client, const riak::string_vector& tokens);
void put_command(riak_console& client, const riak::string_vector& tokens);
void get_command(riak_console& client, const riak::string_vector& tokens)
{
    if (tokens.empty()) return;
    if (client.context().bucket.empty()) return;
    if (tokens.size() != 2) return;
    std::string key(tokens[1]);
    riak::result_ptr result(
      client.client()->fetch(client.context().bucket, key, 1));
    cout << endl;
    if (result->not_found())
        cout << "not found." << endl;
    else 
    {
        cout << endl;
        result->choose_sibling(0)->debug_print();
        cout << endl;
    }
}

void put_command(riak_console& client, const riak::string_vector& tokens)
{
    if (tokens.empty()) return;
    if (client.context().bucket.empty()) return;
    if (tokens.size() != 3) return;
    string arg(tokens[1]);
    string key(tokens[2]);
    string value;
    ifstream input_file(arg.c_str(),ios::binary);
    if (!input_file.is_open())
        value = arg;
    else 
    {
        stringstream buffer;
        buffer << input_file.rdbuf();
        input_file.close();
        value = buffer.str();
    }
    riak::result_ptr result(
                            client.client()->fetch(client.context().bucket, key, 1));
    riak::object_ptr o;
    if (result->not_found())
    {
        o = riak::make_object(client.context().bucket, key, value);
    }
    else 
    {
        o = result->choose_sibling(0);
        o->update_value(value);
    }
    riak::store_params sp;
    sp.w(1).dw(1);
    riak::response<riak::result_ptr> r(client.client()->store(o, sp));
    if (r.error())
        cout << "ERROR(" << r.error().code() << "): " << r.error().message() << endl;
    else {
        cout << endl << "<" << o->bucket() << "/" << o->key();
        cout << "> stored successfully." << endl;
    }
}

void list_command(riak_console& client, const riak::string_vector& tokens)
{
    if (client.context().bucket.empty())
    {
        riak::string_vector buckets = client.client()->list_buckets();
        cout << endl;
        cout << "buckets" << endl;
        cout << "---------------" << endl;
        for (riak::string_vector::const_iterator it=buckets.begin();
             it != buckets.end();
             ++it)
        {
            cout << *it << endl;
        }
        cout << "---------------" << endl;
        cout << buckets.size() << " buckets found\n" << endl;
    }
    else 
    {
        riak::string_vector keys = client.client()->list_keys(client.context().bucket);
        cout << endl;
        cout << "keys" << endl;
        cout << "---------------" << endl;
        for (riak::string_vector::const_iterator it=keys.begin();
             it != keys.end();
             ++it)
        {
            cout << *it << endl;
        }
        cout << "---------------" << endl;
        cout << keys.size() << " keys found\n" << endl;        
    }
}



void info_command(riak_console& client, const riak::string_vector& tokens)
{
    print_server_info(client.client());
}

#endif

std::size_t parse_path_part(const std::string& path, std::string& part, std::size_t start)
{
    if (path.empty()) return start;
    if (path[start] == '/')
        start += 1;
    for (;start<path.size();++start) {
        if (path[start] == '/') break;
        part.push_back(path[start]);
    }
    return start;
}

riak::riak_bkey parse_path()
{
    std::string path(URL->path());
    std::string bucket;
    std::string key;
    if (parse_path_part(path, bucket) < path.size())
        parse_path_part(path, key, bucket.size()+1);
    return riak::riak_bkey(bucket, key);
}

void print_server_info(riak::client_ptr client)
{
    riak::server_info server_info = client->get_server_info();
    cout << "Riak URL: " << URL->protocol() << "://" << URL->host();
    cout << ":" << URL->port() << endl;
    cout << "Node name: " << server_info.node() << endl;
    cout << "Server version: " << server_info.version() << endl;
}



void do_get(riak::client_ptr client)
{
    riak::riak_bkey bkey(parse_path());
    riak::result_ptr result(client->fetch(bkey.bucket(), bkey.key(), 1));
    if (result->not_found())
    {
        cout << "ERROR:  " << URL->path() << " not found." << endl;
        return;
    }
    riak::object_ptr object = result->choose_sibling(0);
    cout << object->value() << std::endl;
}


void print_help()
{
    cout << "Usage: riakc [command] [url]\n" << endl;
    cout << "get,set,server-info,help" << endl;
}

int main(int argc, char *argv[]) 
{
    string command;
    string urlstr = DEFAULT_URL;
    if (argc == 1) 
    {
        command = "help";
    }
    else
    {
        command = argv[1];
        urlstr = argv[2];
    }
    if (command == "help") {
        print_help();
        return 0;
    }
    if (command == "console")
    {
#ifdef HAS_READLINE        
        riak_console console(urlstr, make_client(urlstr));
        return console.run();
#else
        cout << "no readline support" << endl;
#endif
    }
    riak::client_ptr client(make_client(urlstr));
    if (command == "server-info")
        print_server_info(client);
    if (command == "get") 
        do_get(client);
}
