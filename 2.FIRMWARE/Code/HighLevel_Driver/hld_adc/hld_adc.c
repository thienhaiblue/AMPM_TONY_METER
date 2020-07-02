
#include "hld_adc.h"

static uint16_t gadcData[eADC_MCU_RANK_TOTAL];
static bool	convert_done = false;

HLD_ADCStatusTypeDef HLD_ADC_Init(void)
{
	HLD_ADCStatusTypeDef status;
	status = HAL_ADCEx_Calibration_Start(&ADC_HANDLE);
	status = HAL_ADC_Start_DMA(&ADC_HANDLE, (uint32_t*) gadcData, eADC_MCU_RANK_TOTAL);
	return  status;
}


HLD_ADCStatusTypeDef HLD_ADC_StartConversion()
{
	if(convert_done == true)
	{
		convert_done = false;
	}
	return HAL_ADC_Start_DMA(&ADC_HANDLE, (uint32_t*) gadcData, eADC_MCU_RANK_TOTAL);
}


uint16_t HLD_ADC_GetValue(eADC_Channel_Rank eChannel)
{
	assert_param(eChannel < eADC_MCU_RANK_TOTAL);
	if(convert_done == true)
	{
		convert_done = false;
	}
	return gadcData[eChannel];
}


void HLD_ADC_ConversionDone(void)
{
	convert_done = true;
}


bool HLD_ADC_IsConversionDone(void)
{
	return convert_done;
}
