AC_DEFUN([gl_VISIBILITY],
[
  AC_REQUIRE([AC_PROG_CC])
  CFLAG_VISIBILITY=
  HAVE_VISIBILITY=0
  if test -n "$GCC"; then
    AC_MSG_CHECKING([for simple visibility declarations])
    AC_CACHE_VAL(gl_cv_cc_visibility, [
      gl_save_CFLAGS="$CFLAGS"
      CFLAGS="$CFLAGS -fvisibility=hidden"
      AC_TRY_COMPILE(
        [extern __attribute__((__visibility__("hidden"))) int hiddenvar;
         extern __attribute__((__visibility__("default"))) int exportedvar;
         extern __attribute__((__visibility__("hidden"))) int hiddenfunc (void);
         extern __attribute__((__visibility__("default"))) int exportedfunc (void);],
        [],
        gl_cv_cc_visibility=yes,
        gl_cv_cc_visibility=no)
      CFLAGS="$gl_save_CFLAGS"])
    AC_MSG_RESULT([$gl_cv_cc_visibility])
    if test $gl_cv_cc_visibility = yes; then
      CFLAG_VISIBILITY="-fvisibility=hidden"
      HAVE_VISIBILITY=1
    fi
  fi
  AC_SUBST([CFLAG_VISIBILITY])
  AC_SUBST([HAVE_VISIBILITY])
  AC_DEFINE_UNQUOTED([HAVE_VISIBILITY], [$HAVE_VISIBILITY],
    [Define to 1 or 0, depending whether the compiler supports simple visibility declarations.])
])