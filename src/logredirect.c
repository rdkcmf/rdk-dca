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


#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <unistd.h>

#define SLEEP_USEC 100

static char *PERSISTENT_PATH = NULL;
static char *LOG_PATH = NULL;
static const char* INCLUDE_PROPERTIES = "/etc/include.properties";
static const char* DEVICE_PROPERTIES = "/etc/device.properties";

char temp[1024];


int Search_in_File(FILE *fp, FILE *ptr_file) {

	while (fgets(temp, 1024, fp) != NULL ) {
		fputs(temp, ptr_file);
		usleep(SLEEP_USEC);
	}
	return (0);
}

int fsize(FILE *fp) {
	int prev = ftell(fp);
	fseek(fp, 0L, SEEK_END);
	int sz = ftell(fp);
	fseek(fp, prev, SEEK_SET);
	return sz;
}

void
update_ConfVal(void)
{
    FILE *file = fopen( INCLUDE_PROPERTIES, "r");
    if(NULL != file )
    {
        char props[255] = {""};
        while(fscanf(file,"%s", props) != EOF )
        {
           char *property = NULL;
           if(property = strstr( props, "PERSISTENT_PATH=")) {
               property = property + strlen("PERSISTENT_PATH=");
               PERSISTENT_PATH = malloc(strlen(property) + 1);
               strcpy(PERSISTENT_PATH, property);
           } else if (property = strstr( props, "LOG_PATH=")) { 
               if ( 0 == strncmp(props, "LOG_PATH=", strlen("LOG_PATH=")) ) {
                   property = property + strlen("LOG_PATH=");
                   LOG_PATH = malloc(strlen(property) + 1);
                   strcpy(LOG_PATH, property);
               }
           }
        }
        fclose(file);
    }
}

int main(int argc, char *argv[]) {

	FILE * openLogFile;
	FILE * temp_file;

	FILE * rtl_seek_file;

	long size = 0;
	long fileSize = 0;

	char *name = argv[1];

	printf("Log File =  %s \n", name);

	char seekFile[256] = {0}; 
	char logFolder[256] = {0}; 
        char deviceType[25] = {0};

        FILE *file = fopen( DEVICE_PROPERTIES, "r");
        if(NULL != file )
        {
            char props[255] = {""};
            while(fscanf(file,"%s", props) != EOF )
            {
                char *property = NULL;
                if(property = strstr( props, "DEVICE_TYPE="))
                {
                    property = property + strlen("DEVICE_TYPE=");
                    strcpy(deviceType, property);
                    break;
                }
            }
            fclose(file);
        }


        if ( 0 == strcmp("broadband", deviceType) ) {
            update_ConfVal();
            strcpy(seekFile, PERSISTENT_PATH);
            strcat(seekFile,"/.telemetry/tmp/rtl_");
            strcpy(logFolder, LOG_PATH );
            strcat(logFolder,"/");
	    if (PERSISTENT_PATH) {
	    	free(PERSISTENT_PATH);
	    }
	    if (LOG_PATH) {
	    	free(LOG_PATH );
	    }

        } else { 
            // Let us not disturb the fielded RDK-V devices for time being
            strcpy(seekFile, "/opt/.telemetry/tmp/rtl_" );
            strcpy(logFolder, "/opt/logs/" );
        }
       
	char *logNameTemp = malloc(strlen(seekFile) + strlen(name) + 1);
	char *logName = malloc(strlen(logFolder) + strlen(name) + 1);

	memset(logNameTemp, '\0', strlen(seekFile) + strlen(name) + 1);

	strcpy(logNameTemp, seekFile);
	strcat(logNameTemp, name);


	memset(logName, '\0', strlen(logFolder) + strlen(name) + 1);

		strcpy(logName, logFolder);
		strcat(logName, name);



	printf("Log File =  %s \n", logNameTemp);

	rtl_seek_file = fopen(logNameTemp, "r");

	if (rtl_seek_file == NULL ) {

		fflush(stdout);

		rtl_seek_file = fopen(logNameTemp, "w");
		openLogFile = fopen(logName, "rb");
		temp_file = fopen("/tmp/.rtl_temp.log", "w+");

		if((openLogFile != NULL) && (temp_file != NULL)) {
			Search_in_File(openLogFile, temp_file);
			fseek(openLogFile, 0, SEEK_END);
			size = ftell(openLogFile);
			fclose(openLogFile);
                        fclose(temp_file);

		}
                if (rtl_seek_file != NULL) {
		    fprintf(rtl_seek_file, "%ld", size);
		    fclose(rtl_seek_file);
                }

	} else {

		long seek_value = 0;

		if (rtl_seek_file != NULL ) {
			fscanf(rtl_seek_file, "%ld", &seek_value);

			fclose(rtl_seek_file);
		}

		rtl_seek_file = fopen(logNameTemp, "w");

		openLogFile = fopen(logName, "rb");
		temp_file = fopen("/tmp/.rtl_temp.log", "w+");

		if((openLogFile != NULL) && (temp_file != NULL)) {

			fileSize = fsize(openLogFile);
//			printf("File Size =  %d \n",fileSize);
//			printf("Seek Size =  %d \n", seek_value);
			if (seek_value <= fileSize) {
				fseek(openLogFile, seek_value, 0);
				Search_in_File(openLogFile, temp_file);

				size = ftell(openLogFile);
				if(rtl_seek_file != NULL) {
				    fprintf(rtl_seek_file, "%ld", size);
				    close(rtl_seek_file);
				}

				fclose(openLogFile);

                                if(temp_file != NULL){
                                        fclose(temp_file);
                                }



			} else {
				//printf("Here");
				char *fileExtn = ".1";
				char * rotatedLogFile = malloc(strlen(logName) + strlen(fileExtn) + 1);
                                if ( rotatedLogFile == NULL ) {
                                    return 0 ;
                                }
				memset(rotatedLogFile, '\0', strlen(logName) + strlen(fileExtn) + 1);
				strcpy(rotatedLogFile, logName);
				strcat(rotatedLogFile, fileExtn);

				openLogFile = fopen(rotatedLogFile, "rb");
				//printf(rotatedLogFile);
				if((openLogFile != NULL) && (temp_file != NULL)) {
					fseek(openLogFile, seek_value, 0);
					Search_in_File(openLogFile, temp_file);
					fclose(openLogFile);
					//fclose(temp_file);

				} else {

					printf("File is empty");
					printf(rotatedLogFile);
				}

				openLogFile = fopen(logName, "rb");
				//temp_file = fopen(".rtl_temp.log", "a+");
				printf(logName);
				if((openLogFile != NULL) && (temp_file != NULL)) {
					Search_in_File(openLogFile, temp_file);
					fseek(openLogFile, 0, SEEK_END);
					size = ftell(openLogFile);
					fclose(openLogFile);
				}else {

					printf("File is empty 2 ");
					printf(logName);
				}

				if(rtl_seek_file != NULL){
					fprintf(rtl_seek_file, "%ld", size);
					fclose(rtl_seek_file);
				}

				if(temp_file != NULL){
					fclose(temp_file);
				}

				if (rotatedLogFile) {
					free(rotatedLogFile);
				}
			}
		}

	}

	if (logNameTemp) {
		free(logNameTemp);
	}
	if (logName) {
			free(logName);
		}

	return 0;
}



/** @} */


/** @} */
/** @} */
