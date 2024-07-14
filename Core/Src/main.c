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

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart3;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

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

// TASKS Status Arrays and their
bool bAmbTasksStatus[AMBULANCE_TASKS] = {false};
SemaphoreHandle_t AmbTasksStatusMutex;
bool bPolTasksStatus[POLICE_TASKS] = {false};
SemaphoreHandle_t PolTasksStatusMutex;
bool bFireTasksStatus[FIRE_TASKS] = {false};
SemaphoreHandle_t FireTasksStatusMutex;
bool bCorTasksStatus[CORONA_TASKS] = {false};
SemaphoreHandle_t CorTasksStatusMutex;

// Print to console mutex
SemaphoreHandle_t printfMutex;

// TASKS Data Objects
float total_tasks_time = 0;
uint32_t total_tasks_ran = 0;
float average_task_time = 0.0f;
SemaphoreHandle_t xTasksDataMutex;
SemaphoreHandle_t xPrintfMutex;



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
static void MX_RNG_Init(void);
static void MX_TIM2_Init(void);

/* USER CODE BEGIN PFP */
void vHandleCall(void* pvParameters);
void initQueues(void);
void initTasks(void);
void initSemaphores(void);
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
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_RNG_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  initSemaphores();
  initQueues();
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */

  /* USER CODE BEGIN RTOS_THREADS */
  BaseType_t status;
  status = xTaskCreate((TaskFunction_t)initTasks,
		  "init_task",
		  configMINIMAL_STACK_SIZE,
		  (void*) 1,
		  configMAX_PRIORITIES,
		  &vInitTaskHandle);
  configASSERT(status ==  pdTRUE);
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
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
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

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

  /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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
