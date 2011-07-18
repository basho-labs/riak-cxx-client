AC_INIT([libriak_client], [1.0], [andy@andygross.org], [riak_client],
        [http://github.com/argv0/riak-cxx-client])
AC_PREREQ([2.59])
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([1.0 -Wall no-define foreign no-dist])
AM_MAINTAINER_MODE([disable])
AC_CONFIG_HEADERS([riak_client/config.h])
AC_CONFIG_MACRO_DIR([m4])

AC_PROG_CXX
AC_PROG_CC
AC_LANG([C++])
ACX_CHECK_SUNCC
AC_PROG_LIBTOOL
AC_HEADER_SHARED_PTR
AC_HEADER_BOOST_SHARED_PTR
gl_VISIBILITY
AC_HEADER_STDC
LT_INIT([])

AC_SUBST([RIAK_CLIENT_SO_VERSION], [1:0:0])
AC_SUBST([RIAK_CLIENT_API_VERSION], [1.0])

AC_CONFIG_FILES([Makefile deps/Makefile riak_client.pc])
AC_OUTPUT
