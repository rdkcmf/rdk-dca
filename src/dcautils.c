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
#include "safec_lib.h"



#define EC_BUF_LEN 20

/**
 * @addtogroup DCA_APIS
 * @{
 */


/**
 * @brief This API is to find the load average of system and add it to the SearchResult JSON.
 *
 * @return  Returns status of operation.
 * @retval  Return 1 on success.
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

/**
 * @brief This function returns file size.
 *
 * @param[in] fp    File name
 *
 * @return  Returns size of file.
 */
static int fsize(FILE *fp) {
  int prev = ftell(fp);
  fseek(fp, 0L, SEEK_END);
  int sz = ftell(fp);
  if(prev >= 0)
  {
      if(fseek(fp, prev, SEEK_SET) != 0){
           LOG("Cannot set the file position indicator for the stream pointed to by stream\n");
      }
  }
  return sz;
}

/**
 * @brief This function is to clear/free the global paths.
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

/** 
 * @brief  To update current dca execution count
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

void saveExecCounter()
{
  FILE *ec_fp = NULL;
  ec_fp = fopen(EXEC_COUNTER_FILENAME, "w");
  if (NULL == ec_fp) {
    return;
  }

  fprintf(ec_fp, "%d" , CUR_EXEC_COUNT);
  fclose(ec_fp);
  return;
}

/**
 *  @brief This API is to verify whether to skip this telemetry marker.
 *
 *  @param[in] skipInterval Polling frequency
 *
 *  @return Retuns 0 or 1 depening upon the polling frequency.
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

/**
 *  @brief Function to return rotated log file.
 *
 *  @param[in] buf       Buffer
 *  @param[in] buflen    Maximum buffer length
 *  @param[in] name      Current Log file 
 *
 *  @return Returns Seek Log file. 
 */