void initTasks(void) {
	/* TODO: consider creating a dedicated function to initialize the tasks,
	 *       the function will receive the address of the tasks array, and its size,
	 *       and will initialize it.
	 *       This will replace initializing each array specifically.
	 */
	//portENTER_CRITICAL();
	if(xSemaphoreTake(printfMutex, portMAX_DELAY) == pdTRUE) {
		portENTER_CRITICAL();
		printf("Starting initTasks task! \r\n");
		fflush(stdout);
		xSemaphoreGive(printfMutex);
		portEXIT_CRITICAL();
	}
	/* AMBULANCE TASK INIT */
	for(int i=0; i<AMBULANCE_TASKS; i++) {
		taskInit_t* ambulance_taskInit = (taskInit_t*)pvPortMalloc(sizeof(taskInit_t));
		configASSERT(ambulance_taskInit != NULL);

		ambulance_taskInit->department = AMBULANCE;
		ambulance_taskInit->taskIdentifier = i;
		ambulance_taskInit->pQhandler = &qAmbulance;
		ambulance_taskInit->pSemHandler = &AmbTasksStatusMutex;

		BaseType_t status;
		char taskName[configMAX_TASK_NAME_LEN];
		snprintf(taskName, configMAX_TASK_NAME_LEN, "Ambulance_%d", i);
	 	status = xTaskCreate(vHandleCall,
	 							taskName,
								TASKS_MEMORY_SIZE,
								(void*)ambulance_taskInit,
								HANDLE_TASKS_PRIORITY,
								&vAmbulanceTasks[i]);
	 	if(status != pdTRUE) {
	 		/* TODO: handle this error */
	 		vPortFree(ambulance_taskInit);
	 		printf("Starting Ambulance Threads Failed! \r\n");
	 		fflush(stdout);
	 		return;
	 	}
	}
	if(xSemaphoreTake(printfMutex, portMAX_DELAY) == pdTRUE) {
		portENTER_CRITICAL();
		printf("Started Ambulance Threads! \r\n");
		fflush(stdout);
		xSemaphoreGive(printfMutex);
		portEXIT_CRITICAL();
	}


	/* POLICE TASK INIT */
	for(int i=0; i<POLICE_TASKS; i++) {
		taskInit_t* police_taskInit = (taskInit_t*)pvPortMalloc(sizeof(taskInit_t));
		configASSERT(police_taskInit != NULL);

		police_taskInit->department = POLICE;
		police_taskInit->taskIdentifier = i;
		police_taskInit->pQhandler = &qPolice;
		police_taskInit->pSemHandler = &PolTasksStatusMutex;
		BaseType_t status;
		char taskName[configMAX_TASK_NAME_LEN];
		snprintf(taskName, configMAX_TASK_NAME_LEN, "Police_%d", i);
	 	status = xTaskCreate(vHandleCall,
	 							taskName,
								TASKS_MEMORY_SIZE,
								(void*) police_taskInit,
								HANDLE_TASKS_PRIORITY,
								&vPoliceTasks[i]);
	 	if(status != pdTRUE) {
	 		/* TODO: handle this error */
	 		return;
	 	}
	}
	if(xSemaphoreTake(printfMutex, portMAX_DELAY) == pdTRUE) {
		portENTER_CRITICAL();
		printf("Started Police Threads! \r\n");
		fflush(stdout);
		xSemaphoreGive(printfMutex);
		portEXIT_CRITICAL();
	}

	/* FIRE DEP TASK INIT */
	for(int i=0; i<FIRE_TASKS; i++) {
		taskInit_t* fire_taskInit = (taskInit_t*)pvPortMalloc(sizeof(taskInit_t));
		configASSERT(fire_taskInit != NULL);

		fire_taskInit->department = FIRE;
		fire_taskInit->taskIdentifier = i;
		fire_taskInit->pQhandler = &qFire;
		fire_taskInit->pSemHandler = &FireTasksStatusMutex;
		BaseType_t status;
		char taskName[configMAX_TASK_NAME_LEN];
		snprintf(taskName, configMAX_TASK_NAME_LEN, "Fire_%d", i);
	 	status = xTaskCreate(vHandleCall,
	 							taskName,
								TASKS_MEMORY_SIZE,
								(void*) fire_taskInit,
								HANDLE_TASKS_PRIORITY,
								&vFireTasks[i]);
	 	if(status != pdTRUE) {
	 		/* TODO: handle this error */
	 		return;
	 	}
	}
	if(xSemaphoreTake(printfMutex, portMAX_DELAY) == pdTRUE) {
		printf("Started Fire Dep Threads! \r\n");
		fflush(stdout);
		xSemaphoreGive(printfMutex);
	}

	/* CORONA TASK INIT */
	for(int i=0; i<CORONA_TASKS; i++) {
		taskInit_t* corona_taskInit = (taskInit_t*)pvPortMalloc(sizeof(taskInit_t));
		configASSERT(corona_taskInit != NULL);

		corona_taskInit->department = CORONA;
		corona_taskInit->taskIdentifier = i;
		corona_taskInit->pQhandler = &qCorona;
		corona_taskInit->pSemHandler = &CorTasksStatusMutex;
		BaseType_t status;
		char taskName[configMAX_TASK_NAME_LEN];
		snprintf(taskName, configMAX_TASK_NAME_LEN, "Corona_%d", i);
	 	status = xTaskCreate(vHandleCall,
	 							taskName,
								TASKS_MEMORY_SIZE,
								(void*) corona_taskInit,
								HANDLE_TASKS_PRIORITY,
								&vCoronaTasks[i]);
	 	if(status != pdTRUE) {
	 		/* TODO: handle this error */
	 		return;
	 	}
	}
	if(xSemaphoreTake(printfMutex, portMAX_DELAY) == pdTRUE) {
		printf("Started Corona Threads! \r\n");
		fflush(stdout);
		xSemaphoreGive(printfMutex);
	}

	vTaskDelay(100);

	BaseType_t status = xTaskCreate(vDispatcherCode,
							"Dispatcher_Task",
							TASKS_MEMORY_SIZE,
							(void*)1,
							HANDLE_TASKS_PRIORITY,
							&vDispatcherTask);
	if(status != pdTRUE) {
		return;
	}
	if(xSemaphoreTake(printfMutex, portMAX_DELAY) == pdTRUE) {
		printf("Started Dispatcher Thread! \r\n");
		fflush(stdout);
		xSemaphoreGive(printfMutex);
	}


	if(HAL_TIM_Base_Start_IT(&htim2) != HAL_OK) {
		Error_Handler();
	}
	if(xSemaphoreTake(printfMutex, portMAX_DELAY) == pdTRUE) {
		printf("Timer interrupts enabled! \r\n");
		fflush(stdout);
		xSemaphoreGive(printfMutex);
	}

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


void vHandleCall(void* pvParameters) {
	uint32_t ulNotifictionValue;
	TickType_t startTick, endTick, totalTicks;
	taskInit_t* pTaskInit = (taskInit_t*)pvParameters;
	/*
	 * TODO: log the task has been created before transitioning to BLOCKED state.
	 */

	char printMSG[100];
	snprintf(printMSG, 100, "New %s task created! \r\nTask index is %d \r\n"
			,GET_ENUM_DEPARTMENT_STR(pTaskInit->department)
			, pTaskInit->taskIdentifier);
	if(xSemaphoreTake(printfMutex, portMAX_DELAY) == pdTRUE) {
		portENTER_CRITICAL();
		printf(printMSG);
		fflush(stdout);
		xSemaphoreGive(printfMutex);
		portEXIT_CRITICAL();
	}
	for(;;) {
		/* TODO: log a message that this task is created and it's waiting for a job */

		xTaskNotifyWait(0x00, 0x00, &ulNotifictionValue, portMAX_DELAY);
		// Get starting time tick count
		startTick = xTaskGetTickCount();
		// Get packet from the queue handler
		DispatcherPacket new_packet;
		xQueueReceive(*(pTaskInit->pQhandler), &new_packet, portMAX_DELAY);

		// Wait for the job to get done
		vTaskDelayUntil(&startTick, new_packet.timeToHandleInTicks);

		// Update shared app data
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
		// Indicate the task is now available for the next incoming packet.
		if(pTaskInit->pSemHandler != NULL) {
			if(xSemaphoreTake(*(pTaskInit->pSemHandler), portMAX_DELAY)) {
				bAmbTasksStatus[pTaskInit->taskIdentifier] = true;
			}
		}
	}
}


void initSemaphores(void) {
	printfMutex = xSemaphoreCreateMutex();
	xTasksDataMutex = xSemaphoreCreateMutex();
	xPrintfMutex = xSemaphoreCreateMutex();
	AmbTasksStatusMutex = xSemaphoreCreateMutex();
	PolTasksStatusMutex = xSemaphoreCreateMutex();
	FireTasksStatusMutex = xSemaphoreCreateMutex();
	CorTasksStatusMutex = xSemaphoreCreateMutex();
}


void HAL_RNG_ReadyDataCallback(RNG_HandleTypeDef *hrng, uint32_t random32bit)
{
  uint32_t randomPeriod = (random32bit % MAX_TIM2_IT_PERIOD) + MIN_TIM2_IT_PERIOD;

  // Update timer IT period
  __HAL_TIM_SET_AUTORELOAD(&htim2, randomPeriod * 2);
  __HAL_TIM_SET_COUNTER(&htim2, 0);

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);

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
		//UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark(vDispatcherTask);
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
