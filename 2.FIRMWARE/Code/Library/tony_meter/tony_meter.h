/************************************************************************
Name: Hai Nguyen Van
Cellphone: (84) 97-8779-222
Mail:thienhaiblue@ampm.vn
----------------------------------
AMPM JSC
Add: 634/6 Phan Van Suu street , Ward 13, Tan Binh District, HCM City, VN
**************************************************************************/
#ifndef __COMM_H__
#define __COMM_H__
#include 	<stdint.h>
#include 	<stdio.h>
#include 	<string.h>
#include	"hardware_config.h"
#include 	"application_config.h"
#include 	"ringbuffer.h"
#include 	<stdlib.h>
#include	<stdarg.h>
#include 	<stdio.h>
#include 	<tony_meter/tony_meter.h>
#include 	<tonymeter_data_cmp.h>
#include <tonymeter_process_function.h>
#include 	<tonymeter_sparser.h>
#include 	<tonymeter_sprintf.h>



extern	sRingBufferHandle_Typedef	tonymeter_ringbuf;
extern	sRingBufferHandle_Typedef	tonymeter_ringbuf_random_feedback;
extern	uint8_t 					tonymeterRxData;
extern	const uint8_t*				OK_String;
extern	const uint8_t*				Error_String;
extern 	list_t 						TonyMeter_CmdList;
extern 	Timeout_Type 				TonyMeter_TaskTimeout;
extern 	TONYMETER_CMD_PHASE_TYPE	TonyMeter_CmdPhase;
extern 	TONYMETER_CMD_PROCESS_TYPE	TonyMeter_CmdSend;
extern 	uint8_t*					TonyMeter_GprsApn;




HAL_StatusTypeDef 					tonymeter_init(void);
void 								tonymeter_puts(uint8_t *string);
void								tonymeterRXCallback(void);

uint8_t								TonyMeter_AtCmdCheckResponse(uint8_t *str,uint32_t t);
uint8_t 							TonyMeter_SetCmdOK(TONYMETER_CMD_PHASE_TYPE phase);
uint32_t 							TonyMeter_SendCallbackDefault(void);
uint32_t 							TonyMeter_RecvCallbackDefault(void);
void 								TonyMeter_SendCommandNow(const TONYMETER_CMD_PROCESS_TYPE *cmd);
uint8_t 							TonyMeter_SendAtCheck_IsEmpty(void);
void 								TonyMeter_Start(void);

/* Send command to Tonymeter in blocking mode to check return value */
uint8_t 							TonyMeter_SendCommandBlocking(const uint8_t *resOk,const uint8_t *resFails,uint32_t timeout,uint8_t tryAgainNum,const uint8_t *format, ...);



#endif
