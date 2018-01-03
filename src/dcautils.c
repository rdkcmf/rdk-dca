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

#define EC_BUF_LEN 20

/** @description: To get load average of system
 *  @param loadAvg
 *  @return 1 on success.
 */
int getLoadAvg()
{
  FILE *fp;
  char str[LEN+1];

  if (NULL == (fp = fopen("/proc/loadavg", "r"))) {
    LOG("Error in opening /proc/loadavg file");
    return 0;
  }
  if (fread(str, 1, LEN, fp) != LEN) {
    LOG("Error in reading loadavg");
    fclose(fp);
    return 0;
  }
  fclose(fp);
  str[LEN] = '\0';

  addToSearchResult("Load_Average", str);
  return 1;
}

/** @description: To get file size
 *  @param file
 *  @return size
 */
static int fsize(FILE *fp) {
  int prev = ftell(fp);
  fseek(fp, 0L, SEEK_END);
  int sz = ftell(fp);
  fseek(fp, prev, SEEK_SET);
  return sz;
}

/** @description: To clear/free the global paths
 *  @param none
 *  @return none
 */
void clearConfVal(void)
{
  if (PERSISTENT_PATH)
    free(PERSISTENT_PATH);

  if (LOG_PATH)
    free(LOG_PATH);

  if (DEVICE_TYPE)
    free(DEVICE_TYPE);
}

/** @description: To update current execution count
 *  @param none
 *  @return none
 */
void updateExecCounter()
{
  FILE *ec_fp = NULL;
  char buf[EC_BUF_LEN] = {0};
  char *rval = NULL;

  ec_fp = fopen(EXEC_COUNTER_FILENAME, "r");
  if (NULL == ec_fp) {
    CUR_EXEC_COUNT = 0; 
    return;
  }

  rval = fgets(buf, EC_BUF_LEN, ec_fp);
  fclose(ec_fp);
  if (NULL == rval) {
    CUR_EXEC_COUNT = 0; 
    return;
  }

  CUR_EXEC_COUNT = atoi(buf); 
  CUR_EXEC_COUNT++;
  if (CUR_EXEC_COUNT < 0)
    CUR_EXEC_COUNT = 0;

  return;
}

/** @description: To verify whether to skip current execution or not for pattern check
 *  @param skipInterval
 *  @return none
 */
int isSkipParam(int skipInterval)
{
  int rval = 1;
  if (skipInterval == 0 || CUR_EXEC_COUNT == 0) {
    rval = 0;
  } else {
    skipInterval++;
    if (CUR_EXEC_COUNT < skipInterval) {
      rval = 1;
    } else {
      if ((CUR_EXEC_COUNT % skipInterval) == 0) {
        rval = 0;
      }
    }
  }
  return rval;
}


/** @description: To read rotated log file
 *  @param none
 *  @return none
 */
char *getsRotatedLog(char *buf, int buflen, char *name)
{
  static FILE *LOG_FP = NULL;
  static int is_rotated_log = 0;
  char *rval = NULL;

  if ((NULL == PERSISTENT_PATH) || (NULL == LOG_PATH) || (NULL == name)) {
    return NULL;
  }

  if (NULL == LOG_FP)
  {
    char *curLog = NULL;
    long seek_value = 0;

    curLog = malloc(strlen(LOG_PATH) + strlen(name) + 1);
    if (NULL != curLog)
    {
      strcpy(curLog, LOG_PATH);
      strcat(curLog, name);
      if (0 != readLogSeek(name, &seek_value))
      {
        LOG_FP = fopen(curLog, "rb");
        free(curLog);
        curLog = NULL;

        if (NULL == LOG_FP)
          return NULL;
      }
      else
      {
        long fileSize = 0;

        LOG_FP = fopen(curLog, "rb");
        free(curLog);
        curLog = NULL;

        if (NULL == LOG_FP)
          return NULL;

        fileSize = fsize(LOG_FP);

        if (seek_value <= fileSize)
        {
          fseek(LOG_FP, seek_value, 0);
        }
        else
        {
          if (NULL != DEVICE_TYPE)
          {
            if (0 == strcmp("broadband", DEVICE_TYPE)) {
              fseek(LOG_FP, 0, 0);
            }
          }
          else
          {
            char *fileExtn = ".1";
            char * rotatedLog = malloc(strlen(LOG_PATH) + strlen(name) + strlen(fileExtn) + 1);
            if (NULL != rotatedLog) {
              strcpy(rotatedLog, LOG_PATH);
              strcat(rotatedLog, name);
              strcat(rotatedLog, fileExtn);

              fclose(LOG_FP);
              LOG_FP = NULL;

              LOG_FP = fopen(rotatedLog, "rb");

              free(rotatedLog);
              rotatedLog = NULL;

              if (NULL == LOG_FP)
                return NULL;

              fseek(LOG_FP, seek_value, 0);
              is_rotated_log = 1;
            }
          }
        }
      }
    }
  }

  if (NULL != LOG_FP)
  {
    rval = fgets(buf, buflen, LOG_FP);
    if (NULL == rval)
    {
      long seek_value = ftell(LOG_FP);
      LAST_SEEK_VALUE = seek_value;

      fclose(LOG_FP);
      LOG_FP = NULL;

      if (is_rotated_log == 1)
      {
        char *curLog = NULL;
        is_rotated_log = 0;
        curLog = malloc(strlen(LOG_PATH) + strlen(name) + 1);
        if (NULL != curLog)
        {
          strcpy(curLog, LOG_PATH);
          strcat(curLog, name);
          LOG_FP = fopen(curLog, "rb");

          free(curLog);
          curLog = NULL;

          if (NULL == LOG_FP)
            return NULL;
        }
        rval = fgets(buf, buflen, LOG_FP);
      }
    }
  }

  return rval;
}

