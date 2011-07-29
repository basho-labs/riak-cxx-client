BUILD_DIR       = ../../build
DEPS_BUILD_DIR  = $(BUILD_DIR)/deps
DEP_INCLUDE_DIR = $(DEPS_BUILD_DIR)/include
DEP_LIB_DIR     = $(DEPS_BUILD_DIR)/lib

BOOST_SUFFIX = -mt-riak_client

BOOST_SYSTEM_A   = $(DEP_LIB_DIR)/libriakboost_system$(BOOST_SUFFIX).a
BOOST_THREAD_A   = $(DEP_LIB_DIR)/libriakboost_thread$(BOOST_SUFFIX).a
BOOST_DATETIME_A = $(DEP_LIB_DIR)/libriakboost_date_time$(BOOST_SUFFIX).a

BOOST_LIBS = $(BOOST_SYSTEM_A) $(BOOST_THREAD_A) $(BOOST_DATETIME_A)

all: install

tools/build/v2/bjam:
	cd tools/build/v2 && ./bootstrap.sh 

install: tools/build/v2/bjam
	tools/build/v2/bjam --v2 cxxflags="-fPIC -fvisibility=hidden" --prefix=../../build/deps --buildid=riak_client --layout=tagged --without-test link=static threading=multi install

test: tools/build/v2/bjam
	tools/build/v2/bjam --v2 cxxflags="-fPIC -fvisibility=hidden" --prefix=../../build/deps --buildid=riak_client --layout=tagged --with-test link=static threading=multi install

clean:
	tools/build/v2/bjam --v2 clean && rm -Rf ../../build/deps/include/boost && rm -Rf ../../build/deps/lib/libriakboost*.a && rm -Rf bin.v2
