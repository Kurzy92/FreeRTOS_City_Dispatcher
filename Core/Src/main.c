/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

RNG_HandleTypeDef hrng;

SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_tx;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart3;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* Definitions for defaultTask */

/* USER CODE BEGIN PV */

// QUEUE Handles
QueueHandle_t qAmbulance;
QueueHandle_t qPolice;
QueueHandle_t qFire;
QueueHandle_t qCorona;
QueueHandle_t qDispatcher;
QueueHandle_t qLogger;

// TASKS Handles
TaskHandle_t vAmbulanceTasks[AMBULANCE_TASKS];
TaskHandle_t vPoliceTasks[POLICE_TASKS];
TaskHandle_t vFireTasks[FIRE_TASKS];
TaskHandle_t vCoronaTasks[CORONA_TASKS];
TaskHandle_t vDispatcherTask;
TaskHandle_t vInitTaskHandle;

// Display Tasks
TaskHandle_t LvglTask;
lv_display_t *lcd_disp;
volatile int lcd_bus_busy = 0;

// TASKS Data Objects
float total_tasks_time;
uint32_t total_tasks_ran;
float average_task_time;
SemaphoreHandle_t xTasksDataMutex;
SemaphoreHandle_t xPrintfMutex;

// Display Functions
void ui_init(lv_display_t *disp);
void LVGL_Task(void const *argument);



DispatcherPacket dispPack = {
		POLICE,
		"help!"
};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
static void MX_RNG_Init(void);
static void MX_TIM2_Init(void);
static void MX_SPI1_Init(void);

/* USER CODE BEGIN PFP */
void vHandleCall(void* pvParameters);
void initQueues(void);
void initTasks(void);
//void initTasks(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	total_tasks_time = 0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_RNG_Init();
  MX_TIM2_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  initQueues();
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */

  /* USER CODE BEGIN RTOS_THREADS */

  BaseType_t status;

  status = xTaskCreate((TaskFunction_t)LVGL_Task,
		  	  	  	  "LVGL_Task",
					  1024,
					  (void*)1,
					  7,
					  &LvglTask);
  configASSERT(status == pdTRUE);

  status = xTaskCreate((TaskFunction_t)initTasks,
		  "init_task",
		  configMINIMAL_STACK_SIZE,
		  (void*) 1,
		  configMAX_PRIORITIES,
		  &vInitTaskHandle);
  configASSERT(status == pdTRUE);

  xTasksDataMutex = xSemaphoreCreateMutex();
  xPrintfMutex = xSemaphoreCreateMutex();
  /* add threads, ... */

  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */

  vTaskStartScheduler();

  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief RNG Initialization Function
  * @param None
  * @retval None
  */