/** @description: To update the values from the include.properties
 *  @param none
 *  @return none
 */
void updateIncludeConfVal(void)
{
  FILE *file = fopen( INCLUDE_PROPERTIES, "r");
  if(NULL != file )
  {
    char props[255] = {""};
    while(fscanf(file,"%s", props) != EOF )
    {
      char *property = NULL;
      if (property = strstr( props, "PERSISTENT_PATH=")) {
        property = property + strlen("PERSISTENT_PATH=");
        PERSISTENT_PATH = malloc(strlen(property) + 1);
        if (NULL != PERSISTENT_PATH) {
          strcpy(PERSISTENT_PATH, property);
        }
      } else if (property = strstr( props, "LOG_PATH=")) {
        if ( 0 == strncmp(props, "LOG_PATH=", strlen("LOG_PATH=")) ) {
          property = property + strlen("LOG_PATH=");
          LOG_PATH = malloc(strlen(property) + 1);
          if (NULL != LOG_PATH) {
            strcpy(LOG_PATH, property);
          }
        }
      }
    }
    fclose(file);
  }
}

/** @description: To update the configuration values from the device.properties
 *  @param none
 *  @return none
 */
void updateConfVal(void)
{
  FILE *file = NULL;

  file = fopen( DEVICE_PROPERTIES, "r");
  if(NULL != file )
  {
    char props[255] = {""};
    while(fscanf(file,"%s", props) != EOF )
    {
      char *property = NULL;
      if(property = strstr( props, "DEVICE_TYPE="))
      {
        property = property + strlen("DEVICE_TYPE=");
        DEVICE_TYPE = malloc(strlen(property) + 1);
        if (NULL != DEVICE_TYPE) {
          strcpy(DEVICE_TYPE, property);
        }
        break;
      }
    }
    fclose(file);
  }

  updateIncludeConfVal();

  if (NULL != DEVICE_TYPE && NULL != PERSISTENT_PATH && NULL != LOG_PATH) {
    if ( 0 == strcmp("broadband", DEVICE_TYPE) ) {
      char *tmp_seek_file = "/.telemetry/tmp/rtl_";
      char *tmp_log_file = "/";
      char *tmp = NULL;


      tmp = realloc(PERSISTENT_PATH, strlen(PERSISTENT_PATH) + strlen(tmp_seek_file) + 1 );
      if (NULL != tmp) {
        PERSISTENT_PATH = tmp;
        strcat(PERSISTENT_PATH, tmp_seek_file);
      } else {
        free(PERSISTENT_PATH);
        PERSISTENT_PATH = NULL;
      }

      tmp = realloc(LOG_PATH, strlen(LOG_PATH) + strlen(tmp_log_file) + 1 );
      if (NULL != tmp) {
        LOG_PATH = tmp;
        strcat(LOG_PATH, tmp_log_file);
      } else {
        free(LOG_PATH);
        LOG_PATH = NULL;
      }
    } else {
      /* FIXME */
      char *tmp_seek_file = DEFAULT_SEEK_PREFIX;
      char *tmp_log_file = DEFAULT_LOG_PATH;
      char *tmp = NULL;
      //char *tmp_seek_file = "./tmp/rtl_";
      //char *tmp_log_file = "./logs/";
      tmp = realloc(PERSISTENT_PATH, strlen(tmp_seek_file) + 1 );
      if (NULL != tmp) {
        PERSISTENT_PATH = tmp;
        strcpy(PERSISTENT_PATH, tmp_seek_file);
      } else {
        free(PERSISTENT_PATH);
        PERSISTENT_PATH = NULL;
      }

      tmp = realloc(LOG_PATH, strlen(tmp_log_file) + 1 );
      if (NULL != tmp) {
        LOG_PATH = tmp;
        strcpy(LOG_PATH, tmp_log_file);
      } else {
        free(LOG_PATH);
        LOG_PATH = NULL;
      }
    }
  }

}

int readLogSeek(char *name, long *seek_value)
{
  int rc = -1;
  if (NULL != name && NULL != PERSISTENT_PATH) {
    char *seekfile = NULL;
    seekfile = malloc(strlen(PERSISTENT_PATH) + strlen(name) + 1);
    if (NULL != seekfile) {
      FILE *fp = NULL;
      strcpy(seekfile, PERSISTENT_PATH);
      strcat(seekfile, name);
      if (NULL != (fp = fopen(seekfile, "r"))) {
          fscanf(fp, "%ld", seek_value);
          fclose(fp);
          rc = 0;
      }
      free(seekfile);
    }
  }
  return rc;
}

void writeLogSeek(char *name, long seek_value)
{
  if (NULL != name && NULL != PERSISTENT_PATH) {
    char *seekfile = NULL;
    seekfile = malloc(strlen(PERSISTENT_PATH) + strlen(name) + 1);
    if (NULL != seekfile) {
      FILE *fp = NULL;
      strcpy(seekfile, PERSISTENT_PATH);
      strcat(seekfile, name);
      if (NULL != (fp = fopen(seekfile, "w"))) {
          fprintf(fp, "%ld", seek_value);
          fclose(fp);
      }
      free(seekfile);
    }
  }
}

/** @} */


/** @} */
/** @} */
