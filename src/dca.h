/*
 * dca.h
 *
 *  Created on: Aug 11, 2019
 *      Author: skv
 */

#ifndef SRC_DCA_H_
#define SRC_DCA_H_
#include <cjson/cJSON.h>

/**
 * Caller should be freeing vectorMarkerList and grepResultList
 */
int getDCAResults(void* vectorMarkerList, cJSON** grepResultList);

#endif /* SRC_DCA_H_ */
