
#include "hld_delayus.h"


void HLD_TimerDelayUs(uint32_t delayUs)
{
	__HAL_TIM_SET_COUNTER(&htim2, 0);

	HAL_TIM_Base_Start(&htim2);

	while(__HAL_TIM_GET_COUNTER(&htim2) < delayUs);

	HAL_TIM_Base_Stop(&htim2);
}
