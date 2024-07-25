/**
 * @file task_handling.c
 * @brief Task handling and management for various emergency services.
 *
 * This file contains functions to manage and handle tasks for Ambulance, Police,
 * Fire Department, and Corona response services. It includes task creation,
 * task handling, and updating task statuses.
 *
 * @date Jul 15, 2024
 * @author eyalk
 */

#include "task_handling.h"

static int8_t getAvailableAmbulanceTask(void);
static int8_t getAvailablePoliceeTask(void);
static int8_t getAvailableFireTask(void);
static int8_t getAvailableCoronaTask(void);
static void removeCurrentRunningTask(uint8_t* avail_dep_tasks);
static void addCurrentRunningTask(uint8_t* avail_dep_tasks);

char logInitBuffer[MAX_MSG_LENGTH];
char logInitBuffer2[MAX_MSG_LENGTH];

/**
 * @brief Manages and dispatches tasks based on available resources.
 *
 * This function continuously checks for available tasks in the queues for
 * Ambulance, Police, Fire Department, and Corona services. If there are
 * available tasks and the current running tasks are below the maximum allowed,
 * it assigns tasks to the appropriate handlers.
 */
void tasksManagerTask(void) {
	uint32_t ulNotificationValue;
	int8_t newTaskIndex;
	for(;;) {
		if(current_running_tasks >= MAX_TOTAL_CONCURRENT_TASKS) {
			xTaskNotifyWait(0x00, 0x00, &ulNotificationValue, TASK_MANAGER_TICKS_TO_WAIT);
		} else {
			// Check if there are Ambulance tasks to perform
			if(uxQueueMessagesWaiting( qAmbulance ) > 0) {
				newTaskIndex = getAvailableAmbulanceTask();
				if(newTaskIndex != -1) {
					if(xSemaphoreTake(printfMutex, portMAX_DELAY) == pdTRUE) {
						snprintf(logInitBuffer, MAX_MSG_LENGTH,"Ambulance handler number %d is now active\r\n", newTaskIndex+1);
						xSemaphoreGive(printfMutex);
					}
					addCurrentRunningTask(&available_amb_tasks);
					xTaskNotify(vAmbulanceTasks[newTaskIndex], 0, eNoAction);
					continue;
				}
			}
			// Check if there are Police tasks to perform
			if(uxQueueMessagesWaiting( qPolice ) > 0) {
				newTaskIndex = getAvailablePoliceeTask();
				if(newTaskIndex != -1) {
					if(xSemaphoreTake(printfMutex, portMAX_DELAY) == pdTRUE) {
						snprintf(logInitBuffer, MAX_MSG_LENGTH,"Police handler number %d is now active\r\n", newTaskIndex+1);
						xSemaphoreGive(printfMutex);
					}
					addCurrentRunningTask(&available_police_tasks);
					xTaskNotify(vPoliceTasks[newTaskIndex], 0, eNoAction);
					continue;
				}
			}

			// Check if there are Fire Dep tasks to perform
			if(uxQueueMessagesWaiting( qFire ) > 0) {
				newTaskIndex = getAvailableFireTask();
				if(newTaskIndex != -1) {
					if(xSemaphoreTake(printfMutex, portMAX_DELAY) == pdTRUE) {
						snprintf(logInitBuffer, MAX_MSG_LENGTH,"Fire dep handler number %d is now active\r\n", newTaskIndex+1);
						xSemaphoreGive(printfMutex);
					}
					addCurrentRunningTask(&available_fire_tasks);
					xTaskNotify(vFireTasks[newTaskIndex], 0, eNoAction);
					continue;
				}
			}

			// Check if there are Corona tasks to perform
			if(uxQueueMessagesWaiting( qCorona ) > 0) {
				newTaskIndex = getAvailableCoronaTask();
				if(newTaskIndex != -1) {
					if(xSemaphoreTake(printfMutex, portMAX_DELAY) == pdTRUE) {
						snprintf(logInitBuffer, MAX_MSG_LENGTH,"Corona handler number %d is now active\r\n", newTaskIndex+1);
						xSemaphoreGive(printfMutex);
					}
					addCurrentRunningTask(&available_corona_tasks);
					xTaskNotify(vCoronaTasks[newTaskIndex], 0, eNoAction);
					continue;
				}
			}
			portYIELD();

		}
	}
}



