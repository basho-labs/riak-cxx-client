#include "riakc_console.hpp"
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

int parse_path_part(const std::string& path, std::string& part, int start=0);
riak::riak_bkey parse_path();
riak::client_ptr make_client(string url_str)
{
    if (url_str.empty())
        url_str = DEFAULT_URL;
    URL = new riak::url(url_str);
    riak::client_ptr client(riak::new_client(URL->host(), URL->port()));
    return client;
}

/*
struct path_context
{
    std::string bucket;
    std::string key;
    riak::object_ptr object;
};

class riak_console;
typedef boost::function< void (const riak::string_vector&)>   func;
typedef pair<string, func> element;

struct command : public element
{
    command(const string& a1, func a2) 
        : element(a1, a2) { }
    operator string() const { return first; }
};
typedef list<command> command_list;

class lookup
{
public:
    lookup(const riak::string_vector& tokens)
        : tokens_(tokens) { }

    bool operator()(const command& element) const
    {
        return strncmp(tokens_.begin()->c_str(),
                       (static_cast<string>(element)).c_str(),
                       tokens_.begin()->size()) == 0;
    }
private:
    const riak::string_vector& tokens_;
};
*/
void info_command(riak_console& client, const riak::string_vector& tokens);
void list_command(riak_console& client, const riak::string_vector& tokens);
void get_command(riak_console& client, const riak::string_vector& tokens);
void put_command(riak_console& client, const riak::string_vector& tokens);

/*
class info_command : command
{
    void operator()(const path_context& path, riak::client_ptr p, 
                    const riak::string_vector& token);

};

class riak_console
{
public:
    riak_console(const std::string& urlstr)
        : done_(false),
          client_(make_client(urlstr)),
          commands_(),
          reader_("/tmp/rlhist"),
          eoi_(false),
          urlstr_(urlstr)
    {
        init_commands();
    }

    void info(const riak::string_vector& args)
    {
        info_command(*this, args);
    }

    void list(const riak::string_vector& args)
    {
        list_command(*this, args);
    }

    void cd(const riak::string_vector& args)
    {
        if (args.size() == 1)
        {
            set_context("", "");
            return;
        }   
        if (context_.bucket.empty())
            context_.bucket = args[1];
        else
            context_.key = args[1];
    }


    void help(const riak::string_vector& args)
    {
        for (command_list::const_iterator i(commands_.begin());
             i != commands_.end(); ++i )
            cout << static_cast<string>(*i) << endl;
    }

    void get(const riak::string_vector& args)
    {
        get_command(*this, args);
    }

    void put(const riak::string_vector& args)
    {
        put_command(*this, args);
    }
        

    template <typename F>
    void add_command(const std::string& name, F f)
    {
        commands_.push_back(command(name,
            boost::bind(f, this, _1)));
    }
    void init_commands()
    {
        add_command("help", &riak_console::help);
        add_command("cd", &riak_console::cd);
        add_command("get", &riak_console::get);
        add_command("put", &riak_console::put);
        add_command("put %file", &riak_console::put);
        add_command("list", &riak_console::list);
        add_command("ls", &riak_console::list);
        add_command("info", &riak_console::info);
        reader_.RegisterCompletions(commands_);
    }

    void set_context(const std::string& bucket)
    {
        context_.bucket = bucket;
        context_.key.clear();
        context_.object.reset();
    }

    void set_context(const std::string& bucket, const std::string& key)
    {
        context_.bucket = bucket;
        context_.key = key;
        context_.object.reset();
    }

    void set_context(const std::string& bucket, const std::string& key, riak::object_ptr o)
    {
        context_.bucket = bucket;
        context_.key = key;
        context_.object = o;
    }

    int stop() 
    {
        cout << "exiting." << endl;
        done_ = true;
        return 0;
    }

    const std::string& prompt()
    {
        prompt_ = "<" + urlstr_ + "/" + context_.bucket;
        if (!context_.key.empty()) 
            prompt_ += "/" + context_.key;
        prompt_ += "> ";
        return prompt_;
    }
    
    int run_once() 
    {
        reader_.GetLine(prompt(), tokens_, eoi_);
        if (eoi_) return stop();
        if (tokens_.empty()) { return 0; }
        if (*tokens_.begin() == "exit") { return stop(); }
        command_ = find_if(commands_.begin(), commands_.end(), lookup(tokens_));
        if (command_ == commands_.end()) 
        {
            cout << "Unknown command.  Type 'help' for a list of commands" << endl;
            return 0;
        }
        if (command_->second != 0) command_->second(tokens_);
        return 0;
    }

    int run() 
    {
        cout << "riakc version 1.0" << endl;
        cout << "Type 'help' for more information, Ctrl+D to exit." << endl;
        int retval = 0;
        while (!done_)
            retval = run_once();
        return retval;
    }

    const path_context& context() const { return context_; }
    riak::client_ptr& client() { return client_; }

private:
    bool done_;
    riak::client_ptr client_;
    command_list commands_;
    swift::SReadline reader_;
    bool eoi_;
    vector<string> tokens_;
    std::string urlstr_;
    path_context context_;
    std::string prompt_;
    command_list::iterator command_;
};
*/

void get_command(riak_console& client, const riak::string_vector& tokens)
{
    if (tokens.empty()) return;
    if (client.context().bucket.empty()) return;
    if (tokens.size() != 2) return;
    std::string key(tokens[1]);
    riak::riak_result result(
      client.client()->fetch(client.context().bucket, key, 1, 1));
    cout << endl;
    if (result.not_found())
        cout << "not found." << endl;
    else 
    {
        cout << endl;
        result.choose_sibling(0)->debug_print();
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
    riak::riak_result result(
      client.client()->fetch(client.context().bucket, key, 1, 1));
    riak::object_ptr o;
    if (result.not_found())
    {
        o = riak::make_object(client.context().bucket, key, value);
    }
    else 
    {
        o = result.choose_sibling(0);
        o->update_value(value);
    }
    riak::store_params sp;
    sp.w(1).dw(1);
    riak::response<riak::riak_result> r(client.client()->store(o, sp));
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

int parse_path_part(const std::string& path, std::string& part, int start)
{
    if (path.empty()) return start;
    if (path[start] == '/')
        start += 1;
    for (start;start<path.size();++start) {
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
    riak::riak_result result = client->fetch(bkey.bucket(), bkey.key(), 1, 1);
    if (result.not_found())
    {
        cout << "ERROR:  " << URL->path() << " not found." << endl;
        return;
    }
    riak::object_ptr object = result.choose_sibling(0);
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
    int url_idx = 2;
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
        riak_console console(urlstr, make_client(urlstr));
        return console.run();
    }
    riak::client_ptr client(make_client(urlstr));
    if (command == "server-info")
        print_server_info(client);
    if (command == "get") 
        do_get(client);
}
