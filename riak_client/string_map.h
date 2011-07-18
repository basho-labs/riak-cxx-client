#ifndef RIAKC_STRING_MAP_H_
#define RIAKC_STRING_MAP_H_

#include <riak_client/common.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct string_map_t;
typedef struct string_map_t* string_map;

RIAKC_API string_map   string_map_new();
RIAKC_API void         string_map_free(string_map);
RIAKC_API const char * string_map_get(string_map, const char *key);
RIAKC_API void         string_map_set(string_map, const char *key, 
                                      const char *value);
RIAKC_API void         string_map_del(string_map, const char *key);

#ifdef __cplusplus
}
#endif


#endif // include guard
