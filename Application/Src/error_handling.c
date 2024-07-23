/*
 * error_handling.c
 *
 *  Created on: Jul 23, 2024
 *      Author: eyalk
 */

#include "error_handling.h"

void error_handling(char* err_msg) {
	if (err_msg != NULL) {
		if(xSemaphoreTake(printfMutex, portMAX_DELAY) == pdTRUE) {
			printf("%s\r\n", err_msg);
			fflush(stdout);
			xSemaphoreGive(printfMutex);
		}
	} else {
		if(xSemaphoreTake(printfMutex, portMAX_DELAY) == pdTRUE) {
			printf("Unknown error");
			fflush(stdout);
			xSemaphoreGive(printfMutex);
		}
	}
	printf("Program Terminated. \r\n");
	Error_Handler();
}
