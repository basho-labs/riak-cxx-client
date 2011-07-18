#include <riak_client/cxx/object/riak_object.hpp>

namespace riak {

riak_metadata::riak_metadata() 
{
}

riak_metadata::riak_metadata(const string_map& usermeta)
  : usermeta_(usermeta) 
{
}

} // ::riak
