/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2019 RDK Management
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

#ifndef SRC_DCAINTERNAL_H_
#define SRC_DCAINTERNAL_H_

#include <stdlib.h>
#include <stdbool.h>

typedef enum
{
    MTYPE_NONE,
    MTYPE_COUNTER,
    MTYPE_ABSOLUTE
}MarkerType;

typedef struct _GrepMarker
{
    bool reportEmptyParam;
    char* paramType;
    char* markerName;
    char* searchString;
    char* logFile;
    MarkerType mType;
    union{
        unsigned int count;
        char* markerValue;
    }u;
    unsigned int skipFreq;
}GrepMarker;


int displayLogGrepResults(int argc, char *argv[]);

#endif /* SRC_DCAINTERNAL_H_ */
