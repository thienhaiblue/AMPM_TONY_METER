/*
 * console_config.c
 *
 *  Created on: 
 *      Author:
 */

#include <tonymeter_process_function.h>
#include "console_config.h"
#include "tonymeter_sparser.h"
uint8_t gUart_Dbg_Temp;
sRingBufferHandle_Typedef	sRingBuffer_Console_Handle;


//static uint32_t noInitVar[20]  __attribute__((section(".noinit")));



/*------------------------------------------------------  Declaring function for console feature ------------------------------------------------------*/







void console_putchar(uint8_t data)
{
	_putchar(data);
}


eRingBufferResult console_getchar(uint8_t* data)
{
	uint32_t tempData;
	if(ringBuffer_read(&sRingBuffer_Console_Handle, &tempData) != eResult_OK)
	{
		return eResult_Error;
	}
	*data = (uint8_t)tempData;
	return eResult_OK;
}

void  console_receiveData(void)
{
	HAL_UART_Receive_IT(&DBG_UART_HANDLE, (uint8_t*)&gUart_Dbg_Temp, 1);
}

void console_RXCallback(void)
{
 	assert_param(ringBuffer_write(&sRingBuffer_Console_Handle, gUart_Dbg_Temp) == eResult_OK);
	console_receiveData();
}


/*------------------------------------------------------  Declaring function  for console command handle ------------------------------------------------------*/
void help_callback(void *pConsole, int argc, char **argv)
{

//	uint16_t voltage, current,power, energy;
//	uint8_t IDBuffer[32];
//	bool channel_0, channel_1, channel_2, channel_3;

	printf("\r\n<------------------------------------------------------->");
	printf("\r\nATM Interface Application Console:");
	printf("\r\n\"info\" - return application version");
	printf("\r\n<------------------------------------------------------->");
	for(uint8_t index = 0; index < argc; ++index)
	{
		PRINTF("\r\n%s",argv[index]);
	}
	switch(atoi(argv[1]))
	{
	case 0:

	case 1:

		break;
	case 2:

		break;
	default:
		printf("\r\nThis case is not handle !!!");
		break;

	}
}


void setRelay_callback(void *pConsole, int argc, char **argv)
{
	TonyMeter_State relayState = eTonymeter_State_Disable;
	if(atoi(argv[1]) == 0)
	{
		relayState = eTonymeter_State_Disable;
	}
	else if(atoi(argv[1]) == 1)
	{
		relayState = eTonymeter_State_Enable;

	}

	for(uint8_t index = 0; index < TONYMETER_MAX_CHANNEL; ++index)
	{
		TonyMeter_SendCommandNow(&TonyMeter_SetRelayProcess[index][relayState]);
		ao_tony_meter.relayStatus[index] = (bool)relayState;
	}

}


void resetEnergy_callback(void *pConsole, int argc, char **argv)
{
	uint8_t channel = atoi(argv[1]);
	assert_param(channel < TONYMETER_MAX_CHANNEL);
	TonyMeter_SendCommandNow(&TonyMeter_ResetEnergyCounterProcess[channel]);
}


