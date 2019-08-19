/*
 * dcaInternal.h
 *
 *  Created on: Aug 11, 2019
 *      Author: skv
 */

#ifndef SRC_DCAINTERNAL_H_
#define SRC_DCAINTERNAL_H_

#include <stdlib.h>

typedef enum
{
    MTYPE_NONE,
    MTYPE_COUNTER,
    MTYPE_ABSOLUTE
}MarkerType;

typedef struct _GrepMarker
{
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
