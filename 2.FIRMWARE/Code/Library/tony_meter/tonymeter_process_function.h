/************************************************************************
Name: Hai Nguyen Van
Cellphone: (84) 97-8779-222
Mail:thienhaiblue@ampm.vn
----------------------------------
AMPM JSC
Add: 634/6 Phan Van Suu street , Ward 13, Tan Binh District, HCM City, VN
**************************************************************************/
#ifndef TONY_METER_TONYMETER_PROCESS_FUNCTION_H_
#define TONY_METER_TONYMETER_PROCESS_FUNCTION_H_

#include "application_config.h"
#include "hardware_config.h"
#include "tonymeter_sparser.h"
#include "tony_meter.h"
#include "process_tonymeter.h"



#define		TONYMETER_ATCOMMAND_MAX_LENGTH					(128)
#define		TONYMETER_AT_TEMP_BUFFER						(32)
#define 	TONYMETER_ATCOMMAND_MAX_ARGUMENT				(10)
#define 	TONYMETER_RINGBUFFER_SIZE						(TONYMETER_ATCOMMAND_MAX_LENGTH + TONYMETER_AT_TEMP_BUFFER)
#define		TONYMETER_MAX_CHANNEL							(4)
#define		TONYMETER_TRANSMIT_TIMEOUT_DEFAULT				(200)
#define 	TONYMETER_RECEIVE_TIMEOUT_DEFAULT				(500)
#define 	TONYMETER_DEBUG_ARG								(true)
#define		TONYMETER_TRY_AGAIN_NUMBER_DEFAULT				(2)
#define		TONYMETER_MAX_RANDOM_FEEDBACK					(8)
#define		TONYMETER_RANDOM_FEEDBACK_ENABLE				(1)
#define		TONYMETER_NOLOAD_ENABLE							(0)
#define		TONYMETER_RESIDUAL_CURRENT_PROTECT_ENABLE		(0)

typedef struct
{
	uint8_t*			command;
	uint8_t(*sprintf)	(uint8_t *buf);
	uint8_t*			response;
	uint32_t 			(*callback)(uint16_t cnt,uint8_t c);
	uint8_t*			resOk;
	uint8_t*			resFails;
}TONYMETER_AT_CMD_PACKET_TYPE;



typedef enum
{
	eTonymeter_State_Disable	=	0,
	eTonymeter_State_Enable,
	eTonymeter_State_Max,
}TonyMeter_State;
#define 	RELAY_JTV1AS_PA_12V 				(1)

#define		TONY_RETURN_OK_AT_CMD				"OK\r\n"
#define		TONY_RETURN_ERROR_AT_CMD			"ERROR\r\n"


#define		TONY_READ_ID_AT_CMD					"AT+ID?\r\n"

#define		TONY_READ_AC0_AT_CMD				"AT+READ?0\r\n"
#define		TONY_READ_AC1_AT_CMD				"AT+READ?1\r\n"
#define		TONY_READ_AC2_AT_CMD				"AT+READ?2\r\n"
#define		TONY_READ_AC3_AT_CMD				"AT+READ?3\r\n"

#define		TONY_RESET_ENERGY0_AT_CMD			"AT+RESETWH=0\r\n"
#define		TONY_RESET_ENERGY1_AT_CMD			"AT+RESETWH=1\r\n"
#define		TONY_RESET_ENERGY2_AT_CMD			"AT+RESETWH=2\r\n"
#define		TONY_RESET_ENERGY3_AT_CMD			"AT+RESETWH=3\r\n"

#define		TONY_SET_RELAY00_AT_CMD				"AT+RELAY=0,0\r\n"
#define		TONY_SET_RELAY01_AT_CMD				"AT+RELAY=0,1\r\n"
#define		TONY_SET_RELAY10_AT_CMD				"AT+RELAY=1,0\r\n"
#define		TONY_SET_RELAY11_AT_CMD				"AT+RELAY=1,1\r\n"
#define		TONY_SET_RELAY20_AT_CMD				"AT+RELAY=2,0\r\n"
#define		TONY_SET_RELAY21_AT_CMD				"AT+RELAY=2,1\r\n"
#define		TONY_SET_RELAY30_AT_CMD				"AT+RELAY=3,0\r\n"
#define		TONY_SET_RELAY31_AT_CMD				"AT+RELAY=3,1\r\n"

