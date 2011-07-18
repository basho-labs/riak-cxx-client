AC_DEFUN([AC_HEADER_BOOST_SHARED_PTR], [
  AC_CACHE_CHECK(for boost/shared_ptr,
  ac_cv_cxx_boost_shared_ptr,
  [AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  AC_TRY_COMPILE([#include <boost/shared_ptr.hpp>], [using boost::shared_ptr;],
  ac_cv_cxx_boost_shared_ptr=yes, ac_cv_cxx_boost_shared_ptr=no)
  AC_LANG_RESTORE
  ])
  if test "$ac_cv_cxx_boost_shared_ptr" = yes; then
    AC_DEFINE(HAVE_BOOST_SHARED_PTR,,[Define if boost/shared_ptr is present. ])
  fi
])