static void MX_RNG_Init(void)
{

  /* USER CODE BEGIN RNG_Init 0 */

  /* USER CODE END RNG_Init 0 */

  /* USER CODE BEGIN RNG_Init 1 */

  /* USER CODE END RNG_Init 1 */
  hrng.Instance = RNG;
  if (HAL_RNG_Init(&hrng) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RNG_Init 2 */

  /* USER CODE END RNG_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = TIM2_PRESCALER_SET;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = TIM2_PERIOD_SET;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USB_OTG_FS Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_OTG_FS_PCD_Init(void)
{

  /* USER CODE BEGIN USB_OTG_FS_Init 0 */

  /* USER CODE END USB_OTG_FS_Init 0 */

  /* USER CODE BEGIN USB_OTG_FS_Init 1 */

  /* USER CODE END USB_OTG_FS_Init 1 */
  hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
  hpcd_USB_OTG_FS.Init.dev_endpoints = 6;
  hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_OTG_FS.Init.dma_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_OTG_FS.Init.Sof_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.vbus_sensing_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_OTG_FS_Init 2 */

  /* USER CODE END USB_OTG_FS_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_DCX_GPIO_Port, LCD_DCX_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : USER_Btn_Pin */
  GPIO_InitStruct.Pin = USER_Btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : RMII_MDC_Pin RMII_RXD0_Pin RMII_RXD1_Pin */
  GPIO_InitStruct.Pin = RMII_MDC_Pin|RMII_RXD0_Pin|RMII_RXD1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : RMII_REF_CLK_Pin RMII_MDIO_Pin RMII_CRS_DV_Pin */
  GPIO_InitStruct.Pin = RMII_REF_CLK_Pin|RMII_MDIO_Pin|RMII_CRS_DV_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LCD_CS_Pin */
  GPIO_InitStruct.Pin = LCD_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(LCD_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : LCD_DCX_Pin */
  GPIO_InitStruct.Pin = LCD_DCX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(LCD_DCX_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : RMII_TXD1_Pin */
  GPIO_InitStruct.Pin = RMII_TXD1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(RMII_TXD1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LCD_RESET_Pin */
  GPIO_InitStruct.Pin = LCD_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_RESET_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : RMII_TX_EN_Pin RMII_TXD0_Pin */
  GPIO_InitStruct.Pin = RMII_TX_EN_Pin|RMII_TXD0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/**
 * @brief Initialize the tasks and starting the TIM2 timer.
 */
void initTasks(void) {
	/* TODO: consider creating a dedicated function to initialize the tasks,
	 *       the function will receive the address of the tasks array, and its size,
	 *       and will initialize it.
	 *       This will replace initializing each array specifically.
	 */
	printf("Starting initTasks task! \r\n");
	fflush(stdout);
	/* AMBULANCE TASK INIT */
	for(int i=0; i<AMBULANCE_TASKS; i++) {
		BaseType_t status;
		char taskName[configMAX_TASK_NAME_LEN];
		snprintf(taskName, configMAX_TASK_NAME_LEN, "Ambulance_%d", i);
	 	status = xTaskCreate(vHandleCall,
	 							taskName,
								TASKS_MEMORY_SIZE,
								(void*) 1,
								HANDLE_TASKS_PRIORITY,
								&vAmbulanceTasks[i]);
	 	if(status != pdTRUE) {
	 		/* TODO: handle this error */
	 		printf("Starting Ambulance Threads Failed! \r\n");
	 		fflush(stdout);
	 		return;
	 	}
	}
	printf("Started Ambulance Threads! \r\n");
	fflush(stdout);

	/* POLICE TASK INIT */
	for(int i=0; i<POLICE_TASKS; i++) {
		BaseType_t status;
		char taskName[configMAX_TASK_NAME_LEN];
		snprintf(taskName, configMAX_TASK_NAME_LEN, "Police_%d", i);
	 	status = xTaskCreate(vHandleCall,
	 							taskName,
								TASKS_MEMORY_SIZE,
								(void*) 1,
								HANDLE_TASKS_PRIORITY,
								&vPoliceTasks[i]);
	 	if(status != pdTRUE) {
	 		/* TODO: handle this error */
	 		return;
	 	}
	}
	printf("Started Police Threads! \r\n");
	fflush(stdout);
	/* FIRE DEP TASK INIT */
	for(int i=0; i<FIRE_TASKS; i++) {
		BaseType_t status;
		char taskName[configMAX_TASK_NAME_LEN];
		snprintf(taskName, configMAX_TASK_NAME_LEN, "Fire_%d", i);
	 	status = xTaskCreate(vHandleCall,
	 							taskName,
								TASKS_MEMORY_SIZE,
								(void*) 1,
								HANDLE_TASKS_PRIORITY,
								&vFireTasks[i]);
	 	if(status != pdTRUE) {
	 		/* TODO: handle this error */
	 		return;
	 	}
	}
	printf("Started Fire Dep Threads! \r\n");
	fflush(stdout);
	/* CORONA TASK INIT */
	for(int i=0; i<CORONA_TASKS; i++) {
		BaseType_t status;
		char taskName[configMAX_TASK_NAME_LEN];
		snprintf(taskName, configMAX_TASK_NAME_LEN, "Corona_%d", i);
	 	status = xTaskCreate(vHandleCall,
	 							taskName,
								TASKS_MEMORY_SIZE,
								(void*) 1,
								HANDLE_TASKS_PRIORITY,
								&vCoronaTasks[i]);
	 	if(status != pdTRUE) {
	 		/* TODO: handle this error */
	 		return;
	 	}
	}
	printf("Started Corona Threads! \r\n");
	fflush(stdout);

	BaseType_t status = xTaskCreate(vDispatcherCode,
							"Dispatcher_Task",
							TASKS_MEMORY_SIZE,
							(void*)1,
							HANDLE_TASKS_PRIORITY,
							&vDispatcherTask);
	if(status != pdTRUE) {
		return;
	}
	printf("Started Dispatcher Thread! \r\n");
	fflush(stdout);

	if(HAL_TIM_Base_Start_IT(&htim2) != HAL_OK) {
		Error_Handler();
	}

	printf("Timer interrupts enabled! \r\n");
	fflush(stdout);
	//vTaskDelay(100);
	//if(HAL_RNG_GenerateRandomNumber_IT(&hrng) != HAL_OK) {
	//	Error_Handler();
	//}
	//vTaskDelay(100);
	for(;;) {
		uint32_t ulNotificationValue;
		xTaskNotifyWait(0x00, 0x00, &ulNotificationValue, portMAX_DELAY);
	}
}

/**
 * @brief Initialize the queues.
 */
void initQueues(void) {
	/* TODO: Consider setting attributed for each queue */

	qAmbulance = xQueueCreate(TASKS_QUEUE_SIZE, sizeof(DispatcherPacket));
	if(qAmbulance == NULL) {
		/* TODO: handle this error */
		return;
	}
	qPolice = xQueueCreate(TASKS_QUEUE_SIZE, sizeof(DispatcherPacket));
	if(qPolice == NULL) {
		/* TODO: handle this error */
		return;
	}
	qFire = xQueueCreate(TASKS_QUEUE_SIZE, sizeof(DispatcherPacket));
	if(qFire == NULL) {
		/* TODO: handle this error */
		return;
	}
	qCorona = xQueueCreate(TASKS_QUEUE_SIZE, sizeof(DispatcherPacket));
	if(qCorona == NULL) {
		/* TODO: handle this error */
		return;
	}
	qDispatcher = xQueueCreate(TASKS_QUEUE_SIZE, sizeof(DispatcherPacket));
	if(qDispatcher == NULL) {
		/* TODO: handle this error */
		return;
	}
	/* TODO: log a message queues were initialized */

}


/**
 * @brief Handle call function for tasks.
 * @param pvParameters Pointer to the parameters passed to the task.
 */
void vHandleCall(void* pvParameters) {
	configASSERT(((uint32_t) pvParameters) == 1);
	uint32_t ulNotifictionValue;
	TickType_t startTick, endTick, totalTicks;
	/*
	 * TODO: log the task has been created before transitioning to BLOCKED state.
	 */
	//vTaskDelay(1000);
	//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
	for(;;) {
		/* TODO: log a message that this task is created and it's waiting for a job */

		xTaskNotifyWait(0x00, 0x00, &ulNotifictionValue, portMAX_DELAY);
		startTick = xTaskGetTickCount();
		/*
		 * PERFORM TASK
		 */
		if(xTasksDataMutex != NULL) {
			if(xSemaphoreTake(xTasksDataMutex, portMAX_DELAY) == pdTRUE) {
				total_tasks_ran++;
				endTick = xTaskGetTickCount();
				totalTicks = endTick - startTick;
				taskENTER_CRITICAL();
				total_tasks_time += (float)totalTicks/configTICK_RATE_HZ;
				taskEXIT_CRITICAL();
				xSemaphoreGive(xTasksDataMutex);
			} else {
				/*
				 * The mutex couldn't be obtained. Code should never get here.
				 */
			}
		}

	}
}

void HAL_RNG_ReadyDataCallback(RNG_HandleTypeDef *hrng, uint32_t random32bit)
{
  uint32_t randomPeriod = (random32bit % MAX_TIM2_IT_PERIOD) + MIN_TIM2_IT_PERIOD;

  // Update timer IT period
  __HAL_TIM_SET_AUTORELOAD(&htim2, randomPeriod * 2);
  __HAL_TIM_SET_COUNTER(&htim2, 0);

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);

}

/*
 * Display related functions
 */
void lcd_color_transfer_ready_cb(SPI_HandleTypeDef *hspi)
{
        /* CS high */
        HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
        lcd_bus_busy = 0;
        lv_display_flush_ready(lcd_disp);
}

/* Initialize LCD I/O bus, reset LCD */
static int32_t lcd_io_init(void)
{
        /* Register SPI Tx Complete Callback */
        HAL_SPI_RegisterCallback(&hspi1, HAL_SPI_TX_COMPLETE_CB_ID, lcd_color_transfer_ready_cb);

        /* reset LCD */
        HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_RESET);
        HAL_Delay(100);
        HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_SET);
        HAL_Delay(100);

        HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LCD_DCX_GPIO_Port, LCD_DCX_Pin, GPIO_PIN_SET);

        return HAL_OK;
}

/* Platform-specific implementation of the LCD send command function. In general this should use polling transfer. */
static void lcd_send_cmd(lv_display_t *disp, const uint8_t *cmd, size_t cmd_size, const uint8_t *param, size_t param_size)
{
        LV_UNUSED(disp);
        while (lcd_bus_busy);   /* wait until previous transfer is finished */
        /* Set the SPI in 8-bit mode */
        hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
        HAL_SPI_Init(&hspi1);
        /* DCX low (command) */
        HAL_GPIO_WritePin(LCD_DCX_GPIO_Port, LCD_DCX_Pin, GPIO_PIN_RESET);
        /* CS low */
        HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
        /* send command */
        if (HAL_SPI_Transmit(&hspi1, (uint8_t*)cmd, cmd_size, BUS_SPI1_POLL_TIMEOUT) == HAL_OK) {
                /* DCX high (data) */
                HAL_GPIO_WritePin(LCD_DCX_GPIO_Port, LCD_DCX_Pin, GPIO_PIN_SET);
                /* for short data blocks we use polling transfer */
                HAL_SPI_Transmit(&hspi1, (uint8_t *)param, (uint16_t)param_size, BUS_SPI1_POLL_TIMEOUT);
                /* CS high */
                HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
        }
}

/* Platform-specific implementation of the LCD send color function. For better performance this should use DMA transfer.
 * In case of a DMA transfer a callback must be installed to notify LVGL about the end of the transfer.
 */
static void lcd_send_color(lv_display_t *disp, const uint8_t *cmd, size_t cmd_size, uint8_t *param, size_t param_size)
{
        LV_UNUSED(disp);
        while (lcd_bus_busy);   /* wait until previous transfer is finished */
        /* Set the SPI in 8-bit mode */
        hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
        HAL_SPI_Init(&hspi1);
        /* DCX low (command) */
        HAL_GPIO_WritePin(LCD_DCX_GPIO_Port, LCD_DCX_Pin, GPIO_PIN_RESET);
        /* CS low */
        HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
        /* send command */
        if (HAL_SPI_Transmit(&hspi1, (uint8_t*)cmd, cmd_size, BUS_SPI1_POLL_TIMEOUT) == HAL_OK) {
                /* DCX high (data) */
                HAL_GPIO_WritePin(LCD_DCX_GPIO_Port, LCD_DCX_Pin, GPIO_PIN_SET);
                /* for color data use DMA transfer */
                /* Set the SPI in 16-bit mode to match endianness */
                hspi1.Init.DataSize = SPI_DATASIZE_16BIT;
                HAL_SPI_Init(&hspi1);
                lcd_bus_busy = 1;
                HAL_SPI_Transmit_DMA(&hspi1, param, (uint16_t)param_size / 2);
                /* NOTE: CS will be reset in the transfer ready callback */
        }
}

void LVGL_Task(void const *argument)
{
        /* Initialize LVGL */
        lv_init();

        /* Initialize LCD I/O */
        if (lcd_io_init() != 0)
                return;

        /* Create the LVGL display object and the LCD display driver */
        lcd_disp = lv_ili9341_create(LCD_H_RES, LCD_V_RES, LV_LCD_FLAG_NONE, lcd_send_cmd, lcd_send_color);
        lv_display_set_rotation(lcd_disp, LV_DISPLAY_ROTATION_270);

        /* Allocate draw buffers on the heap. In this example we use two partial buffers of 1/10th size of the screen */
        lv_color_t * buf1 = NULL;
        lv_color_t * buf2 = NULL;

        uint32_t buf_size = LCD_H_RES * LCD_V_RES / 10 * lv_color_format_get_size(lv_display_get_color_format(lcd_disp));

        buf1 = lv_malloc(buf_size);
        if(buf1 == NULL) {
                LV_LOG_ERROR("display draw buffer malloc failed");
                return;
        }

        buf2 = lv_malloc(buf_size);
        if(buf2 == NULL) {
                LV_LOG_ERROR("display buffer malloc failed");
                lv_free(buf1);
                return;
        }
        lv_display_set_buffers(lcd_disp, buf1, buf2, buf_size, LV_DISPLAY_RENDER_MODE_PARTIAL);

        ui_init(lcd_disp);

        for(;;) {
                /* The task running lv_timer_handler should have lower priority than that running `lv_tick_inc` */
                lv_timer_handler();
                /* raise the task priority of LVGL and/or reduce the handler period can improve the performance */
                vTaskDelay(10);
        }
}

void ui_init(lv_display_t *disp)
{
        lv_obj_t *obj;

        /* set screen background to white */
        lv_obj_t *scr = lv_screen_active();
        lv_obj_set_style_bg_color(scr, lv_color_white(), 0);
        lv_obj_set_style_bg_opa(scr, LV_OPA_100, 0);

        /* create label */
        obj = lv_label_create(scr);
        lv_obj_set_align(obj, LV_ALIGN_CENTER);
        lv_obj_set_height(obj, LV_SIZE_CONTENT);
        lv_obj_set_width(obj, LV_SIZE_CONTENT);
        lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, 0);
        lv_obj_set_style_text_color(obj, lv_color_black(), 0);
        lv_label_set_text(obj, "Hello World!");
}

/* USER CODE END 4 */



/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */
	if(htim->Instance == TIM2) {
		DispatcherPacket new_packet;
		generateDispatcherMSG(&new_packet);
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		UBaseType_t qItems = uxQueueMessagesWaitingFromISR( qDispatcher );
		UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark(vDispatcherTask);
		if( qItems != TASKS_QUEUE_SIZE) {
			xQueueSendFromISR(qDispatcher, &new_packet, NULL);
			xTaskNotifyFromISR(vDispatcherTask, 0x00, eNoAction, &xHigherPriorityTaskWoken);
			//printf("Added to queue! \r\n");
			//fflush(stdout);
		} else {
			xTaskNotifyFromISR(vDispatcherTask, 0x00, eNoAction, &xHigherPriorityTaskWoken);
		}
		if(xHigherPriorityTaskWoken == pdTRUE)
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

		/*
		 * TODO: add a random item to dispatcher queue.
		 */
	}
  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