#define		TONY_ENABLE00_AT_CMD				"AT+ENABLE=0,0\r\n"
#define		TONY_ENABLE01_AT_CMD				"AT+ENABLE=0,1\r\n"
#define		TONY_ENABLE10_AT_CMD				"AT+ENABLE=1,0\r\n"
#define		TONY_ENABLE11_AT_CMD				"AT+ENABLE=1,1\r\n"
#define		TONY_ENABLE20_AT_CMD				"AT+ENABLE=2,0\r\n"
#define		TONY_ENABLE21_AT_CMD				"AT+ENABLE=2,1\r\n"
#define		TONY_ENABLE30_AT_CMD				"AT+ENABLE=3,0\r\n"
#define		TONY_ENABLE31_AT_CMD				"AT+ENABLE=3,1\r\n"

 /* 
 * AT+RELAYPINS=<channel>,<coil type>,<pin 1>[,<pin 2>]\r\n

 * For example: 
	------------------------------------------------------------------------
 	if turn relay with
 		channel = 0, coil type = 0, pin 1 = 12
	then 
		#define		TONY_SET_RELAYPIN0_AT_CMD	"AT+RELAYPINS=0,0,12\r\n"
	------------------------------------------------------------------------
 	if turn relay with
 		channel = 0, coil type = 2, pin 1 = 12, pin 2 = 13
	then 
		#define		TONY_SET_RELAYPIN0_AT_CMD	"AT+RELAYPINS=0,2,12,13\r\n"

 */
#define		TONY_SET_RELAYPIN0_AT_CMD			"AT+RELAYPINS=0,2,12,13\r\n"
#define		TONY_SET_RELAYPIN1_AT_CMD			"AT+RELAYPINS=1,2,10,11\r\n"
#define		TONY_SET_RELAYPIN2_AT_CMD			"AT+RELAYPINS=2,2,8,9\r\n"
#define		TONY_SET_RELAYPIN3_AT_CMD			"AT+RELAYPINS=3,2,6,7\r\n"

#define		TONY_SET_ADC0_AT_CMD				"AT+ADC=0,3,0\r\n"
#define		TONY_SET_ADC1_AT_CMD				"AT+ADC=1,2,0\r\n"
#define		TONY_SET_ADC2_AT_CMD				"AT+ADC=2,1,0\r\n"
#define		TONY_SET_ADC3_AT_CMD				"AT+ADC=3,0,0\r\n"

#define 	TONY_RELAY_POLARITY0_AT_CMD			"AT+POLARITY=0,1\r\n"
#define 	TONY_RELAY_POLARITY1_AT_CMD			"AT+POLARITY=1,1\r\n"
#define 	TONY_RELAY_POLARITY2_AT_CMD			"AT+POLARITY=2,1\r\n"
#define 	TONY_RELAY_POLARITY3_AT_CMD			"AT+POLARITY=3,1\r\n"


 /* 
 * "AT+ONDELAY=<CHANNEL>,<DELAY*6> 
 * For example if turn relay channel 0 with delay is 8ms then 

 #define	TONY_ONDELAY0_AT_CMD				"AT+ONDELAY=0,48\r\n"

 */
#if(RELAY_JTV1AS_PA_12V == 1)
#define		TONY_ONDELAY0_AT_CMD				"AT+ONDELAY=0,12\r\n"	/* 2ms */
#define		TONY_ONDELAY1_AT_CMD				"AT+ONDELAY=1,12\r\n"	/* 2ms */
#define		TONY_ONDELAY2_AT_CMD				"AT+ONDELAY=2,12\r\n"	/* 2ms */
#define		TONY_ONDELAY3_AT_CMD				"AT+ONDELAY=3,12\r\n"	/* 2ms */

#define 	TONY_OFFDELAY0_AT_CMD				"AT+OFFDELAY=0,12\r\n"	/* 2ms */
#define 	TONY_OFFDELAY1_AT_CMD				"AT+OFFDELAY=1,12\r\n"	/* 2ms */
#define 	TONY_OFFDELAY2_AT_CMD				"AT+OFFDELAY=2,12\r\n"	/* 2ms */
#define 	TONY_OFFDELAY3_AT_CMD				"AT+OFFDELAY=3,12\r\n"	/* 2ms */
#endif


 /* 
 * AT+OVERLOAD=<channel>,<current in mA>,<delay time in ms>\r\n

 * For example: 
	------------------------------------------------------------------------
 	if turn relay with
 		channel = 0, current = 5000mA, delay = 500ms
	then 
 		#define	TONY_OVERLOAD0_AT_CMD				"AT+OVERLOAD=0,5000,500\r\n"
	------------------------------------------------------------------------
 */

