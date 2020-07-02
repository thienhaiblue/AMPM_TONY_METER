
#ifndef HLD_ADC_HLD_ADC_H_
#define HLD_ADC_HLD_ADC_H_

#include "adc.h"

#define	HLD_ADCStatusTypeDef	HAL_StatusTypeDef

typedef enum
{
	eADC_MCU_ANALOG_RANK		= 0,
	eADC_MCU_TEMP_SENSOR_RANK,
	eADC_MCU_WAKE_SCU_RANK,
	eADC_MCU_OUTPUT_RANK,
	eADC_MCU_RANK_TOTAL
}eADC_Channel_Rank;


HLD_ADCStatusTypeDef HLD_ADC_Init(void);

HLD_ADCStatusTypeDef HLD_ADC_StartConversion();

uint16_t HLD_ADC_GetValue(eADC_Channel_Rank eChannel);

void HLD_ADC_ConversionDone(void);

bool HLD_ADC_IsConversionDone(void);

#endif /* HLD_ADC_HLD_ADC_H_ */
