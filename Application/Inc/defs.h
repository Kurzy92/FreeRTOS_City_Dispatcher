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
#include "stm32f7xx_hal_gpio.h"

// Tasks assigned resources
#define AMBULANCE_TASKS 8
#define POLICE_TASKS 5
#define FIRE_TASKS 5
#define CORONA_TASKS 5
#define MAX_TOTAL_CONCURRENT_TASKS 20

// Queue init configuration data
#define TASKS_QUEUE_SIZE 10
#define TASKS_MEMORY_SIZE	500

// Tasks priority defines
#define MANAGER_TASK_PRIORITY 		(configMAX_PRIORITIES - 15)
#define HANDLE_TASKS_PRIORITY 		MANAGER_TASK_PRIORITY + 1
#define DISPATCHER_TASK_PRIORITY	HANDLE_TASKS_PRIORITY + 1
#define GET_DATA_TASK_PRIORITY 		DISPATCHER_TASK_PRIORITY + 1
#define LOGGER_TASK_PRIORITY 		GET_DATA_TASK_PRIORITY + 1

// Calls message config data
#define MAX_NAME_LEN 12
#define MAX_MSG_LENGTH	100


//#define MIN_TIM2_IT_PERIOD	100 	// in milliseconds
//#define MAX_TIM2_IT_PERIOD	500 // in milliseconds

// TIM2 configuration defines
#define TIM2_PRESCALER_SET	539
#define TIM2_PERIOD_SET		3999

// Calls handling times
#define SHORTEST_TASK_DURATION_IN_TICKS		pdMS_TO_TICKS(1000)
#define LONGEST_TASK_DURATION_IN_TICKS		pdMS_TO_TICKS(2000)

// Data printing
#define USER_DATA_PRINT_DELAY_IN_TICKS		pdMS_TO_TICKS(5000)

// Get string from department enum
#define GET_ENUM_DEPARTMENT_STR(x)	( \
						(x)==AMBULANCE ? "Ambulance": \
						(x)==POLICE? "Police": \
						(x)==FIRE? "Fire Dep" : \
						(x)==CORONA? "Corona"  : \
						"unknown dep" \
						)

// Logger Defines
#define LOGGER_UART			1
#define LOGGER_QUEUE_SIZE	25
#define LOGGER_HUART		huart3


typedef enum  {
	AMBULANCE = 0,
	POLICE,
	FIRE,
	CORONA
} DepartmentsEnum;

//typedef struct _Department {
//	DepartmentsEnum name;
//	uint8_t maxResources;
//	uint8_t currentUsedResources;
//} Department_t;

typedef struct _DispatcherPacket {
	DepartmentsEnum department;
	char message[MAX_MSG_LENGTH];
	uint16_t timeToHandleInTicks;
	uint8_t* available_tasks_counter;
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
