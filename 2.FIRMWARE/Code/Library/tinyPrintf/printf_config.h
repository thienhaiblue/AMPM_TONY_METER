/*
 * printf_config.h
 *
 *  Created on:
 *      Author:
 */

#ifndef TINYPRINTF_PRINTF_CONFIG_H_
#define TINYPRINTF_PRINTF_CONFIG_H_

#include <printf.h>
#include "stm32f0xx_hal.h"
#include "application_config.h"
#include "usart.h"
#include "ringbuffer.h"



#define PRINTF_USING_BUFFER					(1)

#if (PRINTF_USING_BUFFER == 1)
#define RINGBUFFER_PRINTF_TX_MAX_BYTE		(2048)
#endif



void printf_init(void);
void printf_process(void);

#endif /* TINYPRINTF_PRINTF_CONFIG_H_ */
