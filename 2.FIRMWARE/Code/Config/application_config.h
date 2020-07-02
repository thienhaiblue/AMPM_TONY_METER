
#ifndef APPLICATION_CONFIG_H_
#define APPLICATION_CONFIG_H_

#include <printf.h>
#include "stm32f0xx_hal.h"
#include "checksum.h"

#define USING_DEBUG (1)

#if(USING_DEBUG == 1)
#define	PRINTF			printf
#else
#define	PRINTF			(void)
#endif

#define INTERVAL_TASK_5MS 			(5)
#define INTERVAL_TASK_10MS 			(10)
#define INTERVAL_TASK_15MS 			(15)
#define INTERVAL_TASK_20MS 			(20)
#define INTERVAL_TASK_30MS 			(30)
#define INTERVAL_TASK_50MS 			(50)
#define INTERVAL_TASK_100MS 		(100)
#define INTERVAL_TASK_350MS			(350)
#define INTERVAL_TASK_500MS			(500)
#define INTERVAL_TASK_1S			(1000)
#define INTERVAL_TASK_2S			(2000)
#define INTERVAL_TASK_3S 			(3000)
#define INTERVAL_TASK_5S 			(5000)
#define INTERVAL_TASK_10S 			(10000)
#define INTERVAL_TASK_30S 			(30000)
#define INTERVAL_TASK_60S 			(60000)

#define ASSERT_NONVOID(x,y)			do{if(!x) { assert_param(false); return y;} }while(0)

#endif /* APPLICATION_CONFIG_H_ */
