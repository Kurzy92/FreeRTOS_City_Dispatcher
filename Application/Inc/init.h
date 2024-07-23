/*
 * init.h
 *
 *  Created on: Jul 23, 2024
 *      Author: eyalk
 */

#ifndef INC_INIT_H_
#define INC_INIT_H_

#include "main.h"
#include "data.h"


void initQueues(void);

void initTasks(void);
int8_t initAmbTasks(void);
int8_t initPolTasks(void);
int8_t initFireTasks(void);
int8_t initCoronaTasks(void);


void initSemaphores(void);

#endif /* INC_INIT_H_ */
