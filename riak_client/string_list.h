#ifndef RIAKC_STRING_LIST_H_
#define RIAKC_STRING_LIST_H_

#include <riak_client/common.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct string_list_t;
typedef struct string_list_t* string_list;

RIAKC_API string_list  string_list_new();
RIAKC_API void         string_list_free(string_list);
RIAKC_API size_t       string_list_size(string_list);
RIAKC_API const char*  string_list_get(string_list, size_t idx);

#ifdef __cplusplus
}
#endif


#endif // include guard
