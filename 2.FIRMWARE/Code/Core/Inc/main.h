/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "stdbool.h"
#include "memory.h"

#include "hardware_config.h"
#include "application_config.h"
#include "can.h"
#include "usart.h"
#include "spi.h"
#include "adc.h"
#include "tim.h"
#include "crc.h"
#include "iwdg.h"
#include "printf.h"
#include "printf_config.h"
#include "console_serial_api.h"

#include "time.h"
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
extern bool gApplicationInitDone;

#define	MCU_MAX_GPIO_OUTPUT					12
extern const GPIO_TypeDef* MCU_GPIO_OUTPUT_PORT[MCU_MAX_GPIO_OUTPUT];
extern const uint16_t MCU_GPIO_OUTPUT_PIN[MCU_MAX_GPIO_OUTPUT];
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MCU_INPUT2__Pin GPIO_PIN_13
#define MCU_INPUT2__GPIO_Port GPIOC
#define MCU_RFID_PD_Pin GPIO_PIN_0
#define MCU_RFID_PD_GPIO_Port GPIOC
#define MCU_RFID_IRQ_Pin GPIO_PIN_1
#define MCU_RFID_IRQ_GPIO_Port GPIOC
#define MCU_RFID_MISO_Pin GPIO_PIN_2
#define MCU_RFID_MISO_GPIO_Port GPIOC
#define MCU_RFID_MOSI_Pin GPIO_PIN_3
#define MCU_RFID_MOSI_GPIO_Port GPIOC
#define MCU_TONY_UART_TXD_Pin GPIO_PIN_0
#define MCU_TONY_UART_TXD_GPIO_Port GPIOA
#define MCU_TONY_UART_RXD_Pin GPIO_PIN_1
#define MCU_TONY_UART_RXD_GPIO_Port GPIOA
#define MCU_UART_DEBUG_TXD_Pin GPIO_PIN_2
#define MCU_UART_DEBUG_TXD_GPIO_Port GPIOA
#define MCU_UART_DEBUG_RXD_Pin GPIO_PIN_3
#define MCU_UART_DEBUG_RXD_GPIO_Port GPIOA
#define MCU_SPI_CS_Pin GPIO_PIN_4
#define MCU_SPI_CS_GPIO_Port GPIOA
#define MCU_SPI_SCK_Pin GPIO_PIN_5
#define MCU_SPI_SCK_GPIO_Port GPIOA
#define MCU_SPI_MISO_Pin GPIO_PIN_6
#define MCU_SPI_MISO_GPIO_Port GPIOA
#define MCU_SPI_MOSI_Pin GPIO_PIN_7
#define MCU_SPI_MOSI_GPIO_Port GPIOA
#define ADC_MCU_WAKE_SCU_Pin GPIO_PIN_4
#define ADC_MCU_WAKE_SCU_GPIO_Port GPIOC
#define MCU_WAKE_SCU_Pin GPIO_PIN_5
#define MCU_WAKE_SCU_GPIO_Port GPIOC
#define ADC_MCU_ANALOG_Pin GPIO_PIN_0
#define ADC_MCU_ANALOG_GPIO_Port GPIOB
#define ADC_MCU_TEMP_SENSOR_Pin GPIO_PIN_1
#define ADC_MCU_TEMP_SENSOR_GPIO_Port GPIOB
#define MCU_TRUSTB_1_WIRE_Pin GPIO_PIN_2
#define MCU_TRUSTB_1_WIRE_GPIO_Port GPIOB
#define MCU_OUTPUT6_Pin GPIO_PIN_10
#define MCU_OUTPUT6_GPIO_Port GPIOB
#define TRUST_PWR_EN__Pin GPIO_PIN_11
#define TRUST_PWR_EN__GPIO_Port GPIOB
#define MCU_RFID_CS__Pin GPIO_PIN_12
#define MCU_RFID_CS__GPIO_Port GPIOB
#define MCU_RFID_SCK_Pin GPIO_PIN_13
#define MCU_RFID_SCK_GPIO_Port GPIOB
#define MCU_OUTPUT7_Pin GPIO_PIN_14
#define MCU_OUTPUT7_GPIO_Port GPIOB
#define MCU_OUTPUT8_Pin GPIO_PIN_15
#define MCU_OUTPUT8_GPIO_Port GPIOB
#define MCU_IR4_UART_TXD_Pin GPIO_PIN_6
#define MCU_IR4_UART_TXD_GPIO_Port GPIOC
#define MCU_IR4_UART_RXD_Pin GPIO_PIN_7
#define MCU_IR4_UART_RXD_GPIO_Port GPIOC
#define MCU_OUTPUT10_Pin GPIO_PIN_8
#define MCU_OUTPUT10_GPIO_Port GPIOC
#define MCU_OUTPUT9_Pin GPIO_PIN_9
#define MCU_OUTPUT9_GPIO_Port GPIOC
#define MCU_OUTPUT1_Pin GPIO_PIN_8
#define MCU_OUTPUT1_GPIO_Port GPIOA
#define MCU_IR3_UART_TXD_Pin GPIO_PIN_9
#define MCU_IR3_UART_TXD_GPIO_Port GPIOA
#define MCU_IR3_UART_RXD_Pin GPIO_PIN_10
#define MCU_IR3_UART_RXD_GPIO_Port GPIOA
#define MCU_CAN_RX_Pin GPIO_PIN_11
#define MCU_CAN_RX_GPIO_Port GPIOA
#define MCU_CAN_TX_Pin GPIO_PIN_12
#define MCU_CAN_TX_GPIO_Port GPIOA
#define MCU_SWDIO_Pin GPIO_PIN_13
#define MCU_SWDIO_GPIO_Port GPIOA
#define MCU_SWCLK_Pin GPIO_PIN_14
#define MCU_SWCLK_GPIO_Port GPIOA
#define MCU_IO_CAN_STB_Pin GPIO_PIN_15
#define MCU_IO_CAN_STB_GPIO_Port GPIOA
#define MCU_IR1_UART_TXD_Pin GPIO_PIN_10
#define MCU_IR1_UART_TXD_GPIO_Port GPIOC
#define MCU_IR1_UART_RXD_Pin GPIO_PIN_11
#define MCU_IR1_UART_RXD_GPIO_Port GPIOC
#define MCU_OUTPUT11_Pin GPIO_PIN_12
#define MCU_OUTPUT11_GPIO_Port GPIOC
#define MCU_OUTPUT12_Pin GPIO_PIN_2
#define MCU_OUTPUT12_GPIO_Port GPIOD
#define MCU_IR2_UART_TXD_Pin GPIO_PIN_3
#define MCU_IR2_UART_TXD_GPIO_Port GPIOB
#define MCU_IR2_UART_RXD_Pin GPIO_PIN_4
#define MCU_IR2_UART_RXD_GPIO_Port GPIOB
#define MCU_OUTPUT5_Pin GPIO_PIN_5
#define MCU_OUTPUT5_GPIO_Port GPIOB
#define MCU_INPUT1__Pin GPIO_PIN_6
#define MCU_INPUT1__GPIO_Port GPIOB
#define MCU_OUTPUT4_Pin GPIO_PIN_7
#define MCU_OUTPUT4_GPIO_Port GPIOB
#define MCU_OUTPUT3_Pin GPIO_PIN_8
#define MCU_OUTPUT3_GPIO_Port GPIOB
#define MCU_OUTPUT2_Pin GPIO_PIN_9
#define MCU_OUTPUT2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
