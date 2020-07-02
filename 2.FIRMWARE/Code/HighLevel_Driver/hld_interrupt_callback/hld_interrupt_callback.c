
#include <tonymeter_sys_tick.h>
#include "hld_interrupt_callback.h"
#include "application_config.h"

extern uint8_t userTxData;

/* ISR for USART Receive Complete */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	/* DBG_UART_HANDLE RX Complete Callback */
	if(huart->Instance == DBG_UART_HANDLE.Instance)
	{
		console_RXCallback();
		return;
	}

	/* TONY_UART_HANDLE RX Complete Callback */
	if(huart->Instance == TONY_UART_HANDLE.Instance)
	{
		tonymeterRXCallback();
		return;
	}
}


/* ISR for ADC Converison Complete */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	HLD_ADC_ConversionDone();
}

/* ISR for SysTick_Handler 1ms */
void Timer1msCallBack(void)
{
	SysTick_Task();
}



