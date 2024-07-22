/*
 * dispatcher.c
 *
 *  Created on: Jul 12, 2024
 *      Author: eyalk
 */

#include "dispatcher.h"


void packetRouting(DispatcherPacket* new_packet);


static const char vAMBstrings[AMB_STRINGS_LEN][MAX_MSG_LENGTH] = { AMBstr1,
																	AMBstr2,
																	AMBstr3,
																	AMBstr4,
																	AMBstr5,
																	AMBstr6,
																	AMBstr7,
																	AMBstr8};

static const char vPOLstrings[POLICE_STRINGS_LEN][MAX_MSG_LENGTH] = { 	POLstr1,
																		POLstr2,
																		POLstr3,
																		POLstr4,
																		POLstr5};

static const char vFIREstrings[FIRE_STRINGS_LEN][MAX_MSG_LENGTH] = { FIREstr1,
																	FIREstr2,
																	FIREstr3,
																	FIREstr4,
																	FIREstr5,
																	FIREstr6,
																	FIREstr7,
																	FIREstr8,
																	FIREstr9};

static const char vCORstrings[CORONA_STRINGS_LEN][MAX_MSG_LENGTH] = { 	CORstr1,
																		CORstr2,
																		CORstr3,
																		CORstr4,
																		CORstr5,
																		CORstr6,
																		CORstr7,
																		CORstr8};


/**
 * @brief Task function for the dispatcher.
 *
 * This function implements the dispatcher task, which waits for notifications
 * to process and route incoming dispatcher packets to the appropriate department queues.
 *
 * @param pvParameters Parameters passed to the task (expected to be `1`).
 */
void vDispatcherCode(void *pvParameters) {
	configASSERT(((uint32_t) pvParameters) == 1);
	uint32_t ulNotificationValue;
	for(;;) {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
		//printf("Dispatcher Entered BLOCKED state! \r\n");
		//fflush(stdout);
		xTaskNotifyWait(0x00, 0x00, &ulNotificationValue, portMAX_DELAY);
		//printf("Dispatcher in RUNNING state! \n \tPerforming a task! \r\n");
		//fflush(stdout);
		DispatcherPacket new_packet;
		if( xQueueReceive(qDispatcher, &new_packet, portMAX_DELAY) == pdPASS) {
			packetRouting(&new_packet);

		}
	}
}


/**
 * @brief Routes a dispatcher packet to the appropriate department queue.
 *
 * This function sends the given dispatcher packet to the queue corresponding
 * to its department.
 *
 * @param new_packet Pointer to the DispatcherPacket to be routed.
 */
void packetRouting(DispatcherPacket* new_packet) {
	switch(new_packet->department) {
		case AMBULANCE:
			xQueueSend(qAmbulance, new_packet, portMAX_DELAY);
			break;
		case POLICE:
			xQueueSend(qPolice, new_packet, portMAX_DELAY);
			break;
		case FIRE:
			xQueueSend(qFire, new_packet, portMAX_DELAY);
			break;
		case CORONA:
			xQueueSend(qCorona, new_packet, portMAX_DELAY);
			break;
	}
}




/**
 * @brief Generate a dispatcher message.
 *
 * This function generates a dispatcher message based on a random department and message.
 * It also sets the time required to handle the task in ticks.
 *
 * @param hDispPacket Pointer to the DispatcherPacket to be filled with generated data.
 */
void generateDispatcherMSG(DispatcherPacket* hDispPacket) {
	/*
	 * TODO: Consider creating a Mutex for when trying to modify hDispPacket.
	 * 		 It's also used in the TIM2 Interrupt Handler.
	 */
	DepartmentsEnum dep;
	uint8_t msgIdx = 0;

	// Generate Department
	uint32_t random_number = 0;
	if (HAL_RNG_GenerateRandomNumber(&hrng, &random_number) == HAL_OK) {
	        dep = (random_number % 4);
	        hDispPacket->department = dep;
	}

	// Pick a message
	switch(dep) {
		case AMBULANCE:
			if (HAL_RNG_GenerateRandomNumber(&hrng, &random_number) == HAL_OK) {
				msgIdx = (uint8_t)(random_number % AMB_STRINGS_LEN);
			}
			strncpy(hDispPacket->message, vAMBstrings[msgIdx], MAX_MSG_LENGTH - 1);
			hDispPacket->message[MAX_MSG_LENGTH - 1] = '\0';
			break;


		case POLICE:
			if (HAL_RNG_GenerateRandomNumber(&hrng, &random_number) == HAL_OK) {
				msgIdx = (uint8_t)(random_number % POLICE_STRINGS_LEN);
			}
			strncpy(hDispPacket->message, vPOLstrings[msgIdx], MAX_MSG_LENGTH - 1);
			hDispPacket->message[MAX_MSG_LENGTH - 1] = '\0';
			break;


		case FIRE:
			if (HAL_RNG_GenerateRandomNumber(&hrng, &random_number) == HAL_OK) {
				msgIdx = (uint8_t)(random_number % FIRE_STRINGS_LEN);
			}
			strncpy(hDispPacket->message, vFIREstrings[msgIdx], MAX_MSG_LENGTH - 1);
			hDispPacket->message[MAX_MSG_LENGTH - 1] = '\0';
			break;


		case CORONA:
			if (HAL_RNG_GenerateRandomNumber(&hrng, &random_number) == HAL_OK) {
				msgIdx = (uint8_t)(random_number % CORONA_STRINGS_LEN);
			}
			strncpy(hDispPacket->message, vCORstrings[msgIdx], MAX_MSG_LENGTH - 1);
			hDispPacket->message[MAX_MSG_LENGTH - 1] = '\0';
			break;


		default:
			break;
	}

	// Pick handling time
	if (HAL_RNG_GenerateRandomNumber(&hrng, &random_number) == HAL_OK) {
		hDispPacket->timeToHandleInTicks = (uint16_t)(random_number % LONGEST_TASK_DURATION_IN_TICKS) +
													SHORTEST_TASK_DURATION_IN_TICKS;
	}
}
