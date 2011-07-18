AC_DEFUN([AC_HEADER_SHARED_PTR], [
  AC_CACHE_CHECK(for tr1/shared_ptr,
  ac_cv_cxx_tr1_shared_ptr,
  [AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  AC_TRY_COMPILE([#include <tr1/memory>], [using std::tr1::shared_ptr;],
  ac_cv_cxx_tr1_shared_ptr=yes, ac_cv_cxx_tr1_shared_ptr=no)
  AC_LANG_RESTORE
  ])
  if test "$ac_cv_cxx_tr1_shared_ptr" = yes; then
    AC_DEFINE(HAVE_TR1_SHARED_PTR,,[Define if tr1/shared_ptr is present. ])
  fi
])
