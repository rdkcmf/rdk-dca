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

char temp[1024];


int Search_in_File(char *fname, char *str) {

	char temp[1024];
	int find_result = 0;

	FILE *fp;

	if ((fp = fopen(fname, "r")) == NULL ) {
		return (-1);
	}

	//printf("Log File =  %s \n", str);

	while (fgets(temp, 1024, fp) != NULL ) {
		if ((strstr(temp, str)) != NULL ) {
			find_result++;
		}
		usleep(SLEEP_USEC);

	}
	printf("%s=%d\n", str,find_result);
        fclose(fp);
	return 0;
}

int main(int argc, char *argv[]) {

	char *logFile = argv[1];
	char *logString = argv[2];

	if (logString != NULL ) {

		Search_in_File(logFile, logString);

	} else {

		printf("File is Empty\n");
	}

	return 0;
}



/** @} */


/** @} */
/** @} */
