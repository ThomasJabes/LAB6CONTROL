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
#include <string.h> // Para usar funciones como strcmp

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
UART_HandleTypeDef huart1;  // USART1 para comunicación con Arduino (PA9/TX y PA10/RX)
UART_HandleTypeDef huart2;  // USART2 para comunicación con la terminal

// Buffer para almacenar la cadena recibida
uint8_t received_data[20];  // Buffer de recepción de datos
uint8_t received_index = 0; // Índice para el buffer de recepción
uint8_t received_char;      // Carácter recibido individualmente

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();  // Inicialización del HAL
  SystemClock_Config();  // Configuración del reloj del sistema
  MX_GPIO_Init();  // Inicialización de los GPIO
  MX_USART1_UART_Init();  // Inicialización de UART1 para comunicación con Arduino
  MX_USART2_UART_Init();  // Inicialización de UART2 para comunicación con la terminal

  /* USER CODE BEGIN 2 */
  // Iniciar la recepción de datos por interrupciones en USART1
  HAL_UART_Receive_IT(&huart1, &received_char, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    // El código principal se queda en un ciclo infinito,
    // esperando que se reciban datos a través de las interrupciones UART.
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                              | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;  // Comunicación con Arduino
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2;  // Comunicación con la terminal
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Callback para recibir datos completos por UART
  * @param huart: estructura UART_HandleTypeDef
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  // Verifica que sea el UART correcto (USART1)
  if (huart->Instance == USART1) {
    // Almacenar el carácter recibido en el buffer
    received_data[received_index++] = received_char;

    // Verifica si se ha recibido el carácter de nueva línea ('\n')
    if (received_char == '\n') {
      received_data[received_index] = '\0';  // Termina la cadena con NULL
      received_index = 0;  // Reiniciar índice para la próxima cadena

      // Comparar la cadena recibida con los posibles valores enviados desde el Arduino
      if (strcmp((char*)received_data, "Izquierda\n") == 0) {
        HAL_UART_Transmit(&huart2, (uint8_t*)"Izquierda recibida\n", 19, HAL_MAX_DELAY);
      }
      else if (strcmp((char*)received_data, "Derecha\n") == 0) {
        HAL_UART_Transmit(&huart2, (uint8_t*)"Derecha recibida\n", 17, HAL_MAX_DELAY);
      }
      else if (strcmp((char*)received_data, "Abajo\n") == 0) {
        HAL_UART_Transmit(&huart2, (uint8_t*)"Abajo recibido\n", 15, HAL_MAX_DELAY);
      }
      else if (strcmp((char*)received_data, "Arriba\n") == 0) {
        HAL_UART_Transmit(&huart2, (uint8_t*)"Arriba recibido\n", 16, HAL_MAX_DELAY);
      }
      else if (strcmp((char*)received_data, "A\n") == 0) {
        HAL_UART_Transmit(&huart2, (uint8_t*)"X recibido\n", 11, HAL_MAX_DELAY);
      }
      else if (strcmp((char*)received_data, "B\n") == 0) {
        HAL_UART_Transmit(&huart2, (uint8_t*)"Y recibido\n", 11, HAL_MAX_DELAY);
      }

      // Reinicia el buffer para recibir más datos
      memset(received_data, 0, sizeof(received_data));
    }

    // Reinicia la recepción de datos por interrupciones en USART1
    HAL_UART_Receive_IT(&huart1, &received_char, 1);
  }
}

/* USER CODE BEGIN 4 */

/**
  * @brief Callback de error de UART
  * @param huart: estructura UART_HandleTypeDef
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  // Manejar errores si es necesario
  if (huart->Instance == USART1)
  {
    // Error en USART1
  }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
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
