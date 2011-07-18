#ifndef RIAKC_CLIENT_H__
#define RIAKC_CLIENT_H__

#include <riak_client/common.h>
#include <riak_client/riak_object.h>
#include <riak_client/riak_metadata.h>
#include <riak_client/string_list.h>
#include <riak_client/string_map.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct riak_client_t;
typedef struct riak_client_t* riak_client;

struct riak_content_t;
typedef struct riak_content_t* riak_content;

typedef struct riak_data_t 
{
    char  *data;
    size_t len;
} riak_data;

typedef struct vclock_t {
    size_t len;
    char data[0];
} vclock_t;

typedef vclock_t* vclock;
 
RIAKC_API riak_client  riak_client_new(const char* host,const char *port);
RIAKC_API void         riak_client_free(riak_client);
RIAKC_API int          riak_ping(riak_client);
RIAKC_API int          riak_set_client_id(riak_client, uint32_t id);
RIAKC_API uint32_t     riak_get_client_id(riak_client);
RIAKC_API string_list  riak_list_buckets(riak_client);
RIAKC_API string_list  riak_list_keys(riak_client, const char* bucket);
RIAKC_API riak_object  riak_get(riak_client, const char* bucket, 
                                const char *key);
RIAKC_API int          riak_put(riak_client, riak_object);
RIAKC_API riak_object  riak_put_returnbody(riak_client, riak_object);


RIAKC_API void          vclock_free(vclock);

#ifdef __cplusplus
}
#endif

#endif // include guard
