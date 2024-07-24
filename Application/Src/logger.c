/*
 * logger.c
 *
 *  Created on: Jul 25, 2024
 *      Author: eyalk
 */

#include "logger.h"

void Logger_Print(void) {
	char msg[MAX_MSG_LENGTH];

	for(;;) {
		if(xQueueReceive(qLogger, &msg, 10) != pdPASS) {
			continue;
		}
		#ifdef DEBUG
		printf("%s", msg);
		fflush(stdout);
		#else
		HAL_UART_Transmit(LOGGER_HUART, msg, MAX_MSG_LENGTH, 10);
		vTaskDelay(20);
		#endif
	}
}

