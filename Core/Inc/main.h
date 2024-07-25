/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "defs.h"
#include "dispatcher.h"
#include "stdbool.h"
#include "stdio.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "queue.h"
#include "semphr.h"
#include "task_handling.h"
#include "timers.h"
#include "error_handling.h"
#include "init.h"
#include "logger.h"


/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern QueueHandle_t qDispatcher;
extern QueueHandle_t qAmbulance;
extern QueueHandle_t qPolice;
extern QueueHandle_t qFire;
extern QueueHandle_t qCorona;
extern QueueHandle_t qLogger;
extern QueueHandle_t qBtnData;

extern TaskHandle_t vAmbulanceTasks[AMBULANCE_TASKS];
extern TaskHandle_t vPoliceTasks[POLICE_TASKS];
extern TaskHandle_t vFireTasks[FIRE_TASKS];
extern TaskHandle_t vCoronaTasks[CORONA_TASKS];
extern TaskHandle_t vGetDataTask;
extern TaskHandle_t vDispatcherTask;
extern TaskHandle_t vTasksManagerTask;
extern TaskHandle_t vGetDataTask;
extern TaskHandle_t vLoggerTask;
extern TaskHandle_t vBtnDataTask;

extern RNG_HandleTypeDef hrng;
extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart3;


extern SemaphoreHandle_t xTasksDataMutex;
extern SemaphoreHandle_t printfMutex;
extern uint32_t total_tasks_ran;

extern bool bAmbTasksStatus[AMBULANCE_TASKS];
extern bool bPolTasksStatus[POLICE_TASKS];
extern bool bFireTasksStatus[FIRE_TASKS];
extern bool bCorTasksStatus[CORONA_TASKS];

extern SemaphoreHandle_t AmbTasksStatusMutex;
extern SemaphoreHandle_t PolTasksStatusMutex;
extern SemaphoreHandle_t FireTasksStatusMutex;
extern SemaphoreHandle_t CorTasksStatusMutex;

extern float total_tasks_time;
extern uint32_t total_tasks_ran;
extern float average_task_time;
extern int8_t current_running_tasks;

extern uint8_t available_amb_tasks;
extern uint8_t available_police_tasks;
extern uint8_t available_fire_tasks;
extern uint8_t available_corona_tasks;
extern bool btnFlag;


/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void DebounceTimerCallback(TimerHandle_t xTimer);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define USER_Btn_Pin GPIO_PIN_13
#define USER_Btn_GPIO_Port GPIOC
#define USER_Btn_EXTI_IRQn EXTI15_10_IRQn
#define MCO_Pin GPIO_PIN_0
#define MCO_GPIO_Port GPIOH
#define RMII_MDC_Pin GPIO_PIN_1
#define RMII_MDC_GPIO_Port GPIOC
#define RMII_REF_CLK_Pin GPIO_PIN_1
#define RMII_REF_CLK_GPIO_Port GPIOA
#define RMII_MDIO_Pin GPIO_PIN_2
#define RMII_MDIO_GPIO_Port GPIOA
#define RMII_CRS_DV_Pin GPIO_PIN_7
#define RMII_CRS_DV_GPIO_Port GPIOA
#define RMII_RXD0_Pin GPIO_PIN_4
#define RMII_RXD0_GPIO_Port GPIOC
#define RMII_RXD1_Pin GPIO_PIN_5
#define RMII_RXD1_GPIO_Port GPIOC
#define LD1_Pin GPIO_PIN_0
#define LD1_GPIO_Port GPIOB
#define RMII_TXD1_Pin GPIO_PIN_13
#define RMII_TXD1_GPIO_Port GPIOB
#define LD3_Pin GPIO_PIN_14
#define LD3_GPIO_Port GPIOB
#define STLK_RX_Pin GPIO_PIN_8
#define STLK_RX_GPIO_Port GPIOD
#define STLK_TX_Pin GPIO_PIN_9
#define STLK_TX_GPIO_Port GPIOD
#define USB_PowerSwitchOn_Pin GPIO_PIN_6
#define USB_PowerSwitchOn_GPIO_Port GPIOG
#define USB_OverCurrent_Pin GPIO_PIN_7
#define USB_OverCurrent_GPIO_Port GPIOG
#define USB_SOF_Pin GPIO_PIN_8
#define USB_SOF_GPIO_Port GPIOA
#define USB_VBUS_Pin GPIO_PIN_9
#define USB_VBUS_GPIO_Port GPIOA
#define USB_ID_Pin GPIO_PIN_10
#define USB_ID_GPIO_Port GPIOA
#define USB_DM_Pin GPIO_PIN_11
#define USB_DM_GPIO_Port GPIOA
#define USB_DP_Pin GPIO_PIN_12
#define USB_DP_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define RMII_TX_EN_Pin GPIO_PIN_11
#define RMII_TX_EN_GPIO_Port GPIOG
#define RMII_TXD0_Pin GPIO_PIN_13
#define RMII_TXD0_GPIO_Port GPIOG
#define SW0_Pin GPIO_PIN_3
#define SW0_GPIO_Port GPIOB
#define LD2_Pin GPIO_PIN_7
#define LD2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
// Data task defines
#define DATA_GPIO_PORT 	GPIOC
#define DATA_GPIO_PIN 	GPIO_PIN_13
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
