/*
 * task_handling.h
 *
 *  Created on: Jul 15, 2024
 *      Author: eyalk
 */

#ifndef INC_TASK_HANDLING_H_
#define INC_TASK_HANDLING_H_

#include "main.h"
#include "queue.h"
#include "semphr.h"
#include "string.h"

#define TASK_MANAGER_TICKS_TO_WAIT  pdMS_TO_TICKS(40)

void vHandleCall(void* pvParameters);
void tasksManagerTask(void);


#endif /* INC_TASK_HANDLING_H_ */
