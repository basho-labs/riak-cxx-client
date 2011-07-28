#ifndef RIAKC_CONSOLE_HPP_
#define RIAKC_CONSOLE_HPP_

#include "SReadline.h"
#include <riak_client/cxx/riak_client.hpp>
#include <boost/function.hpp>
#include <boost/functional.hpp>
#include <boost/bind.hpp>
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>

using std::cout;
using std::endl;
using std::string;

class riak_console;
typedef boost::function< void (const riak::string_vector&)> func;
typedef std::pair<string, func> element;

struct path_context
{
    string bucket;
    string key;
    riak::object_ptr object;
};

struct command : public element
{
    command(const string& a1, func a2) 
        : element(a1, a2) { }
    operator string() const { return first; }
    void operator()(const riak::string_vector& tokens);
};

typedef std::list<command> command_list;

/* command lookup functor */
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


class riak_console
{
public:
     riak_console(const std::string& urlstr, riak::client_ptr client)
        : done_(false),
          client_(client),
          commands_(),
          reader_("/tmp/rlhist"),
          eoi_(false),
          urlstr_(urlstr)
    {
        init_commands();
    }

    void info(const riak::string_vector& args)
    {
        
 //info_command(*this, args);
    }

    void list(const riak::string_vector& args)
    {
//        list_command(*this, args);
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
//        get_command(*this, args);
    }

    void put(const riak::string_vector& args)
    {
//        put_command(*this, args);
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
    riak::string_vector tokens_;
    std::string urlstr_;
    path_context context_;
    std::string prompt_;
    command_list::iterator command_;
};


#endif 
