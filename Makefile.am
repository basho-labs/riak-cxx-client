AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4
lib_LTLIBRARIES = libriak_client.la
libriak_client_la_SOURCES = src/cxx/pbc_client.cpp \
                            src/cxx/pbc_message.cpp \
			    src/cxx/pbc_header.cpp \
	                    src/cxx/connection.cpp \
			    src/cxx/riakclient.pb.cc \
	                    src/cxx/basic_client.cpp \
	   		    src/cxx/riak_object.cpp \
			    src/cxx/url.cpp \
			    src/client.cpp \
                            src/string_list.cpp \
			    src/string_map.cpp

libriak_client_la_CXXFLAGS = -g3 -Ibuild/deps/include $(CFLAG_VISIBILITY) 
libriak_client_la_LDFLAGS = -Lbuild/deps/lib -lprotobuf-lite \
			    -lriakboost_system-mt-riak_client \
			    -lriakboost_thread-mt-riak_client \
			    -version-info 1:0:0 -no-undefined

riak_client_includedir = $(includedir)/riak_client/cxx
nobase_include_HEADERS = riak_client/cxx/riak_client.hpp \
			 riak_client/cxx/riak_client_fwd.hpp \
			 riak_client/cxx/common.hpp \
			 riak_client/cxx/error.hpp \
			 riak_client/cxx/basic.hpp \
			 riak_client/cxx/basic/basic_client.hpp \
			 riak_client/cxx/basic/bucket_properties.hpp \
			 riak_client/cxx/basic/riak_result.hpp \
			 riak_client/cxx/basic/response.hpp \
			 riak_client/cxx/basic/server_info.hpp \
			 riak_client/cxx/basic/store_params.hpp \
			 riak_client/cxx/client.hpp \
			 riak_client/cxx/client/client.hpp \
			 riak_client/cxx/client/bucket.hpp \
			 riak_client/cxx/client/resolver.hpp \
			 riak_client/cxx/object.hpp \
			 riak_client/cxx/object/link.hpp \
			 riak_client/cxx/object/riak_content.hpp \
		 	 riak_client/cxx/object/riak_metadata.hpp \
			 riak_client/cxx/object/riak_object.hpp \
			 riak_client/cxx/object/riak_version.hpp \
			 riak_client/cxx/operations.hpp \
			 riak_client/cxx/operations/fetch_object.hpp \
			 riak_client/cxx/operations/delete_object.hpp \
			 riak_client/cxx/operations/store_object.hpp \
			 riak_client/cxx/util.hpp \
			 riak_client/cxx/util/shared_ptr.hpp \
			 riak_client/cxx/util/noncopyable.hpp \
			 riak_client/cxx/util/url.hpp \
			 riak_client/client.h \
			 riak_client/common.h \
                         riak_client/config.h \
			 riak_client/string_list.h \
			 riak_client/string_map.h \
			 riak_client/riak_object.h \
			 riak_client/riak_metadata.h

riak_client_libincludedir = $(libdir)/riak_client/include

bin_PROGRAMS = riakc
riakc_SOURCES = src/riakc/riakc.cpp
riakc_CXXFLAGS = $(libriak_client_la_CXXFLAGS) 
# -I/usr/local/Cellar/readline/6.2.1/include
riakc_LDFLAGS = -lriak_client
 # -L/usr/local/Cellar/readline/6.2.1/lib -lreadline

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = riak_client.pc

check_PROGRAMS = riak_cxx_unit_test
TESTS = $(check_PROGRAMS)

riak_cxx_unit_test_SOURCES = test/test_riak_object.cpp test/test_basic.cpp
riak_cxx_unit_test_CXXFLAGS = $(libriak_client_la_CXXFLAGS)
riak_cxx_unit_test_LDFLAGS = -lriak_client -lriakboost_unit_test_framework-mt-riak_client

#riak_cxx_test_server_SOURCES = test/test_server.cpp src/cxx/pbc_header.cpp src/cxx/pbc_message.cpp src/cxx/riakclient.pb.cc
#riak_cxx_test_server_CXXFLAGS = $(libriak_client_la_CXXFLAGS) -Isrc/cxx
#riak_cxx_test_server_LDFLAGS =  $(libriak_client_la_LDFLAGS) -lriak_client 

CLEANFILES = $(check_PROGRAMS)
SUBDIRS = deps
DIST_SUBDIRS = deps
