/*
 * defs.h
 *
 *  Created on: Jul 8, 2024
 *      Author: eyalk
 */

#ifndef INC_DEFS_H_
#define INC_DEFS_H_

#define AMBULANCE_TASKS 4
#define POLICE_TASKS 3
#define FIRE_TASKS 2
#define CORONA_TASKS 4
#define MAX_TOTAL_CONCURRENT_TASKS 10
#define TASKS_QUEUE_SIZE 10
#define TASKS_MEMORY_SIZE	configMINIMAL_STACK_SIZE

#define HANDLE_TASKS_PRIORITY (configMAX_PRIORITIES - 10)

#define FLAG_AMBULANCE 	(1 << 0) // 0x01
#define FLAG_POLICE 	(1 << 1) // 0x02
#define FLAG_FIRE		(1 << 2) // 0x04
#define FLAG_CORONA		(1 << 3) // 0x08

#define MAX_NAME_LEN 12

#define MIN_TIM2_IT_PERIOD	100 	// in milliseconds
#define MAX_TIM2_IT_PERIOD	500 // in milliseconds

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
	char* message;
} DispatcherPacket;


#endif /* INC_DEFS_H_ */