#define		TONY_OVERLOAD0_AT_CMD				"AT+OVERLOAD=0,5000,500\r\n"
#define		TONY_OVERLOAD1_AT_CMD				"AT+OVERLOAD=1,5000,500\r\n"
#define		TONY_OVERLOAD2_AT_CMD				"AT+OVERLOAD=2,5000,500\r\n"
#define		TONY_OVERLOAD3_AT_CMD				"AT+OVERLOAD=3,5000,500\r\n"


 /* 
 * AT+NOLOAD=<channel>,<current in mA>,<delay time in ms>\r\n

 * For example: 
	------------------------------------------------------------------------
 	if turn relay with
 		channel = 0, current = 5mA, delay = 500ms
	then 
 		#define	TONY_NOLOAD0_AT_CMD				"AT+NOLOAD=0,5,500\r\n"
	------------------------------------------------------------------------
 */
#define		TONY_NOLOAD0_AT_CMD					"AT+NOLOAD=0,5,500\r\n"
#define		TONY_NOLOAD1_AT_CMD					"AT+NOLOAD=1,5,500\r\n"
#define		TONY_NOLOAD2_AT_CMD					"AT+NOLOAD=2,5,500\r\n"
#define		TONY_NOLOAD3_AT_CMD					"AT+NOLOAD=3,5,500\r\n"

/* 
 * AT+UNDERVOLT=<threshold in V *100>,<delay time in ms>\r\n

 * For example: 
	------------------------------------------------------------------------
 	if turn relay with
 		threshold = 176V, delay = 500ms
	then 
 		#define	TONY_UNDERVOLT_AT_CMD				"AT+UNDERVOLT=17600,500\r\n"
	------------------------------------------------------------------------
 */ 

#define		TONY_UNDERVOLT_AT_CMD				"AT+UNDERVOLT=17600,500\r\n"

/* 
 * AT+OVERVOLT=<threshold in V *100>,<delay time in ms>\r\n

 * For example: 
	------------------------------------------------------------------------
 	if turn relay with
 		threshold = 176V, delay = 500ms
	then 
 		#define	TONY_OVERVOLT_AT_CMD				"AT+OVERVOLT=25000,500\r\n"
	------------------------------------------------------------------------
 */ 
#define		TONY_OVERVOLT_AT_CMD				"AT+OVERVOLT=25000,500\r\n"

/* 
 * AT+RESDETECT=<channel>,<adc pin>,<gain>,<thresh>\r\n

 * For example: 
	------------------------------------------------------------------------
 	if turn relay with
 		channel = 0, adc pin = 0, gain = 2200, thresh = 300
	then 
 		#define		TONY_RESIDUAL_CURRENT0_AT_CMD		"AT+RESDETECT=0,0,2200,300\r\n"
	------------------------------------------------------------------------
 */ 
#define		TONY_RESIDUAL_CURRENT0_AT_CMD		"AT+RESDETECT=0,0,2200,300\r\n"
#define		TONY_RESIDUAL_CURRENT1_AT_CMD		"AT+RESDETECT=1,1,2200,300\r\n"
#define		TONY_RESIDUAL_CURRENT2_AT_CMD		"AT+RESDETECT=2,2,2200,300\r\n"
#define		TONY_RESIDUAL_CURRENT3_AT_CMD		"AT+RESDETECT=3,3,2200,300\r\n"

#define		TONY_READ_RESIDUAL0_AT_CMD			"AT+RESIDUAL?0\r\n"
#define		TONY_READ_RESIDUAL1_AT_CMD			"AT+RESIDUAL?1\r\n"
#define		TONY_READ_RESIDUAL2_AT_CMD			"AT+RESIDUAL?2\r\n"
#define		TONY_READ_RESIDUAL3_AT_CMD			"AT+RESIDUAL?3\r\n"


