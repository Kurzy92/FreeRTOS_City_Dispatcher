/*
 * init.c
 *
 *  Created on: Jul 23, 2024
 *      Author: eyalk
 */

#include "init.h"

/**
 * @brief Initialize and start the various system tasks.
 */
void initTasks(void) {
	/* AMBULANCE TASK INIT */
	if(initAmbTasks() == -1) {
		error_handling("Ambulance tasks creation error!\r\n");
	}

	/* POLICE TASK INIT */
	if(initPolTasks() == -1) {
		error_handling("Police tasks creation error!\r\n");
	}

	/* FIRE DEP TASK INIT */
	if(initFireTasks() == -1) {
		error_handling("Fire Dep tasks creation error!\r\n");
	}

	/* CORONA TASK INIT */
	if(initCoronaTasks() == -1) {
		error_handling("Corona tasks creation error!\r\n");
	}

	vTaskDelay(100);

	BaseType_t status = xTaskCreate((TaskFunction_t)vDispatcherCode,
							"Dispatcher_Task",
							TASKS_MEMORY_SIZE,
							(void*)1,
							DISPATCHER_TASK_PRIORITY,
							&vDispatcherTask);
	if(status != pdTRUE) {
		error_handling("Dispatcher task creation error!\r\n");
	}

	status = xTaskCreate((TaskFunction_t)tasksManagerTask,
						"tasks_manager",
						TASKS_MEMORY_SIZE,
						(void*)1,
						MANAGER_TASK_PRIORITY,
						&vTasksManagerTask);
	if(status != pdTRUE) {
		error_handling("Task Manager task creation error!\r\n");
	}
	status = xTaskCreate((TaskFunction_t)getTasksStatus,
						"get_data_task",
						configMINIMAL_STACK_SIZE,
						(void*)1,
						GET_DATA_TASK_PRIORITY,
						&vGetDataTask);
	if(status != pdTRUE) {
		error_handling("Get Data task creation error!\r\n");
	}


	if(HAL_TIM_Base_Start_IT(&htim2) != HAL_OK) {
		error_handling("Failed to start timer2!\r\n");
	}

	for(;;) {
		uint32_t ulNotificationValue;
		xTaskNotifyWait(0x00, 0x00, &ulNotificationValue, portMAX_DELAY);
	}
}

/**
 * @brief Initialize all required queues.
 */
void initQueues(void) {
	qAmbulance = xQueueCreate(TASKS_QUEUE_SIZE, sizeof(DispatcherPacket));
	if(qAmbulance == NULL) {
		error_handling("Ambulance queue creation failed!\r\n");
	}
	qPolice = xQueueCreate(TASKS_QUEUE_SIZE, sizeof(DispatcherPacket));
	if(qPolice == NULL) {
		error_handling("Police queue creation failed!\r\n");
	}
	qFire = xQueueCreate(TASKS_QUEUE_SIZE, sizeof(DispatcherPacket));
	if(qFire == NULL) {
		error_handling("Fire Dep queue creation failed!\r\n");
	}
	qCorona = xQueueCreate(TASKS_QUEUE_SIZE, sizeof(DispatcherPacket));
	if(qCorona == NULL) {
		error_handling("Corona queue creation failed!\r\n");
	}
	qDispatcher = xQueueCreate(TASKS_QUEUE_SIZE, sizeof(DispatcherPacket));
	if(qDispatcher == NULL) {
		error_handling("Dispatcher queue creation failed!\r\n");
	}
}




/**
 * @brief Initialize all required semaphores.
 */
void initSemaphores(void) {
	printfMutex = xSemaphoreCreateMutex();
	if(printfMutex == NULL) {
		error_handling("Print mutex creation failed!\r\n");
	}
	xTasksDataMutex = xSemaphoreCreateMutex();
	if(xTasksDataMutex == NULL) {
		error_handling("Tasks data mutex creation failed!\r\n");
	}
	AmbTasksStatusMutex = xSemaphoreCreateMutex();
	if(AmbTasksStatusMutex == NULL) {
		error_handling("Ambulance tasks mutex creation failed!\r\n");
	}
	PolTasksStatusMutex = xSemaphoreCreateMutex();
	if(PolTasksStatusMutex == NULL) {
		error_handling("Police tasks mutex creation failed!\r\n");
	}
	FireTasksStatusMutex = xSemaphoreCreateMutex();
	if(FireTasksStatusMutex == NULL) {
		error_handling("Fire Dep tasks mutex creation failed!\r\n");
	}
	CorTasksStatusMutex = xSemaphoreCreateMutex();
	if(CorTasksStatusMutex == NULL) {
		error_handling("Corona tasks mutex creation failed!\r\n");
	}
}

/**
 * @brief Initialize and start Ambulance tasks.
 *
 * @return int8_t Returns 0 on success, -1 on failure.
 */
