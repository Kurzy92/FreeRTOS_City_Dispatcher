/*
 * logger.c
 *
 *  Created on: Jul 25, 2024
 *      Author: eyalk
 */

#include "logger.h"

/**
 * @brief Task to print log messages.
 *
 * This task continuously receives messages from the log queue (`qLogger`)
 * and transmits them over UART.
 *
 * @param pvParameters Parameters passed to the task (unused).
 */
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


/**
 * @brief Sends a log message to the log queue.
 *
 * This function copies the provided message into a local buffer
 * and sends it to the log queue (`qLogger`).
 *
 * @param message The message to log.
 */
void SendLogMessage(const char *message) {
    char logMessage[MAX_MSG_LENGTH];

    // Ensure the message fits into the logMessage buffer
    strncpy(logMessage, message, MAX_MSG_LENGTH - 1);
    logMessage[MAX_MSG_LENGTH - 1] = '\0';  // Null-terminate to avoid overflow

    if (xQueueSend(qLogger, logMessage, 100) != pdPASS) {
        // Failed to send to queue
    }
}


/**
 * @brief Sends a data message to the button data queue.
 *
 * This function copies the provided message into a local buffer
 * and sends it to the button data queue (`qBtnData`).
 *
 * @param message The message to send.
 */
void SendDataMessage(const char *message) {
    char dataMessage[MAX_MSG_LENGTH];

    // Ensure the message fits into the logMessage buffer
    strncpy(dataMessage, message, MAX_MSG_LENGTH - 1);
    dataMessage[MAX_MSG_LENGTH - 1] = '\0';  // Null-terminate to avoid overflow

    if (xQueueSend(qBtnData, dataMessage, 100) != pdPASS) {
        // Failed to send to queue
    }
}


/**
 * @brief Task to print data messages.
 *
 * This task continuously checks the button data queue (`qBtnData`) for messages.
 * If the queue is empty, it waits for a notification. When a message is received,
 * it is transmitted over UART.
 *
 * @param pvParameters Parameters passed to the task (unused).
 */
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
