/************************************************************************
Name: Hai Nguyen Van
Cellphone: (84) 97-8779-222
Mail:thienhaiblue@ampm.vn
----------------------------------
AMPM JSC
Add: 634/6 Phan Van Suu street , Ward 13, Tan Binh District, HCM City, VN
**************************************************************************/
#include <tonymeter_sparser.h>
#include "ringbuffer.h"
#include "application_config.h"
#include "tonymeter_at_cmd_parser.h"

extern  uint32_t TICK_Get(void);
extern sRingBufferHandle_Typedef 	sTonyMeterATCmdRingbuff;
extern sRingBufferHandle_Typedef	tonymeter_ringbuf;

void TonyMeter_CmdAddToList(list_t cmdList, TONYMETER_CMD_LIST_TYPE *cmd)
{
	list_add(cmdList, cmd);
}

void TonyMeter_CmdResetList(list_t cmdList)
{
	list_init(cmdList);
}


uint8_t TonyMeter_CmdCheckList_IsEmpty(list_t cmdList)
{
	if(cmdList[0] == NULL)
		return 1;
	return 0;
}

uint8_t TonyMeter_CmdCheck_IsEmpty(TONYMETER_CMD_PROCESS_TYPE *cmdSend)
{
	if(cmdSend->next == NULL)
	{
		return 1;
	}
	return 0;
}

uint8_t TonyMeter_CmdTask_IsIdle(TONYMETER_CMD_PHASE_TYPE ampmCmdPhase)
{
	switch(ampmCmdPhase)
	{
	case TONYMETER_CMD_ERROR:
	case TONYMETER_CMD_OK:
		return 1;
	default:
		return 0;
	}
}

uint8_t TonyMeter_SendCmd(TONYMETER_CMD_PHASE_TYPE *ampmCmdPhase, TONYMETER_CMD_PROCESS_TYPE *cmdSend,
		const TONYMETER_CMD_PROCESS_TYPE *cmd,
		Timeout_Type *taskTimeout
)
{
	if(TonyMeter_CmdTask_IsIdle(*ampmCmdPhase))
	{
		*cmdSend = *cmd;
		InitTimeout(taskTimeout, SYSTICK_TIME_MS(cmdSend->delay));
		return 1;
	}
	return 0;
}


void TonyMeter_SendCmdNow(const TONYMETER_CMD_PROCESS_TYPE *cmd)
{
	assert_param(ringBuffer_write(&sTonyMeterATCmdRingbuff, (uint32_t)cmd) == eResult_OK);
}

void TonyMeter_Cmd_Task(TONYMETER_CMD_PHASE_TYPE *ampmCmdPhase, TONYMETER_CMD_PROCESS_TYPE *cmdSend, list_t cmdList,Timeout_Type *taskTimeout)
{
	uint32_t cmdSend_Address;
	TONYMETER_CMD_LIST_TYPE *cmd;
	uint8_t res;

	switch(*ampmCmdPhase)
	{
	case TONYMETER_CMD_SEND:
		if(ringBuffer_read(&sTonyMeterATCmdRingbuff, &cmdSend_Address) == eResult_OK)
		{
			*cmdSend = *((TONYMETER_CMD_PROCESS_TYPE *)cmdSend_Address);
			InitTimeout(taskTimeout, SYSTICK_TIME_MS(cmdSend->delay));
			ringBuffer_flush(&tonymeter_ringbuf);
			if(cmdSend->send_data_callback != NULL)
			{
				cmdSend->send_data_callback();
				InitTimeout(taskTimeout, SYSTICK_TIME_MS(cmdSend->timeout));
				*ampmCmdPhase = TONYMETER_CMD_WAIT;
			}
			else
			{
				*ampmCmdPhase = TONYMETER_CMD_DELAY;
			}
		}
		else
		{
			*ampmCmdPhase = TONYMETER_CMD_OK;
		}
		break;
	case TONYMETER_CMD_RESEND:
		if (cmdSend->retryNum)
		{
			cmdSend->retryNum--;
			InitTimeout(taskTimeout, SYSTICK_TIME_MS(cmdSend->delay));
			*ampmCmdPhase = TONYMETER_CMD_SEND;
		}
		else
		{
			*ampmCmdPhase = TONYMETER_CMD_GOTO_ERROR;
		}
		break;
	case TONYMETER_CMD_GOTO_ERROR:
		if(cmdSend->end_data_callback)
		{
			if(cmdSend->end_data_callback(TONYMETER_CMD_ERROR))
			{
				if(*ampmCmdPhase == TONYMETER_CMD_GOTO_ERROR)
				{
					*ampmCmdPhase = TONYMETER_CMD_ERROR;
				}
			}
		}
		else
		{
			*ampmCmdPhase = TONYMETER_CMD_ERROR;
		}
		break;
	case TONYMETER_CMD_WAIT:
		if(cmdSend->recv_data_callback == NULL)
		{
			*ampmCmdPhase = TONYMETER_CMD_DELAY;
			break;
		}
		if(CheckTimeout(taskTimeout) == SYSTICK_TIMEOUT)
		{
			*ampmCmdPhase = TONYMETER_CMD_RESEND;
		}
		else
		{
			res = cmdSend->recv_data_callback();
			if(res == TONYMETER_CMD_DELAY)
			{
				InitTimeout(taskTimeout, SYSTICK_TIME_MS(cmdSend->delay));
				*ampmCmdPhase = TONYMETER_CMD_DELAY;
			}
			else if(res == TONYMETER_CMD_RESEND)
			{
				*ampmCmdPhase = TONYMETER_CMD_RESEND;
				InitTimeout(taskTimeout, SYSTICK_TIME_MS(cmdSend->delay));
			}
		}
		break;
	case TONYMETER_CMD_DELAY:
		if(CheckTimeout(taskTimeout) == SYSTICK_TIMEOUT)
		{
			if(cmdSend->end_data_callback)
			{
				if(cmdSend->end_data_callback(TONYMETER_CMD_OK))
				{
					if(*ampmCmdPhase == TONYMETER_CMD_DELAY)
					{
						*ampmCmdPhase = TONYMETER_CMD_OK;
					}
				}
			}
			else
			{
				*ampmCmdPhase = TONYMETER_CMD_OK;
			}
		}
		break;

	case TONYMETER_CMD_ERROR:
		break;
	case TONYMETER_CMD_OK:
		if(cmdSend->next)
		{

			*cmdSend = *((TONYMETER_CMD_PROCESS_TYPE *)cmdSend->next);
			InitTimeout(taskTimeout, SYSTICK_TIME_MS(cmdSend->delay));
			*ampmCmdPhase = TONYMETER_CMD_SEND;
			break;
		}
		if(cmdList[0] != NULL)
		{
			if(*ampmCmdPhase != TONYMETER_CMD_ERROR)
			{
				cmd = list_pop(cmdList);
				if(cmd != NULL)
				{
					*cmdSend = *cmd->pt;
					InitTimeout(taskTimeout, SYSTICK_TIME_MS(cmdSend->delay));
					*ampmCmdPhase = TONYMETER_CMD_SEND;
				}
			}
			else
			{
				list_init(cmdList);
			}
		}
		*ampmCmdPhase = TONYMETER_CMD_SEND;
		break;
	default:
		*ampmCmdPhase = TONYMETER_CMD_ERROR;
		break;
	}

	/* Random feedback process */
	AT_ComnandParser();
}