int8_t initAmbTasks(void) {
	for(int i=0; i<AMBULANCE_TASKS; i++) {
		taskInit_t* ambulance_taskInit = (taskInit_t*)pvPortMalloc(sizeof(taskInit_t));
		configASSERT(ambulance_taskInit != NULL);

		ambulance_taskInit->department = AMBULANCE;
		ambulance_taskInit->taskIdentifier = i;
		ambulance_taskInit->pQhandler = &qAmbulance;
		ambulance_taskInit->pSemHandler = &AmbTasksStatusMutex;
		ambulance_taskInit->bTaskStatusArr = bAmbTasksStatus;
		ambulance_taskInit->numOfTasks = AMBULANCE_TASKS;

		BaseType_t status;
		char taskName[configMAX_TASK_NAME_LEN];
		snprintf(taskName, configMAX_TASK_NAME_LEN, "Ambulance_%d", i);
	 	status = xTaskCreate((TaskFunction_t)vHandleCall,
	 							taskName,
								TASKS_MEMORY_SIZE,
								(void*)ambulance_taskInit,
								HANDLE_TASKS_PRIORITY,
								&vAmbulanceTasks[i]);
	 	if(status != pdTRUE) {
	 		return -1;
	 	}
	}
	if(xSemaphoreTake(printfMutex, portMAX_DELAY) == pdTRUE) {
		printf("Started Ambulance Threads! \r\n");
		fflush(stdout);
		xSemaphoreGive(printfMutex);
	}
	return 0;
}


/**
 * @brief Initialize and start Police tasks.
 *
 * @return int8_t Returns 0 on success, -1 on failure.
 */
int8_t initPolTasks(void) {
	for(int i=0; i<POLICE_TASKS; i++) {
		taskInit_t* police_taskInit = (taskInit_t*)pvPortMalloc(sizeof(taskInit_t));
		configASSERT(police_taskInit != NULL);

		police_taskInit->department = POLICE;
		police_taskInit->taskIdentifier = i;
		police_taskInit->pQhandler = &qPolice;
		police_taskInit->pSemHandler = &PolTasksStatusMutex;
		police_taskInit->bTaskStatusArr = bPolTasksStatus;
		police_taskInit->numOfTasks = POLICE_TASKS;

		BaseType_t status;
		char taskName[configMAX_TASK_NAME_LEN];
		snprintf(taskName, configMAX_TASK_NAME_LEN, "Police_%d", i);
	 	status = xTaskCreate((TaskFunction_t)vHandleCall,
	 							taskName,
								TASKS_MEMORY_SIZE,
								(void*) police_taskInit,
								HANDLE_TASKS_PRIORITY,
								&vPoliceTasks[i]);
	 	if(status != pdTRUE) {
	 		return -1;
	 	}
	}
	if(xSemaphoreTake(printfMutex, portMAX_DELAY) == pdTRUE) {
		printf("Started Police Threads! \r\n");
		fflush(stdout);
		xSemaphoreGive(printfMutex);
	}
	return 0;
}


/**
 * @brief Initialize and start Fire Dep tasks.
 *
 * @return int8_t Returns 0 on success, -1 on failure.
 */
int8_t initFireTasks(void) {
	for(int i=0; i<FIRE_TASKS; i++) {
		taskInit_t* fire_taskInit = (taskInit_t*)pvPortMalloc(sizeof(taskInit_t));
		configASSERT(fire_taskInit != NULL);

		fire_taskInit->department = FIRE;
		fire_taskInit->taskIdentifier = i;
		fire_taskInit->pQhandler = &qFire;
		fire_taskInit->pSemHandler = &FireTasksStatusMutex;
		fire_taskInit->bTaskStatusArr = bFireTasksStatus;
		fire_taskInit->numOfTasks = FIRE_TASKS;
		BaseType_t status;
		char taskName[configMAX_TASK_NAME_LEN];
		snprintf(taskName, configMAX_TASK_NAME_LEN, "Fire_%d", i);
	 	status = xTaskCreate((TaskFunction_t)vHandleCall,
	 							taskName,
								TASKS_MEMORY_SIZE,
								(void*) fire_taskInit,
								HANDLE_TASKS_PRIORITY,
								&vFireTasks[i]);
	 	if(status != pdTRUE) {
	 		return -1;
	 	}
	}
	if(xSemaphoreTake(printfMutex, portMAX_DELAY) == pdTRUE) {
		printf("Started Fire Dep Threads! \r\n");
		fflush(stdout);
		xSemaphoreGive(printfMutex);
	}
	return 0;
}


/**
 * @brief Initialize and start Corona tasks.
 *
 * @return int8_t Returns 0 on success, -1 on failure.
 */
int8_t initCoronaTasks(void) {
	for(int i=0; i<CORONA_TASKS; i++) {
		taskInit_t* corona_taskInit = (taskInit_t*)pvPortMalloc(sizeof(taskInit_t));
		configASSERT(corona_taskInit != NULL);

		corona_taskInit->department = CORONA;
		corona_taskInit->taskIdentifier = i;
		corona_taskInit->pQhandler = &qCorona;
		corona_taskInit->pSemHandler = &CorTasksStatusMutex;
		corona_taskInit->bTaskStatusArr = bCorTasksStatus;
		corona_taskInit->numOfTasks = CORONA_TASKS;

		BaseType_t status;
		char taskName[configMAX_TASK_NAME_LEN];
		snprintf(taskName, configMAX_TASK_NAME_LEN, "Corona_%d", i);
	 	status = xTaskCreate((TaskFunction_t)vHandleCall,
	 							taskName,
								TASKS_MEMORY_SIZE,
								(void*) corona_taskInit,
								HANDLE_TASKS_PRIORITY,
								&vCoronaTasks[i]);
	 	if(status != pdTRUE) {
	 		return -1;
	 	}
	}
	if(xSemaphoreTake(printfMutex, portMAX_DELAY) == pdTRUE) {
		printf("Started Corona Threads! \r\n");
		fflush(stdout);
		xSemaphoreGive(printfMutex);
	}
	return 0;
}
