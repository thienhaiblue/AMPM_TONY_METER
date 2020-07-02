
#include "hld_rcc.h"


void HLD_RCC_CheckResetCause(void)
{
	PRINTF("\r\nSystem Reset Cause: ");
	if(__HAL_RCC_GET_FLAG(RCC_FLAG_OBLRST) != RESET)
	{
		PRINTF("Option Byte Load !!!");
	}
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST) != RESET)
	{
		PRINTF("POR/PDR !!!");
	}
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST) != RESET)
	{
		PRINTF("Software !!!");
	}
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST) != RESET)
	{
		PRINTF("Independent Watchdog !!!");
	}
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST) != RESET)
	{
		PRINTF("Window Watchdog !!!");
	}
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_LPWRRST) != RESET)
	{
		PRINTF("Low Power !!!");
	}
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST) != RESET)
	{
		PRINTF("Pin reset !!!");
	}
	else
	{
		PRINTF("Unkown !!!");
	}

	/* Clear reset flags in any case */
	__HAL_RCC_CLEAR_RESET_FLAGS();
}