/**
 * @brief Handles individual tasks based on the department.
 *
 * This function is responsible for handling tasks assigned to it. It waits for
 * notifications indicating a new task to process, executes the task, and updates
 * the shared data with the results.
 *
 * @param pvParameters Pointer to the task initialization parameters.
 */
void vHandleCall(void* pvParameters) {
	uint32_t ulNotifictionValue;
	TickType_t startTick, endTick, totalTicks;
	taskInit_t* pTaskInit = (taskInit_t*)pvParameters;
	DispatcherPacket new_packet = {0};
	size_t max_message_length = MAX_MSG_LENGTH - 3;
	char printMSG[100];
	snprintf(printMSG, 100, "New %s task created! \r\nTask index is %d \r\n"
			,GET_ENUM_DEPARTMENT_STR(pTaskInit->department)
			, (pTaskInit->taskIdentifier+1));
	SendLogMessage(printMSG);
	if(xSemaphoreTake(printfMutex, portMAX_DELAY) == pdTRUE) {
		SendLogMessage(printMSG);
		xSemaphoreGive(printfMutex);
	}
	for(;;) {
		/* TODO: log a message that this task is created and it's waiting for a job */

		xTaskNotifyWait(0x00, 0x00, &ulNotifictionValue, portMAX_DELAY);

		startTick = xTaskGetTickCount();

		if(!(xQueueReceive(*(pTaskInit->pQhandler), &new_packet, portMAX_DELAY) == pdTRUE)) {
			continue;
		}

		// Wait for the job to get done.
		vTaskDelay(new_packet.timeToHandleInTicks);
		if(xSemaphoreTake(printfMutex, portMAX_DELAY) == pdTRUE) {
			snprintf(logInitBuffer, MAX_MSG_LENGTH,"%s - Handled task: ", GET_ENUM_DEPARTMENT_STR(pTaskInit->department));
			SendLogMessage(logInitBuffer);
			snprintf(logInitBuffer, MAX_MSG_LENGTH, "%.*s \r\n", (int)max_message_length, new_packet.message);
			SendLogMessage(logInitBuffer);
			xSemaphoreGive(printfMutex);
		}

		// Update shared app data
		if(xTasksDataMutex != NULL) {
			if(xSemaphoreTake(xTasksDataMutex, portMAX_DELAY) == pdTRUE) {
				total_tasks_ran++;
				endTick = xTaskGetTickCount();
				totalTicks = endTick - startTick;
				total_tasks_time += (float)totalTicks / configTICK_RATE_HZ;
				average_task_time = (float)total_tasks_time/total_tasks_ran;
				/*
				 * TODO: Calculating the average_task_time in this task is a waste
				 * 		 of resources. Data calculation should be done in its own
				 * 		 lower priority task and only run when printing of data
				 * 		 is required.
				 */
				xSemaphoreGive(xTasksDataMutex);
			}
		}
		// Indicate the task is now available for the next incoming packet.
		if(*pTaskInit->pSemHandler != NULL) {
			if(xSemaphoreTake(*(pTaskInit->pSemHandler), portMAX_DELAY) ==  pdTRUE) {
				removeCurrentRunningTask(new_packet.available_tasks_counter);
				pTaskInit->bTaskStatusArr[pTaskInit->taskIdentifier] = false;
				xSemaphoreGive(*(pTaskInit->pSemHandler));
			}
		}
	}
}


/**
 * @brief Increments the count of currently running tasks.
 *
 * This function safely increments the count of currently running tasks
 * using a semaphore to ensure thread safety.
 */
