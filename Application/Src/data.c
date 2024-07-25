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
    char logBuffer[MAX_MSG_LENGTH];

    for (;;) {
        if (!btnFlag) {
            printedStatus = false;
            xTaskNotifyWait(0x00, 0x00, &ulNotificationValue, portMAX_DELAY);
        }
        if (!printedStatus) {
            taskENTER_CRITICAL();
            vTaskSuspend(vTasksManagerTask);
            vTaskSuspend(vLoggerTask);


            snprintf(logBuffer, MAX_MSG_LENGTH, "\n\n************** Tasks Status Report **************\r\n");
            SendDataMessage(logBuffer);

            snprintf(logBuffer, MAX_MSG_LENGTH, "           Current Running tasks: %d\r\n", (int)current_running_tasks);
            SendDataMessage(logBuffer);

            snprintf(logBuffer, MAX_MSG_LENGTH, "              Total Ran Tasks: %d\r\n", (int)total_tasks_ran);
            SendDataMessage(logBuffer);

            snprintf(logBuffer, MAX_MSG_LENGTH, "     Total tasks running time: %.3f seconds\r\n", total_tasks_time);
            SendDataMessage(logBuffer);

            snprintf(logBuffer, MAX_MSG_LENGTH, "     Average task running time: %.3f seconds\r\n", average_task_time);
            SendDataMessage(logBuffer);

            snprintf(logBuffer, MAX_MSG_LENGTH, "\n********* Ambulances Tasks Status Report *********\r\n");
            SendDataMessage(logBuffer);

            snprintf(logBuffer, MAX_MSG_LENGTH, "           Total ambulance tasks: %d\r\n", (int)AMBULANCE_TASKS);
            SendDataMessage(logBuffer);

            snprintf(logBuffer, MAX_MSG_LENGTH, "           Current occupied tasks: %d\r\n", (int)(AMBULANCE_TASKS - available_amb_tasks));
            SendDataMessage(logBuffer);

            snprintf(logBuffer, MAX_MSG_LENGTH, "           Current available tasks: %d\r\n", (int)available_amb_tasks);
            SendDataMessage(logBuffer);

            snprintf(logBuffer, MAX_MSG_LENGTH, "\n*********** Police Tasks Status Report ***********\r\n");
            SendDataMessage(logBuffer);

            snprintf(logBuffer, MAX_MSG_LENGTH, "           Total police tasks: %d\r\n", (int)POLICE_TASKS);
            SendDataMessage(logBuffer);

            snprintf(logBuffer, MAX_MSG_LENGTH, "           Current occupied tasks: %d\r\n", (int)(POLICE_TASKS - available_police_tasks));
            SendDataMessage(logBuffer);

            snprintf(logBuffer, MAX_MSG_LENGTH, "           Current available tasks: %d\r\n", (int)available_police_tasks);
            SendDataMessage(logBuffer);

            snprintf(logBuffer, MAX_MSG_LENGTH, "\n********** Fire Dep Tasks Status Report **********\r\n");
            SendDataMessage(logBuffer);

            snprintf(logBuffer, MAX_MSG_LENGTH, "           Total fire dep tasks: %d\r\n", (int)FIRE_TASKS);
            SendDataMessage(logBuffer);

            snprintf(logBuffer, MAX_MSG_LENGTH, "           Current occupied tasks: %d\r\n", (int)(FIRE_TASKS - available_fire_tasks));
            SendDataMessage(logBuffer);

            snprintf(logBuffer, MAX_MSG_LENGTH, "           Current available tasks: %d\r\n", (int)available_fire_tasks);
            SendDataMessage(logBuffer);

            snprintf(logBuffer, MAX_MSG_LENGTH, "\n*********** Corona Tasks Status Report ************\r\n");
            SendDataMessage(logBuffer);

            snprintf(logBuffer, MAX_MSG_LENGTH, "           Total corona tasks: %d\r\n", (int)CORONA_TASKS);
            SendDataMessage(logBuffer);

            snprintf(logBuffer, MAX_MSG_LENGTH, "           Current occupied tasks: %d\r\n", (int)(CORONA_TASKS - available_corona_tasks));
            SendDataMessage(logBuffer);

            snprintf(logBuffer, MAX_MSG_LENGTH, "           Current available tasks: %d\r\n\n\n", (int)available_corona_tasks);
            SendDataMessage(logBuffer);
            xTaskNotify(vBtnDataTask, 0x00, eNoAction);
            //vTaskSuspend(vBtnDataTask);
            taskEXIT_CRITICAL();
            printedStatus = !printedStatus;
            xTaskNotifyWait(0x00, 0x00, &ulNotificationValue, portMAX_DELAY);
        }
    }
}
