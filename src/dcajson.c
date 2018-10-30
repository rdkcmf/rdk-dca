/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2016 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */



/**
 * @defgroup dca
 * @{
 **/




/**
 * @defgroup dca
 * @{
 * @defgroup src
 * @{
 **/

// To provide telemetry data in a JSON format

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

/** @} */


/** @} */
/** @} */
