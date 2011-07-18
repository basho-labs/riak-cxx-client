#ifndef RIAKC_COMMON_H_
#define RIAKC_COMMON_H_

#include <riak_client/config.h>

#ifndef __GNUC__
#define  __attribute__(x) 
#endif // __GNUC__

#ifdef HAVE_VISIBILITY
#define RIAKC_API __attribute__((visibility("default")))
#else
#define RIAKC_API 
#endif // HAVE_VISIBILITY

#endif // include guard