char *getsRotatedLog(char *buf, int buflen, char *name)
{
  static FILE *LOG_FP = NULL;
  static int is_rotated_log = 0;
  char *rval = NULL;
  errno_t rc = -1;
  char *curLog = NULL;
  char * rotatedLog = NULL;
  size_t curLog_len = 0;
  size_t rotatedLog_len = 0;
  char *fileExtn = ".1";

  if ((NULL == PERSISTENT_PATH) || (NULL == LOG_PATH) || (NULL == name)) {
    LOG("Path variables are empty");
    return NULL;
  }

  curLog_len = strlen(LOG_PATH) + strlen(name) + 1;
  rotatedLog_len = strlen(LOG_PATH) + strlen(name) + strlen(fileExtn) + 1;
  curLog = malloc(curLog_len);
  rotatedLog = malloc(rotatedLog_len);
  if((curLog == NULL) || (rotatedLog == NULL))
     return NULL;

  if (NULL == LOG_FP)
  {
    long seek_value = 0;
    if (NULL != curLog)
    {
      rc = sprintf_s(curLog,curLog_len, "%s%s", LOG_PATH,name);
      if(rc < EOK)
      {
        ERR_CHK(rc);
        goto EXIT;
      }

      if (0 != readLogSeek(name, &seek_value))
      {
        LOG_FP = fopen(curLog, "rb");

        if (NULL == LOG_FP)
          goto EXIT;
      }
      else
      {
        long fileSize = 0;

        LOG_FP = fopen(curLog, "rb");

        if (NULL == LOG_FP)
        {
          LAST_SEEK_VALUE = 0;
          goto EXIT;
        }

        fileSize = fsize(LOG_FP);

        if (seek_value <= fileSize)
        {
             if(fseek(LOG_FP, seek_value, 0) != 0){
                 LOG("Cannot set the file position indicator for the stream pointed to by stream\n");
             }
        }
        else
        {
          int entry_flag = -1;
          int ind = -1;

          if(NULL != DEVICE_TYPE)
          {
            rc = strcmp_s("broadband", strlen("broadband"), DEVICE_TYPE , &ind);
            ERR_CHK(rc);
            if((!ind) && (rc == EOK))
            {
             entry_flag = 1;
            }
          }

          if (1 == entry_flag)
          {
            LOG("Telemetry file pointer corrupted");
            if(fseek(LOG_FP, 0, 0) != 0){
                 LOG("Cannot set the file position indicator for the stream pointed to by stream\n");
            }
          }
          else
          {
            if (NULL != rotatedLog) {
              rc = sprintf_s(rotatedLog,rotatedLog_len, "%s%s%s", LOG_PATH,name,fileExtn);
              if(rc < EOK)
              {
                ERR_CHK(rc);
                fclose(LOG_FP);
                LOG_FP = NULL;
                goto EXIT;
              }

              fclose(LOG_FP);
              LOG_FP = NULL;

              LOG_FP = fopen(rotatedLog, "rb");

              if (NULL == LOG_FP)
              {
                LOG("Error in opening file %s", rotatedLog);
                LAST_SEEK_VALUE = 0;
                goto EXIT;
              }


              if(fseek(LOG_FP, seek_value, 0) != 0){
                  LOG("Cannot set the file position indicator for the stream pointed to by stream\n");
              }
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
        is_rotated_log = 0;
        if (NULL != curLog)
        {
          rc = sprintf_s(curLog,curLog_len, "%s%s", LOG_PATH,name);
          if(rc < EOK)
          {
             ERR_CHK(rc);
             goto EXIT;
          }

          LOG_FP = fopen(curLog, "rb");

          if (NULL == LOG_FP)
          {
            LOG("Error in opening file %s", curLog);
            LAST_SEEK_VALUE = 0;
            goto EXIT;
          }

        }
        rval = fgets(buf, buflen, LOG_FP);
	if (NULL == rval)
	{
          seek_value = ftell(LOG_FP);
          LAST_SEEK_VALUE = seek_value;

          fclose(LOG_FP);
          LOG_FP = NULL;
	}

      }
    }
  }

  free(curLog);
  free(rotatedLog);
  return rval;

EXIT:
 free(curLog);
 free(rotatedLog);
 return NULL;
}

/**
 *  @brief Function to update the global paths like PERSISTENT_PATH,LOG_PATH from include.properties file.
 *
 *  @param[in] logpath   Log file path
 *  @param[in] perspath  Persistent path
 */
void updateIncludeConfVal(char *logpath, char *perspath)
{
  errno_t rc = -1;
  int p_path_len = -1, log_path_len = -1;
  FILE *file = fopen( INCLUDE_PROPERTIES, "r");
  if(NULL != file )
  {
    char props[255] = {""};
    p_path_len = strlen("PERSISTENT_PATH=");
    log_path_len = strlen("LOG_PATH=");
    while(fscanf(file,"%255s", props) != EOF )
    {
      char *property = NULL;
      if ((property = strstr( props, "PERSISTENT_PATH="))) {
        property = property + p_path_len;
        PERSISTENT_PATH = malloc(strlen(property) + 1);
        if (NULL != PERSISTENT_PATH) {
          rc = strcpy_s(PERSISTENT_PATH,strlen(property) + 1, property);
          if(rc != EOK)
          {
            ERR_CHK(rc);
            free(PERSISTENT_PATH);
            PERSISTENT_PATH = NULL;
            fclose(file);
            return;
          }
        }
      } else if ((property = strstr( props, "LOG_PATH="))) {
        if ( 0 == strncmp(props, "LOG_PATH=", log_path_len) ) {
          property = property + log_path_len;
          LOG_PATH = malloc(strlen(property) + 1);
          if (NULL != LOG_PATH) {
            rc = strcpy_s(LOG_PATH,strlen(property) + 1, property);
            if(rc != EOK)
             {
               ERR_CHK(rc);
               free(LOG_PATH);
               LOG_PATH = NULL;
               fclose(file);
               return;
             }

          }
        }
      }
    }
    fclose(file);
  }
  if ((NULL != logpath) && (logpath[0] != '\0'))
  {
    char *tmp = NULL;
    tmp = realloc(LOG_PATH, strlen(logpath) + 1 );
    if (NULL != tmp) {
      LOG_PATH = tmp;
      rc = strcpy_s(LOG_PATH,strlen(logpath) + 1, logpath);
      if(rc != EOK)
      {
        ERR_CHK(rc);
        free(LOG_PATH);
        LOG_PATH = NULL;
        return;
      }
    } else {
      free(LOG_PATH);
      LOG_PATH = NULL;
    }
  }
  if ((NULL != perspath) && (perspath[0] != '\0'))
  {
    char *tmp = NULL;
    tmp = realloc(PERSISTENT_PATH,strlen(perspath) + 1 );
    if (NULL != tmp) {
      PERSISTENT_PATH = tmp;
      rc = strcpy_s(PERSISTENT_PATH,strlen(perspath) + 1, perspath);
      if(rc != EOK)
      {
        ERR_CHK(rc);
        free(PERSISTENT_PATH);
        PERSISTENT_PATH = NULL;
        return;
      }
    } else {
      free(PERSISTENT_PATH);
      PERSISTENT_PATH = NULL;
    }
  }
}

/**
 *  @brief Function to update the configuration values from device.properties file.
 *
 *  @param[in] logpath   Log file path
 *  @param[in] perspath  Persistent path
 */
void updateConfVal(char *logpath, char *perspath)
{
  FILE *file = NULL;
  errno_t rc = -1;
  int ind = -1;
  int device_type_len = strlen("DEVICE_TYPE=");
  int length = 0 ;

  file = fopen( DEVICE_PROPERTIES, "r");
  if(NULL != file )
  {
    char props[255] = {""};
    while(fscanf(file,"%255s", props) != EOF )
    {
      char *property = NULL;
      length = 0;
      if((property = strstr( props, "DEVICE_TYPE=")))
      {
        property = property + device_type_len;
        length = strlen(property) + 1;
        DEVICE_TYPE = malloc(length);
        if (NULL != DEVICE_TYPE) {
          rc = strcpy_s(DEVICE_TYPE,length, property);
          if(rc != EOK)
          {
            ERR_CHK(rc);
            free(DEVICE_TYPE);
            DEVICE_TYPE = NULL;
            fclose(file);
            return;
          }
        }
        break;
      }
    }
    fclose(file);
  }
  
  updateIncludeConfVal(logpath, perspath);

  if (NULL != DEVICE_TYPE && NULL != PERSISTENT_PATH && NULL != LOG_PATH) {
    rc = strcmp_s("broadband", strlen("broadband"), DEVICE_TYPE , &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK)) {
      char *tmp_seek_file = "/.telemetry/tmp/rtl_";
      char *tmp_log_file = "/";
      char *tmp = NULL;

      if (NULL == perspath || perspath[0] == '\0') {
        length = strlen(PERSISTENT_PATH) + strlen(tmp_seek_file) + 1 ;
        tmp = realloc(PERSISTENT_PATH,length );
        if (NULL != tmp) {
          PERSISTENT_PATH = tmp;
          rc = strcat_s(PERSISTENT_PATH,length, tmp_seek_file);
          if(rc != EOK)
          {
            ERR_CHK(rc);
            free(PERSISTENT_PATH);
            PERSISTENT_PATH = NULL;
            return;
          }
        } else {
          free(PERSISTENT_PATH);
          PERSISTENT_PATH = NULL;
        }
      }

      if (NULL == logpath || logpath[0] == '\0') {
        length = strlen(LOG_PATH) + strlen(tmp_log_file) + 1 ;
        tmp = realloc(LOG_PATH, length );
        if (NULL != tmp) {
          LOG_PATH = tmp;
          rc = strcat_s(LOG_PATH,length, tmp_log_file);
          if(rc != EOK)
          {
            ERR_CHK(rc);  
            free(LOG_PATH);
            LOG_PATH = NULL; 
            return;
          }
        } else {
          free(LOG_PATH);
          LOG_PATH = NULL;
        }
      }
    } else {
      /* FIXME */
      char *tmp_seek_file = DEFAULT_SEEK_PREFIX;
      char *tmp_log_file = DEFAULT_LOG_PATH;
      char *tmp = NULL;

      //char *tmp_seek_file = "./tmp/rtl_";
      //char *tmp_log_file = "./logs/";
      if (NULL == perspath || perspath[0] == '\0') {
        length = strlen(tmp_seek_file) + 1;
        tmp = realloc(PERSISTENT_PATH, length );
        if (NULL != tmp) {
          PERSISTENT_PATH = tmp;
          rc = strcpy_s(PERSISTENT_PATH,length, tmp_seek_file);
          if(rc != EOK)
          {
            ERR_CHK(rc);  
            free(PERSISTENT_PATH);
            PERSISTENT_PATH = NULL; 
            return;
          }
        } else {
          free(PERSISTENT_PATH);
          PERSISTENT_PATH = NULL;
        }
      }

      if (NULL == logpath || logpath[0] == '\0') {
        length = strlen(tmp_log_file) + 1 ;
        tmp = realloc(LOG_PATH, length );
        if (NULL != tmp) {
          LOG_PATH = tmp;
          rc = strcpy_s(LOG_PATH,length, tmp_log_file);
          if(rc != EOK)
          {
            ERR_CHK(rc);  
            free(LOG_PATH);
            LOG_PATH = NULL; 
            return;
          }
        } else {
          free(LOG_PATH);
          LOG_PATH = NULL;
        }
      }
    }
  }

}

/**
 *  @brief Function to read the rotated Log file.
 *
 *  @param[in] name        Log file name.
 *  @param[in] seek_value  Position to seek.
 *
 *  @return Returns the status of the operation.
 *  @retval Returns -1 on failure, appropriate errorcode otherwise.
 */
int readLogSeek(char *name, long *seek_value)
{
  int rc = -1;
  errno_t ret = -1;
  int length = 0 ;
  if (NULL != name && NULL != PERSISTENT_PATH) {
    char *seekfile = NULL;
    length = strlen(PERSISTENT_PATH) + strlen(name) + 1;
    seekfile = malloc(length);
    if (NULL != seekfile) {
      FILE *fp = NULL;
	  ret = sprintf_s(seekfile,length, "%s%s", PERSISTENT_PATH,name);
	  if(ret < EOK)
	  {
		 ERR_CHK(ret);
		 free(seekfile);
		 return -1;
	  }

      if (NULL != (fp = fopen(seekfile, "r"))) {
          /*Coverity Fix CID:18152 CHECKED_RETURN */
          if( fscanf(fp, "%ld", seek_value) != 1) {
             LOG("Error in fscanf()\n");
          }
 
          fclose(fp);
          rc = 0;
      }
      free(seekfile);
    }
  }
  return rc;
}

/**
 *  @brief Function to write the rotated Log file.
 *
 *  @param[in] name        Log file name.
 *  @param[in] seek_value  Position to seek.
 *
 *  @return Returns the status of the operation.
 *  @retval Returns -1 on failure, appropriate errorcode otherwise.
 */
void writeLogSeek(char *name, long seek_value)
{
 errno_t rc = -1;
 int length = 0;
  if (NULL != name && NULL != PERSISTENT_PATH) {
    char *seekfile = NULL;
    length = strlen(PERSISTENT_PATH) + strlen(name) + 1;
    seekfile = malloc(length);
    if (NULL != seekfile) {
      FILE *fp = NULL;
	  rc = sprintf_s(seekfile,length, "%s%s", PERSISTENT_PATH,name);
	  if(rc< EOK)
	  {
		 ERR_CHK(rc);
		 free(seekfile);
		 return;
	  } 
      if (NULL != (fp = fopen(seekfile, "w"))) {
          fprintf(fp, "%ld", seek_value);
          fclose(fp);
      }
      free(seekfile);
    }	
  }
}

/** @} */  //END OF GROUP DCA_APIS

/** @} */


/** @} */
/** @} */
