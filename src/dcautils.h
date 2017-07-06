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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cJSON.h"

#define MAXLINE 1024
#define MAXLEN 512
#define LEN 14

#define USLEEP_SEC 100
#define MAX_PROCESS 10
#define RDK_EC_MAXLEN 5 /* RDK Error code maximum length */

#define RTL_FILENAME "/.rtl_temp.log"

#define INCLUDE_PROPERTIES "/etc/include.properties"
#define DEVICE_PROPERTIES "/etc/device.properties"
#define DELIMITER "<#=#>"

#define DEFAULT_SEEK_PREFIX "/opt/.telemetry/tmp/rtl_"
#define DEFAULT_LOG_PATH "/opt/logs/"


extern char *PERSISTENT_PATH;
extern char *LOG_PATH;
extern char *DEVICE_TYPE;

extern char *RTL_TEMP_LOG_FILE;
extern char *OUTPUT_FILE;

extern cJSON *SEARCH_RESULT_JSON;
extern cJSON *ROOT_JSON;

//#define LOG(fmt,__etc...) fprintf(stderr, "%s(%s:%d) "fmt"\n", __FUNCTION__, __FILE__, (int)__LINE__, ##__etc);fflush(stdout);
#define LOG(fmt,__etc...) fprintf(stderr, fmt"\n", ##__etc);fflush(stdout);

/* utility functions */
int getLoadAvg(void);
char *getsRotatedLog(char *buf, int buflen, char *name);
void clearConfVal(void);
void updateIncludeConfVal(void);
void updateConfVal(void);
int readLogSeek(char *name, long *seek_value);
void writeLogSeek(char *name, long seek_value);
int checkLogSeek(char *name, long prevSeekVal);

/* JSON functions */
void initSearchResultJson(cJSON **root, cJSON **sr);
void addToSearchResult(char *key, char *value);
void clearSearchResultJson(cJSON **root);
int printJson(cJSON *root);

int getProcUsage(char *processName);

/** @} */


/** @} */
/** @} */
/** @} */


/** @} */
/** @} */
