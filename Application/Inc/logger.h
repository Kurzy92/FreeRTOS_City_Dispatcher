/*
 * logger.h
 *
 *  Created on: Jul 25, 2024
 *      Author: eyalk
 */

#ifndef INC_LOGGER_H_
#define INC_LOGGER_H_

#include "main.h"


void Logger_Print(void *pvParameters);
void SendLogMessage(const char *message);
void SendDataMessage(const char *message);
void Data_Print(void *pvParameters);

#endif /* INC_LOGGER_H_ */
