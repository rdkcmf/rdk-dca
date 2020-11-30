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

#include "dcautils.h"
#include "dcalist.h"
#include "safec_lib.h"

/*
 * @addtogroup DCA_APIS
 * @{
 */

/**
 * @brief To insert a new pattern node at the beginning of the list.
 *
 * @param[in] pch         Node head.
 * @param[in] pattern     Search pattern.
 * @param[in] header      Header.
 * @param[in] dtype       Data type.
 * @param[in] count       Pattern count.
 * @param[in] data        Data.
 *
 * @return  Returns the value of rc.
 */
int insertPCNode(GList **pch, char *pattern, char *header, DType_t dtype, int count, char *data)
{
  pcdata_t *new = NULL;
  int rc = -1;
  
  new = (pcdata_t *) malloc(sizeof(*new));
  if (NULL != new) {
    if (pattern != NULL) {
      new->pattern = strdup(pattern);
      if (!(new->pattern)) {
          free(new->pattern);
          free(new);
          return rc;
      }
    } else {
      new->pattern = NULL;
    }
    if (header != NULL) {
      new->header = strdup(header);
      if (!(new->header)) {
          free(new->header);
          free(new);
          return rc;

      }
    } else {
      new->header = NULL;
    }

    new->d_type = dtype;
    if (dtype == INT) {
      new->count = count;
    } else if (dtype == STR) {
      if (NULL != data) {
        new->data = strdup(data);
        if (!(new->data)) {
            free(new->data);
            free(new);
            return rc;
        }
      } else {
        new->data = NULL;
      }
    }

    *pch = g_list_append(*pch , new);
    rc = 0;
  }
  return rc;
}

/**
 * @brief To do custom comparison.
 *
 * @param[in] np   Node pattern.
 * @param[in] sp   Search pattern.
 *
 * @return  Returns status of the operation.
 * @retval  Returns 0 on success and NULL on failure.
 */
gint comparePattern(gconstpointer np, gconstpointer sp)
{
  pcdata_t *tmp = (pcdata_t *)np;
  if (tmp && tmp->pattern && NULL != strstr(sp, tmp->pattern)) {
        return 0;
    }
  return -1;
}

/**
 * @brief To search node from the list based on the given pattern.
 *
 * @param[in]  pch       Node head.
 * @param[in]  pattern   Pattern to search.
 *
 * @return  Returns node on success and NULL on failure.
 */
pcdata_t* searchPCNode(GList *pch, char *pattern)
{
  GList *fnode = NULL;
  fnode = g_list_find_custom(pch, pattern, (GCompareFunc)comparePattern);
  if (NULL != fnode)
    return fnode->data;
  else
    return NULL;
}

/**
 * @brief Debug function to print the node.
 *
 * @param[in] data       node data
 * @param[in] user_data  user data
 */
void print_pc_node(gpointer data, gpointer user_data)
{
  UNREFERENCED_PARAMETER(user_data);
  pcdata_t *node = (pcdata_t *)data;
  if (node) {
    LOG("node pattern:%s, header:%s", node->pattern, node->header);
    if (node->d_type == INT) {
      LOG("\tcount:%d", node->count);
    } else if (node->d_type == STR) {
      LOG("\tdata:%s", node->data);
    }
  }
}

/**
 * @brief Debug function to print all nodes in the list.
 *
 * @param[in] pch  node head
 */
void printPCNodes(GList *pch)
{
  g_list_foreach(pch, (GFunc)print_pc_node, NULL);
}

/**
 * @brief To delete a node.
 *
 * @param[in] node    Node head.
 */
 void freePCNode(gpointer node)
{
  pcdata_t *tmp = (pcdata_t *)(node);
  if (NULL != tmp)
  {
    if (NULL != tmp->pattern) {
      free(tmp->pattern);
      tmp->pattern = NULL;
    }
    if (NULL != tmp->header) {
      free(tmp->header);
      tmp->header = NULL;
    }
    if (tmp->d_type == STR) {
      if (NULL != tmp->data) {
        free(tmp->data);
        tmp->data = NULL;
      }
    }
    free(tmp);
  }
}

/**
 * @brief To delete/clear all the nodes in the list.
 *
 * @param[in]  pch   Node head.
 */
void clearPCNodes(GList **pch)
{
  g_list_free_full(*pch, &freePCNode);
}

/** @} */  //END OF GROUP DCA_APIS

/** @} */


/** @} */
/** @} */

  
