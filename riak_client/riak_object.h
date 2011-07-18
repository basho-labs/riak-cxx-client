#ifndef RIAKC_OBJECT_H_
#define RIAKC_OBJECT_H_

#include <riak_client/common.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct riak_metadata_t;
typedef struct riak_metadata_t* riak_metadata;

struct riak_object_t;
typedef struct riak_object_t* riak_object;

RIAKC_API riak_object  riak_object_new(const char* bucket, const char *key, const char *value);
RIAKC_API void         riak_object_free(riak_object);
RIAKC_API const char*  riak_object_get_key(riak_object);
RIAKC_API const char*  riak_object_get_bucket(riak_object);
//RIAKC_API const vclock riak_object_get_vclock(riak_object);
RIAKC_API const riak_metadata riak_object_get_metadata(riak_object);



#ifdef __cplusplus
}
#endif


#endif // include guard
