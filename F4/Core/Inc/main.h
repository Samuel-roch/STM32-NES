/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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
extern I2C_HandleTypeDef  hi2c2;
extern LTDC_HandleTypeDef hltdc;
extern SPI_HandleTypeDef hspi5;

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LORA_CS_Pin GPIO_PIN_6
#define LORA_CS_GPIO_Port GPIOF
#define TOUCH_RST_Pin GPIO_PIN_2
#define TOUCH_RST_GPIO_Port GPIOH
#define TOUCH_INT_Pin GPIO_PIN_3
#define TOUCH_INT_GPIO_Port GPIOH
#define LORA_DIO_0_Pin GPIO_PIN_15
#define LORA_DIO_0_GPIO_Port GPIOI
#define LORA_DIO_0_EXTI_IRQn EXTI15_10_IRQn
#define LORA_DIO_1_Pin GPIO_PIN_0
#define LORA_DIO_1_GPIO_Port GPIOJ
#define LORA_DIO_1_EXTI_IRQn EXTI0_IRQn
#define LORA_RESET_Pin GPIO_PIN_1
#define LORA_RESET_GPIO_Port GPIOJ
#define LED_1_Pin GPIO_PIN_2
#define LED_1_GPIO_Port GPIOG
#define LED_2_Pin GPIO_PIN_3
#define LED_2_GPIO_Port GPIOG
#define LED_3_Pin GPIO_PIN_6
#define LED_3_GPIO_Port GPIOC
/* USER CODE BEGIN Private defines */

#define TOUCH_PORT hi2c2
#define LORA_SPI hspi5

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
