AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4
DEPS_CXXFLAGS = "$(CFLAG_VISIBILITY) -fPIC"

BUILD_DIR = $(abspath ../build/deps)
PROTOBUF_SRC = protobuf-2.4.1
BOOST_SRC = boost-1.47.0


all-local: boost.ts protobufs.ts

boost.ts:
	cd $(BOOST_SRC) && make DEPS_CXXFLAGS=$(DEPS_CXXFLAGS)
	touch boost.ts

protobufs.ts: $(PROTOBUF_SRC)/Makefile
	cd $(PROTOBUF_SRC) && make && make install
	touch protobufs.ts


boost-test.ts: 
	cd $(BOOST_SRC) && make DEPS_CXXFLAGS=$(DEPS_CXXFLAGS) test
	touch boost-test.ts

$(PROTOBUF_SRC)/Makefile:
	cd $(PROTOBUF_SRC) && CXXFLAGS=$(DEPS_CXXFLAGS) ./configure \
	--prefix=$(BUILD_DIR) --disable-shared

check-local: boost-test.ts


clean-local:

distclean-local:
	cd $(PROTOBUF_SRC) && make distclean
	cd $(BOOST_SRC) && make clean
	rm -f protobufs.ts
	rm -f boost.ts
	rm -f boost-test.ts

