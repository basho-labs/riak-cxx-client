#include <riak_client/cxx/util/url.hpp>
#include <string>
#include <algorithm>
#include <cctype>
#include <functional>

using namespace std;

namespace riak {

url::url(const string& url_str)
{
    parse(url_str);
}


void url::parse(const string& url_s)
{
    const string prot_end("://");
    const string host_end(":");
    string::const_iterator prot_i = search(url_s.begin(), url_s.end(),
                                           prot_end.begin(), prot_end.end());
    protocol_.reserve(distance(url_s.begin(), prot_i));
    transform(url_s.begin(), prot_i,
              back_inserter(protocol_),
              ptr_fun<int,int>(tolower));
    if( prot_i == url_s.end() )
        return;
    advance(prot_i, prot_end.length());
    string::const_iterator port_i = find(prot_i, url_s.end(), ':');
    string::const_iterator path_i = find(prot_i, url_s.end(), '/');
    if ( (port_i != url_s.end()) && (port_i < path_i) )
    {
        host_.reserve(distance(prot_i, port_i));
        transform(prot_i, port_i,
                  back_inserter(host_),
                  ptr_fun<int,int>(tolower));
        port_.assign(++port_i, path_i);
    }
    else
    {
        host_.reserve(distance(prot_i, path_i));
        transform(prot_i, path_i,
                  back_inserter(host_),
                  ptr_fun<int,int>(tolower));
    }
    string::const_iterator query_i = find(path_i, url_s.end(), '?');
    path_.assign(path_i, query_i);
    if( query_i != url_s.end() )
        ++query_i;
    query_.assign(query_i, url_s.end());
}

} // :: riak
