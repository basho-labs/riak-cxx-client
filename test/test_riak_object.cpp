#include <boost/test/unit_test.hpp>
#include <riak_client/cxx/object/riak_metadata.hpp>


BOOST_AUTO_TEST_CASE (test_metadata)
{
    riak::string_map usermeta;
    usermeta["foo"] = "bar";
    riak::riak_metadata md(usermeta);
    md.content_type("content_type");
    md.charset("charset");
    md.encoding("encoding");
    md.vtag("vtag");
    md.lastmod(1, 1);
    BOOST_REQUIRE(md.usermeta().at("foo") == "bar");
    BOOST_REQUIRE(md.content_type() == "content_type");
    BOOST_REQUIRE(md.charset() == "charset");
    BOOST_REQUIRE(md.vtag() == "vtag");
    BOOST_REQUIRE(md.lastmod().first == 1);
    BOOST_REQUIRE(md.lastmod().second == 1);
}