static void addCurrentRunningTask(uint8_t* avail_dep_tasks) {
	if(xTasksDataMutex != NULL) {
		if(xSemaphoreTake(xTasksDataMutex, portMAX_DELAY) == pdTRUE) {
			current_running_tasks++;
			*(avail_dep_tasks)-=1;
			xSemaphoreGive(xTasksDataMutex);
		}
	}
}


/**
 * @brief Decrements the count of currently running tasks.
 *
 * This function safely decrements the count of currently running tasks
 * using a semaphore to ensure thread safety.
 */
static void removeCurrentRunningTask(uint8_t* avail_dep_tasks) {
	if(xTasksDataMutex != NULL) {
		if(xSemaphoreTake(xTasksDataMutex, portMAX_DELAY) == pdTRUE) {
			current_running_tasks--;
			*(avail_dep_tasks)+=1;
			xSemaphoreGive(xTasksDataMutex);
		}
	}
}


/**
 * @brief Get an available ambulance task.
 *
 * This function iterates through the ambulance task statuses to find the first available task.
 * It returns the index of the available task, or -1 if all tasks are busy.
 *
 * @return int8_t The index of the available ambulance tasks, or -1 if all tasks are busy.
 */
static int8_t getAvailableAmbulanceTask(void) {
	if(xSemaphoreTake(AmbTasksStatusMutex, portMAX_DELAY) == pdTRUE) {
		for(int i = 0; i < AMBULANCE_TASKS; i++) {
			if(bAmbTasksStatus[i] == false) {
				bAmbTasksStatus[i] = true;
				xSemaphoreGive(AmbTasksStatusMutex);
				return (int8_t)i;
			}
		}
		xSemaphoreGive(AmbTasksStatusMutex);
	}
	return -1;
}

/**
 * @brief Get an available police task.
 *
 * This function iterates through the police task statuses to find the first available task.
 * It returns the index of the available task, or -1 if all tasks are busy.
 *
 * @return int8_t The index of the available police tasks, or -1 if all tasks are busy.
 */
static int8_t getAvailablePoliceeTask(void) {
	if(xSemaphoreTake(PolTasksStatusMutex, portMAX_DELAY) == pdTRUE) {
		for(int i = 0; i < POLICE_TASKS; i++) {
			if(bPolTasksStatus[i] == false) {
				bPolTasksStatus[i] = true;
				xSemaphoreGive(PolTasksStatusMutex);
				return (int8_t)i;
			}
		}
		xSemaphoreGive(PolTasksStatusMutex);
	}
	return -1;
}

/**
 * @brief Get an available fire department task.
 *
 * This function iterates through the fire department task statuses to find the first available task.
 * It returns the index of the available task, or -1 if all tasks are busy.
 *
 * @return int8_t The index of the available fire department tasks, or -1 if all tasks are busy.
 */
static int8_t getAvailableFireTask(void) {
	if(xSemaphoreTake(FireTasksStatusMutex, portMAX_DELAY) == pdTRUE) {
		for(int i = 0; i < FIRE_TASKS; i++) {
			if(bFireTasksStatus[i] == false) {
				bFireTasksStatus[i] = true;
				xSemaphoreGive(FireTasksStatusMutex);
				return (int8_t)i;
			}
		}
		xSemaphoreGive(FireTasksStatusMutex);
	}
	return -1;
}

/**
 * @brief Get an available corona task.
 *
 * This function iterates through the corona task statuses to find the first available task.
 * It returns the index of the available task, or -1 if all tasks are busy.
 *
 * @return int8_t The index of the available corona tasks, or -1 if all tasks are busy.
 */
static int8_t getAvailableCoronaTask(void) {
	if(xSemaphoreTake(CorTasksStatusMutex, portMAX_DELAY) == pdTRUE) {
		for(int i = 0; i < CORONA_TASKS; i++) {
			if(bCorTasksStatus[i] == false) {
				bCorTasksStatus[i] = true;
				xSemaphoreGive(CorTasksStatusMutex);
				return (int8_t)i;
			}
		}
		xSemaphoreGive(CorTasksStatusMutex);
	}
	return -1;
}
