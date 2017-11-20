
#include "dcautils.h"

void initSearchResultJson(cJSON **root, cJSON **sr)
{
  *root = cJSON_CreateObject();
  if (NULL != *root) {
    cJSON_AddItemToObject(*root, "searchResult", *sr = cJSON_CreateArray());
  }
}

void addToSearchResult(char *key, char *value)
{
  if (NULL != SEARCH_RESULT_JSON) {
    cJSON *obj = cJSON_CreateObject();
    if (NULL != obj) {
      cJSON_AddStringToObject(obj, key, value);
      cJSON_AddItemToArray(SEARCH_RESULT_JSON, obj);
    }
  }
}

void clearSearchResultJson(cJSON **root)
{
    cJSON_Delete(*root);
}

int printJson(cJSON *root)
{
    if (NULL != root) {
      char *out = cJSON_PrintUnformatted(root);
      if (NULL != out) {
        printf("%s\n", out);
        free(out);
      }
    }
}
