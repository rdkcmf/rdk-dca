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


#include <iostream>
#include <stdio.h>
#include <string.h>


#define LEN 14
#define BUF_LEN 16
#define CMD_LEN 256

using namespace std;


typedef struct _procMemCpuInfo {
    char processName[BUF_LEN];
    char cpuUse[BUF_LEN];
    char memUse[BUF_LEN];
} procMemCpuInfo;


bool getProcInfo(procMemCpuInfo *pInfo);

int main(int argc,char *argv[]) {
    FILE *in = NULL;
    char *processName = NULL;
    FILE *fp = NULL ;
    procMemCpuInfo pInfo;

    memset(&pInfo, '\0', sizeof(procMemCpuInfo));
    
    if(argv[1] == NULL)
    {
       return 0;
    }
	
    processName = argv[1];
 
    if (processName != NULL) {  
        memcpy(pInfo.processName, processName, strlen(processName)+1); 
    }
   
    if(false == getProcInfo(&pInfo))
    {
        cout << "{\"Failed\":\"0\"}";
	    return 0;
    }
	
    cout << "{\"mem_" << processName << "\":\"" << pInfo.memUse << "\"},";
    cout << "{\"cpu_" << processName << "\":\"" << pInfo.cpuUse << "\"}";

    return 1;
}


bool getProcInfo(procMemCpuInfo *pInfo)
{
    bool ret = false;
    FILE *inFp = NULL;
    char command[CMD_LEN] = {'\0'};
    char *pcpu = NULL, *pmem = NULL;
    char var1[BUF_LEN] = {'\0'};
    char var2[BUF_LEN] = {'\0'};
    char var3[BUF_LEN] = {'\0'};           
    char var4[BUF_LEN] = {'\0'};
    char var5[BUF_LEN] = {'\0'};
    char var6[BUF_LEN] = {'\0'};           
    char var7[BUF_LEN] = {'\0'};           
    char var8[512]= {'\0'};
    char var9[512]= {'\0'}; 
    char var10[512]= {'\0'};          

    if (pInfo == NULL) {

        return false;    
    }

#ifdef INTEL
    /* Format Use:  `top n 1 | grep Receiver` */

    sprintf(command, "top -n 1 | grep -i '%s'", pInfo->processName);
#else 
     /* ps -C Receiver -o %cpu -o %mem */
    //sprintf(command, "ps -C '%s' -o %%cpu -o %%mem | sed 1d", pInfo->processName);
    sprintf(command, "top -b -n 1 | grep -i '%s'", pInfo->processName);

#endif

   
    if(!(inFp = popen(command, "r"))){
        return false;
    }


 //  2268 root      20   0  831m  66m  20m S   27 13.1 491:06.82 Receiver
#ifdef INTEL

    if (fscanf(inFp,"%s %s %s %s %s %s %s %s", var1, var2, var3, var4, var5, var6, var7, var8) == 8) {                           
        pmem = var5;
        pcpu = var7;
        
	}
//#endif

#else

//while(fscanf(inFp,"%s %s", var1, var2) == 2)
//    {
//	    pcpu = var1;
//	    pmem = var2;
 //   }
 
  if (fscanf(inFp,"%s %s %s %s %s %s %s %s %s %s", var1, var2, var3, var4, var5, var6, var7, var8, var9, var10) == 10) {                           
        pmem = var6;
        pcpu = var9;
        
	}


#endif         

    if ((pcpu != NULL) && (pmem != NULL)) {
        strncpy(pInfo->cpuUse, pcpu, strlen(pcpu)+1);
        strncpy(pInfo->memUse, pmem, strlen(pmem)+1);
        ret = true;
    }
    
    pclose(inFp);
    return ret;

}


/** @} */


/** @} */
/** @} */
