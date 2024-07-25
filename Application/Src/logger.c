/*
 * logger.c
 *
 *  Created on: Jul 25, 2024
 *      Author: eyalk
 */

#include "logger.h"

void Logger_Print(void *pvParameters) {
    char msg[MAX_MSG_LENGTH];

    for (;;) {
        if (xQueueReceive(qLogger, &msg, portMAX_DELAY) == pdPASS) {
        	taskENTER_CRITICAL();
            if(HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), 200) != HAL_OK) {
            	vTaskDelay(20);
            }
            taskEXIT_CRITICAL();
            vTaskDelay(pdMS_TO_TICKS(20));

        }
    }
}


void SendLogMessage(const char *message) {
    char logMessage[MAX_MSG_LENGTH];

    // Ensure the message fits into the logMessage buffer
    strncpy(logMessage, message, MAX_MSG_LENGTH - 1);
    logMessage[MAX_MSG_LENGTH - 1] = '\0';  // Null-terminate to avoid overflow

    if (xQueueSend(qLogger, logMessage, 100) != pdPASS) {
        // Failed to send to queue
    }
}

void SendDataMessage(const char *message) {
    char dataMessage[MAX_MSG_LENGTH];

    // Ensure the message fits into the logMessage buffer
    strncpy(dataMessage, message, MAX_MSG_LENGTH - 1);
    dataMessage[MAX_MSG_LENGTH - 1] = '\0';  // Null-terminate to avoid overflow

    if (xQueueSend(qBtnData, dataMessage, 100) != pdPASS) {
        // Failed to send to queue
    }
}

void Data_Print(void *pvParameters) {
    char msg[MAX_MSG_LENGTH];
    uint32_t ulNotificationValue;
    for (;;) {
    	UBaseType_t qItems = uxQueueMessagesWaiting(qBtnData);
    	if((uint32_t)qItems == 0) {
    		xTaskNotifyWait(0x00, 0x00, &ulNotificationValue, portMAX_DELAY);
    	} else {
    		if (xQueueReceive(qBtnData, &msg, portMAX_DELAY) == pdPASS) {
    			taskENTER_CRITICAL();
    			if(HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), 200) != HAL_OK) {
    				vTaskDelay(20);
    			}
    			taskEXIT_CRITICAL();
    			vTaskDelay(pdMS_TO_TICKS(20));
    		}

        }

    }
}