extern 		const 	uint8_t* 	TONY_READ_AC_AT_CMD_ARR[TONYMETER_MAX_CHANNEL];
extern		const 	uint8_t* 	TONY_RESET_ENERGY_AT_CMD_ARR[TONYMETER_MAX_CHANNEL];
extern		const 	uint8_t*	TONY_SET_RELAY_AT_CMD_ARR[TONYMETER_MAX_CHANNEL][2];
extern		const 	uint8_t* 	TONY_ENABLE_AT_CMD_ARR[TONYMETER_MAX_CHANNEL][2];
extern		const	uint8_t* 	TONY_SET_RELAYPIN_AT_CMD_ARR[TONYMETER_MAX_CHANNEL];
extern		const 	uint8_t* 	TONY_SET_ADC_AT_CMD_ARR[TONYMETER_MAX_CHANNEL];
extern		const	uint8_t* 	TONY_RELAY_POLARITY_AT_CMD_ARR[TONYMETER_MAX_CHANNEL];
extern		const 	uint8_t* 	TONY_ONDELAY_AT_CMD_ARR[TONYMETER_MAX_CHANNEL];
extern		const 	uint8_t* 	TONY_OFFDELAY_AT_CMD_ARR[TONYMETER_MAX_CHANNEL];
extern		const 	uint8_t* 	TONY_OVERLOAD_AT_CMD_ARR[TONYMETER_MAX_CHANNEL];
extern 		const 	uint8_t* 	TONY_NOLOAD_AT_CMD_ARR[TONYMETER_MAX_CHANNEL];
extern 		const 	uint8_t* 	TONY_UNDERVOL_AT_CMD_ARR[1];
extern		const 	uint8_t* 	TONY_OVERVOLT_AT_CMD_ARR[1];
extern 		const 	uint8_t* 	TONY_RESIDUAL_CURRENT_AT_CMD_ARR[TONYMETER_MAX_CHANNEL];



extern const 	TONYMETER_CMD_PROCESS_TYPE		TonyMeter_ReadIDProcess[1];
extern const 	TONYMETER_CMD_PROCESS_TYPE		TonyMeter_ResetEnergyCounterProcess[TONYMETER_MAX_CHANNEL];
extern const	TONYMETER_CMD_PROCESS_TYPE		TonyMeter_ReadACProcess[TONYMETER_MAX_CHANNEL];
extern const 	TONYMETER_CMD_PROCESS_TYPE 		TonyMeter_ReadResidualCurrentProcess[TONYMETER_MAX_CHANNEL];
extern const 	TONYMETER_CMD_PROCESS_TYPE		TonyMeter_SetRelayPolarityProcess[TONYMETER_MAX_CHANNEL];
extern const 	TONYMETER_CMD_PROCESS_TYPE		TonyMeter_SetRelayTimeOnDelayProcess[TONYMETER_MAX_CHANNEL];
extern const 	TONYMETER_CMD_PROCESS_TYPE		TonyMeter_SetRelayTimeOffDelayProcess[TONYMETER_MAX_CHANNEL];
extern const 	TONYMETER_CMD_PROCESS_TYPE		TonyMeter_SetOverLoadProtectProcess[TONYMETER_MAX_CHANNEL];
extern const 	TONYMETER_CMD_PROCESS_TYPE		TonyMeter_SetOverLoadTimeProtectProcess[TONYMETER_MAX_CHANNEL];


extern const TONYMETER_CMD_PROCESS_TYPE		TonyMeter_SetRelayProcess[TONYMETER_MAX_CHANNEL][2];
extern const TONYMETER_CMD_PROCESS_TYPE		TonyMeter_SetChannelStatusProcess[TONYMETER_MAX_CHANNEL][2];

extern const STR_INFO_Type RandomFeedBack_CallBack[TONYMETER_MAX_RANDOM_FEEDBACK];



/* Define TonyMeter AT Command Callback */
uint32_t TonyMeterGetID_Callback(uint16_t cnt,uint8_t c);
uint32_t TonyMeterReadAC_Callback(uint16_t cnt,uint8_t c);
uint32_t TonyMeterReadResidualCurrent_Callback(uint16_t cnt,uint8_t c);
uint32_t OverCurrent_Alert_Callback(uint16_t cnt,uint8_t c);
uint32_t UnderVolt_Alert_Callback(uint16_t cnt,uint8_t c);
uint32_t OverVolt_Alert_Callback(uint16_t cnt,uint8_t c);
uint32_t Residual_Current_Alert_Callback(uint16_t cnt,uint8_t c);
uint32_t No_Load_Alert_Callback(uint16_t cnt,uint8_t c);
uint32_t Timeout_Alert_Callback(uint16_t cnt,uint8_t c);
#endif /* TONY_METER_TONYMETER_PROCESS_FUNCTION_H_ */
