/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define WHEEL_LED5_Pin GPIO_PIN_5
#define WHEEL_LED5_GPIO_Port GPIOA
#define WHEEL_LED4_Pin GPIO_PIN_6
#define WHEEL_LED4_GPIO_Port GPIOA
#define WHEEL_LED3_Pin GPIO_PIN_7
#define WHEEL_LED3_GPIO_Port GPIOA
#define DEBUG_LED_Pin GPIO_PIN_0
#define DEBUG_LED_GPIO_Port GPIOB
#define WHEEL_LED2_Pin GPIO_PIN_8
#define WHEEL_LED2_GPIO_Port GPIOA
#define WHEEL_LED1_Pin GPIO_PIN_9
#define WHEEL_LED1_GPIO_Port GPIOA
#define DASH_ENABLE_Pin GPIO_PIN_4
#define DASH_ENABLE_GPIO_Port GPIOB
#define WHEEL_BUTTON3_Pin GPIO_PIN_5
#define WHEEL_BUTTON3_GPIO_Port GPIOB
#define WHEEL_BUTTON3_EXTI_IRQn EXTI9_5_IRQn
#define WHEEL_BUTTON2_Pin GPIO_PIN_6
#define WHEEL_BUTTON2_GPIO_Port GPIOB
#define WHEEL_BUTTON2_EXTI_IRQn EXTI9_5_IRQn
#define WHEEL_BUTTON1_Pin GPIO_PIN_7
#define WHEEL_BUTTON1_GPIO_Port GPIOB
#define WHEEL_BUTTON1_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
