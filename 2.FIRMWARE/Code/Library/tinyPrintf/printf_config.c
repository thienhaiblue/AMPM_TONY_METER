/*
 * printf_config.c
 *
 *  Created on:
 *      Author:
 */


#include <printf_config.h>

#if (PRINTF_USING_BUFFER == 1)
static sRingBufferHandle_Typedef sRingBuffer_PrintfTX;
#endif

#if (USING_DEBUG == 1)
void _putchar(char character)
{
#if (PRINTF_USING_BUFFER == 1)
	assert_param(ringBuffer_write(&sRingBuffer_PrintfTX, character) == eResult_OK);
#else
	assert_param(HAL_UART_Transmit(&DBG_UART_HANDLE, (uint8_t *)&character, 1, 100) == HAL_OK);
#endif
}
#endif


void printf_init(void)
{
#if (PRINTF_USING_BUFFER == 1)
	assert_param(ringBuffer_init(&sRingBuffer_PrintfTX, eRingBuffer_Type8bit, RINGBUFFER_PRINTF_TX_MAX_BYTE) == eResult_OK);
#endif
}



void printf_process(void)
{
#if (PRINTF_USING_BUFFER == 1)
	uint32_t data;
	while(ringBuffer_read(&sRingBuffer_PrintfTX,(uint32_t*) &data) == eResult_OK)
	{
		assert_param(HAL_UART_Transmit(&DBG_UART_HANDLE, (uint8_t *)&data, 1, 100) == HAL_OK);
	}
#endif
}
