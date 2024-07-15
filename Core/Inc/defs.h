/*
 * defs.h
 *
 *  Created on: Jul 8, 2024
 *      Author: eyalk
 */

#ifndef INC_DEFS_H_
#define INC_DEFS_H_

#include "stdint.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "stdbool.h"

#define AMBULANCE_TASKS 4
#define POLICE_TASKS 3
#define FIRE_TASKS 2
#define CORONA_TASKS 4
#define MAX_TOTAL_CONCURRENT_TASKS 10
#define TASKS_QUEUE_SIZE 10
#define TASKS_MEMORY_SIZE	500

#define HANDLE_TASKS_PRIORITY (configMAX_PRIORITIES - 10)
#define MANAGER_TASK_PRIORITY HANDLE_TASKS_PRIORITY-3
#define DISPATCHER_TASK_PRIORITY	MANAGER_TASK_PRIORITY + 1

#define MAX_NAME_LEN 12
#define MAX_MSG_LENGTH	100

#define MIN_TIM2_IT_PERIOD	100 	// in milliseconds
#define MAX_TIM2_IT_PERIOD	500 // in milliseconds

#define TIM2_PRESCALER_SET	539
#define TIM2_PERIOD_SET		3999

#define GET_ENUM_DEPARTMENT_STR(x)	( \
						(x)==AMBULANCE ? "Ambulance": \
						(x)==POLICE? "Police": \
						(x)==FIRE? "Fire Dep" : \
						(x)==CORONA? "Corona"  : \
						"unknown dep" \
						)

typedef enum  {
	AMBULANCE = 0,
	POLICE,
	FIRE,
	CORONA
} DepartmentsEnum;

typedef struct _Department {
	DepartmentsEnum name;
	uint8_t maxResources;
	uint8_t currentUsedResources;
	/* Create a semaphore for this type? */
} Department_t;

typedef struct _DispatcherPacket {
	DepartmentsEnum department;
	char message[MAX_MSG_LENGTH];
	uint16_t timeToHandleInTicks;
} DispatcherPacket;

typedef struct _taskInit_t {
	DepartmentsEnum department;
	uint8_t taskIdentifier;
	QueueHandle_t* pQhandler;
	SemaphoreHandle_t* pSemHandler;
	bool* bTaskStatusArr;
	uint8_t numOfTasks;
} taskInit_t;

#endif /* INC_DEFS_H_ */
