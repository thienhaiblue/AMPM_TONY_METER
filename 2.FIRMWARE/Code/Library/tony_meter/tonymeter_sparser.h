/************************************************************************
Name: Hai Nguyen Van
Cellphone: (84) 97-8779-222
Mail:thienhaiblue@ampm.vn
----------------------------------
AMPM JSC
Add: 634/6 Phan Van Suu street , Ward 13, Tan Binh District, HCM City, VN
**************************************************************************/

#ifndef __SPARSER_H__
#define __SPARSER_H__
#include <linked_list/linked_list.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <tonymeter_sys_tick.h>





typedef struct
{
	uint8_t *command;
	uint32_t (*callback)(uint16_t cnt,uint8_t c);
} STR_INFO_Type;

#define STR_PARSER_COUNT(x)   (sizeof (x) / sizeof (STR_INFO_Type))


typedef enum
{
	TONYMETER_CMD_SEND = 0,
	TONYMETER_CMD_RESEND,
	TONYMETER_CMD_WAIT,
	TONYMETER_CMD_DELAY,
	TONYMETER_CMD_GOTO_ERROR,
	TONYMETER_CMD_ERROR,
	TONYMETER_CMD_OK
}TONYMETER_CMD_PHASE_TYPE;

typedef struct
{
	void*		next;
	void*		commandPacketStruct;
	uint32_t 	(*send_data_callback)(void);
	uint32_t 	(*recv_data_callback)(void);
	uint8_t  	(*end_data_callback)(TONYMETER_CMD_PHASE_TYPE phase);
	uint8_t 	retryNum;
	uint16_t 	timeout;	/*	cmd timeout in ms	*/
	uint16_t 	delay;		/*	delay when retry in ms	*/
}TONYMETER_CMD_PROCESS_TYPE;

typedef struct
{
	struct TONYMETER_CMD_LIST_TYPE *next;
	TONYMETER_CMD_PROCESS_TYPE *pt;
}TONYMETER_CMD_LIST_TYPE;


typedef enum
{
	STR_NEW_STATE,
	STR_WAIT_FINISH,
	STR_FINISH
}STR_STATE_Type;


typedef struct
{
	STR_STATE_Type state;
	uint16_t dataRecvLength;
	uint16_t dataRecvMaxLength;
	uint8_t strParserCntMax;
	uint8_t strInProcess;
	uint32_t timeout;
	uint32_t timeoutSet;
} STR_PARSER_Type;


void StrParserInit(STR_PARSER_Type *process,uint8_t *numCharParsed,uint8_t strParserCntMax,uint16_t dataLengthMax);
void StrComnandParser(uint32_t sysTimeMs,const STR_INFO_Type *info,STR_PARSER_Type *process,uint8_t *numCharParsed,uint8_t c);
void TonyMeter_Cmd_Task(
		TONYMETER_CMD_PHASE_TYPE *ampmCmdPhase,
		TONYMETER_CMD_PROCESS_TYPE *cmdSend,
		list_t cmdList,
		Timeout_Type *taskTimeout
);
uint8_t TonyMeter_SendCmd(
		TONYMETER_CMD_PHASE_TYPE *ampmCmdPhase,
		TONYMETER_CMD_PROCESS_TYPE *cmdSend,
		const TONYMETER_CMD_PROCESS_TYPE *cmd,
		Timeout_Type *taskTimeout
);

void TonyMeter_SendCmdNow(const TONYMETER_CMD_PROCESS_TYPE *cmd);

uint8_t TonyMeter_CmdTask_IsIdle(TONYMETER_CMD_PHASE_TYPE ampmCmdPhase);
void TonyMeter_CmdAddToList(list_t cmdList, TONYMETER_CMD_LIST_TYPE *cmd);
void TonyMeter_CmdResetList(list_t cmdList);
uint8_t TonyMeter_CmdCheckList_IsEmpty(list_t cmdList);
uint8_t TonyMeter_CmdCheck_IsEmpty(TONYMETER_CMD_PROCESS_TYPE *cmdSend);

#endif
