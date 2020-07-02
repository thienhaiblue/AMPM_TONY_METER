
#ifndef HARDWARE_CONFIG_H_
#define HARDWARE_CONFIG_H_

#include "adc.h"
#include "can.h"
#include "crc.h"
#include "gpio.h"
#include "iwdg.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"


/* Define Hardwares Module HandleTypedef */

#define		TONY_UART_HANDLE		huart4

#define		DBG_UART_HANDLE			huart2

#define 	CRC_HANDLE				hcrc

#define		SPI_HANDLE				hspi1


#define		ADC_HANDLE				hadc

#define		WATCHDOG_HANDLE			hiwdg

#define 	TIMER_DELAY_HANDLE		htim2



#endif /* HARDWARE_CONFIG_H_ */
