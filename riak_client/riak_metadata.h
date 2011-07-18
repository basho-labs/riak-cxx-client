#ifndef RIAKC_METADATA_H_
#define RIAKC_METADATA_H_

#include <riak_client/common.h>

#ifdef __cplusplus
extern "C" {
#endif

struct riak_metadata_t;
typedef struct riak_metadata_t* riak_metadata;

RIAKC_API riak_metadata riak_metadata_new();
RIAKC_API void          riak_metadata_free(riak_metadata);
RIAKC_API const char*   riak_metadata_get(riak_metadata, const char *key);
RIAKC_API void          riak_metadata_set(riak_metadata, const char *key, 
                                          const char *value);
RIAKC_API void          riak_metadata_del(riak_metadata, const char *key);

#ifdef __cplusplus
}
#endif

#endif // include guard
