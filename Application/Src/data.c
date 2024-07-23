/*
 * data.c
 *
 *  Created on: Jul 23, 2024
 *      Author: eyalk
 */
#include "data.h"


void getTasksStatus(void) {
	uint32_t ulNotificationValue;
	bool printedStatus = false;
	for(;;) {
		if(!btnFlag) {
			printedStatus = false;
			xTaskNotifyWait(0x00, 0x00, &ulNotificationValue, portMAX_DELAY);
		}
		taskENTER_CRITICAL();
		/* Current Running tasks
		 * Total Ran Tasks
		 * Total Time
		 * Average task time
		 * Ambulance tasks details
		 * Police Tasks details
		 * Fire Dep tasks details
		 * Corona tasks details
		 */
		if(!printedStatus) {
			printf("\n\n"
					"************** Tasks Status Report **************\r\n"
					"            Current Running tasks: %d\r\n"
					"              Total Ran Tasks: %d\r\n"
					"          Total tasks running time: %.3f\r\n"
					"          Average task running time: %.3f\r\n",
					(int)current_running_tasks,
					(int)total_tasks_ran,
					total_tasks_time,
					average_task_time);
			fflush(stdout);
			printf("********* Ambulances Tasks Status Report *********\n"
					"              Total ambulance tasks: %d\r\n"
					"              Current occupied tasks: %d\r\n"
					"              current available tasks: %d\r\n",
					(int)AMBULANCE_TASKS,
					(int)(AMBULANCE_TASKS - available_amb_tasks),
					(int)available_amb_tasks);
			fflush(stdout);
			printf("*********** Police Tasks Status Report ***********\n"
					"             Total police tasks: %d\r\n"
					"             Current occupied tasks: %d\r\n"
					"             current available tasks: %d\r\n",
					(int)POLICE_TASKS,
					(int)(POLICE_TASKS - available_police_tasks),
					(int)available_police_tasks);
			fflush(stdout);
			printf("********** Fire Dep Tasks Status Report **********\n"
					"              Total fire dep tasks: %d\r\n"
					"              Current occupied tasks: %d\r\n"
					"              current available tasks: %d\r\n",
					(int)FIRE_TASKS,
					(int)(FIRE_TASKS - available_fire_tasks),
					(int)available_fire_tasks);
			fflush(stdout);
			printf("*********** Corona Tasks Status Report ************\n"
					"              Total corona tasks: %d\r\n"
					"              Current occupied tasks: %d\r\n"
					"              current available tasks: %d\r\n\n\n",
					(int)CORONA_TASKS,
					(int)(CORONA_TASKS - available_corona_tasks),
					(int)available_corona_tasks);
			fflush(stdout);
			printedStatus = !printedStatus;
			btnFlag = false;

			//taskYIELD();
		}
		taskEXIT_CRITICAL();
	}
}
