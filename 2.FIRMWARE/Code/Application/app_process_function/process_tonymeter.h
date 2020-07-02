#ifndef APP_PROCESS_FUNCTION_PROCESS_TONYMETER_H_
#define APP_PROCESS_FUNCTION_PROCESS_TONYMETER_H_

#include "ringbuffer.h"
#include "tonymeter_process_function.h"
#include "tony_meter.h"
#include "platform_define.h"
/************************** Constant Definitions *****************************/
/**************************** Type Definitions *******************************/

typedef enum
{
	eAlert_Status_None	= 0,
	eAlert_Status_UnderVol,
	eAlert_Status_OverVol,
	eAlert_Status_OverCurrent,
	eAlert_Status_ResidualCurrent,
}Alert_status;

typedef enum
{
	eAlert_Channel_0 = 0,
	eAlert_Channel_1,
	eAlert_Channel_2,
	eAlert_Channel_3,
	eAlert_Channel_all,
}Alert_Channel;

typedef enum
{
	eAlert_Status_UnderVol_Bit = 0x00,
	eAlert_Status_OverVol_Bit = 0x01,
	eAlert_Status_OverCurrent_Bit = 0x02,
	eAlert_Status_ResidualCurrent_Bit = 0x04,
}Alert_StatusBit;

enum
{
    BIKE_IG_ON_SIG = USER_SIG,
	TONY_SEND_PING_SIG,
	TONY_READ_AC_PARAM_SIG,
	TONY_RECEIVE_GW_TIMESYNC_SIG,
	TONY_RECEIVE_GW_CONTROL_CP_SIG,
	TONY_WORKING_STATUS_SIG,
	TONY_ALERT_STATUS_TIMEOUT_SIG,
    TONY_INVALID_SIG = 0xFFU
};



typedef struct
{

    uint8_t				deferSig;
    bool				relayStatus[eAlert_Channel_all];
    Alert_status		alertStatus;
    Alert_Channel		alertChannel;
    uint32_t			errorCode[eAlert_Channel_all];
} tony_control_t;


/***************** Macros (Inline Functions) Definitions *********************/
/************************** Function Prototypes ******************************/
void process_tonymeter_init(void);


/************************** Variable Definitions *****************************/
extern tony_control_t ao_tony_meter;

extern sRingBufferHandle_Typedef sTonyMeterATCmdRingbuff;
/******************************************************************************/

#endif /* APP_PROCESS_FUNCTION_PROCESS_TONYMETER_H_ */
