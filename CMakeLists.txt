CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(riak_client) 

INCLUDE(CTest)

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

SET(Boost_USE_MULTITHREADED ON)
FIND_PACKAGE(Boost 1.47 REQUIRED system thread)
FIND_PACKAGE(Protobuf 2.4.1 REQUIRED)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
ADD_DEFINITIONS(-DHAVE_BOOST_SHARED_PTR -DRIAKC_USE_BOOST_SHARED_PTR)

PROTOBUF_GENERATE_CPP(RIAK_PROTO_SOURCES RIAK_PROTO_HEADERS
                      src/riak_kv.proto
                      src/riak.proto
                      src/riak_search.proto)

SET(RIAK_CLIENT_SOURCES
    ${RIAK_PROTO_SOURCES}
    src/client.cpp
    src/cxx/basic_client.cpp
    src/cxx/connection.cpp
    src/cxx/pbc_client.cpp
    src/cxx/pbc_header.cpp
    src/cxx/pbc_message.cpp
    src/cxx/riak_object.cpp
    src/cxx/url.cpp
    src/string_list.cpp
    src/string_map.cpp)

ADD_LIBRARY(riak_client SHARED ${RIAK_CLIENT_SOURCES})
SET_TARGET_PROPERTIES(riak_client PROPERTIES
                      VERSION 1.0.0
                      SOVERSION 1.0)
TARGET_LINK_LIBRARIES(riak_client ${PROTOBUF_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})

ADD_EXECUTABLE(riakc src/riakc/riakc.cpp)
TARGET_LINK_LIBRARIES(riakc riak_client)

INCLUDE(BoostTestTargets)
ADD_BOOST_TEST(TestRiakBasic SOURCES test/test_basic.cpp LIBRARIES riak_client)
ADD_BOOST_TEST(TestRiakObject SOURCES test/test_riak_object.cpp LIBRARIES riak_client)

INCLUDE(GNUInstallDirs)

INSTALL(TARGETS riak_client riakc
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

INSTALL(DIRECTORY riak_client DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/riak_client.pc.in"
               "${CMAKE_CURRENT_BINARY_DIR}/riak_client.pc"
               IMMEDIATE @ONLY)

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/riak_client.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
               "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
               IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
