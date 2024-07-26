/*
 * error_handling.c
 *
 *  Created on: Jul 23, 2024
 *      Author: eyalk
 */

#include "error_handling.h"

void error_handling(char* err_msg) {
	char logBuffer[MAX_MSG_LENGTH];
	if (err_msg != NULL) {
		if(xSemaphoreTake(printfMutex, portMAX_DELAY) == pdTRUE) {
			snprintf(logBuffer, MAX_MSG_LENGTH,"%s\r\n", err_msg);
			SendLogMessage(logBuffer);
			xSemaphoreGive(printfMutex);
		}
	} else {
		if(xSemaphoreTake(printfMutex, portMAX_DELAY) == pdTRUE) {
			snprintf(logBuffer, MAX_MSG_LENGTH,"Unknown error");
			SendLogMessage(logBuffer);
			xSemaphoreGive(printfMutex);
		}
	}
	snprintf(logBuffer, MAX_MSG_LENGTH,"Unknown error");
	SendLogMessage(logBuffer);
	Error_Handler();
}