void StrParserInit(STR_PARSER_Type *process,uint8_t *numCharParsed,uint8_t strParserCntMax,uint16_t dataLengthMax)
{
	uint8_t i;
	process->state = STR_FINISH;
	process->dataRecvLength = 0;
	process->strParserCntMax = strParserCntMax;
	process->timeout = 0;
	process->timeoutSet = 10000;// 10sec
	process->dataRecvMaxLength = dataLengthMax;
	for(i = 0; i < process->strParserCntMax; i++)
	{
		numCharParsed[i] = 0;
	}
}

void StrParserCtl(uint32_t sysTimeMs,STR_PARSER_Type *process,uint8_t *numCharParsed)
{	
	uint8_t i;
	if(process->state == STR_FINISH)
	{
		process->timeout = sysTimeMs;
	}
	else
	{
		if(sysTimeMs- process->timeout >= process->timeoutSet)
		{
			process->timeout = sysTimeMs;
			process->state = STR_FINISH;
			for(i = 0; i < process->strParserCntMax;i++)
			{
				numCharParsed[i] = 0;
			}
		}
	}
}

void StrComnandParser(uint32_t sysTimeMs,const STR_INFO_Type *info,STR_PARSER_Type *process,uint8_t *numCharParsed,uint8_t c)
{
	uint32_t i; 
	static uint8_t lastChar = 0;
	StrParserCtl(sysTimeMs,process,numCharParsed);
	switch(process->state)
	{
	case STR_FINISH:
		for(i = 0; i < process->strParserCntMax;i++)
		{
			if(c == info[i].command[numCharParsed[i]] || info[i].command[numCharParsed[i]] == '\t')
			{
				lastChar = c;
				numCharParsed[i]++;
				if(info[i].command[numCharParsed[i]] == '\0')
				{
					process->strInProcess = i;
					process->state = STR_WAIT_FINISH;
					process->dataRecvLength = 0;
				}
			}
			else if(lastChar == info[i].command[0])
			{
				numCharParsed[i] = 1;
			}
			else
			{
				numCharParsed[i] = 0;
			}
		}
		break;
	case STR_WAIT_FINISH:
		if(info[process->strInProcess].callback(process->dataRecvLength,c) == 0)
		{
			process->state = STR_FINISH;
			//process->timeoutSet = 10000;
			for(i = 0; i < process->strParserCntMax;i++)
			{
				numCharParsed[i] = 0;
			}
		}
		process->dataRecvLength++;
		if(process->dataRecvLength >= process->dataRecvMaxLength)
		{
			process->state = STR_FINISH;
			for(i = 0; i <  process->strParserCntMax;i++)
			{
				numCharParsed[i] = 0;
			}
		}
		break;
	default:
		process->state = STR_FINISH;
		for(i = 0; i <  process->strParserCntMax;i++)
		{
			numCharParsed[i] = 0;
		}
		break;
	}	
}

