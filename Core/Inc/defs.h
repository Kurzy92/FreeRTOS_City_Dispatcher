/*
 * defs.h
 *
 *  Created on: Jul 8, 2024
 *      Author: eyalk
 */

#ifndef INC_DEFS_H_
#define INC_DEFS_H_

#include "stdint.h"

/*
 * Department Allocated Tasks Defines
 */
#define AMBULANCE_TASKS 4
#define POLICE_TASKS 3
#define FIRE_TASKS 2
#define CORONA_TASKS 4


#define MAX_TOTAL_CONCURRENT_TASKS 10

/*
 * Queue Init Defines
 */
#define TASKS_QUEUE_SIZE 10
#define TASKS_MEMORY_SIZE	configMINIMAL_STACK_SIZE
#define HANDLE_TASKS_PRIORITY (configMAX_PRIORITIES - 10)


/*
 * Dispatcher Messages String Defines
 */
#define MAX_NAME_LEN 12
#define MAX_MSG_LENGTH	100


#define MIN_TIM2_IT_PERIOD	100 	// in milliseconds
#define MAX_TIM2_IT_PERIOD	500 // in milliseconds


/*
 * TIM2 Defines
 */
#define TIM2_PRESCALER_SET	539
#define TIM2_PERIOD_SET		3999


/*
 * Display Defines
 */
#define LCD_H_RES       240
#define LCD_V_RES       320
#define BUS_SPI1_POLL_TIMEOUT 0x1000U


enum DepartmentsEnum {
	AMBULANCE = 0,
	POLICE,
	FIRE,
	CORONA
};

typedef struct _Department {
	enum DepartmentsEnum name;
	uint8_t maxResources;
	uint8_t currentUsedResources;
	/* Create a semaphore for this type? */
} Department_t;

typedef struct _DispatcherPacket {
	enum DepartmentsEnum department;
	char message[MAX_MSG_LENGTH];
	uint16_t timeToHandleInTicks;
} DispatcherPacket;


#endif /* INC_DEFS_H_ */
