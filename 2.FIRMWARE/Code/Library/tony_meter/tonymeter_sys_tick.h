/************************************************************************
Name: Hai Nguyen Van
Cellphone: (84) 97-8779-222
Mail:thienhaiblue@ampm.vn
----------------------------------
AMPM JSC
Add: 634/6 Phan Van Suu street , Ward 13, Tan Binh District, HCM City, VN
**************************************************************************/
#ifndef __SYS_TICK_H__
#define __SYS_TICK_H__
#include <stdint.h>

#define SYSTICK_SECOND 			(1000)
#define SYSTICK_TIME_MS(x)		(x)
#define SYSTICK_TIME_SEC(x)		(x * SYSTICK_SECOND)

#define SYSTICK_TIMEOUT			0
#define SYSTICK_TIMEIN			1


typedef struct
{
	uint32_t start_time; 		
	uint32_t timeout;
	uint32_t crc; 
}Timeout_Type;

void		SysTick_Task(void);
uint32_t	SysTick_Get(void);
uint32_t	SysTick_Get64(void);
void 		SysTick_DelayMs(uint32_t ms);
void 		InitTimeout(Timeout_Type *t,uint32_t timeout);
uint32_t 	CheckTimeout(Timeout_Type *t);

#endif

