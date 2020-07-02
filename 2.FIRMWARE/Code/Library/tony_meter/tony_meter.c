/************************************************************************
Name: Hai Nguyen Van
Cellphone: (84) 97-8779-222
Mail:thienhaiblue@ampm.vn 
----------------------------------
AMPM JSC
Add: 634/6 Phan Van Suu street , Ward 13, Tan Binh District, HCM City, VN
**************************************************************************/
#include <tony_meter/tony_meter.h>
#include <tonymeter_at_cmd_parser.h>


sRingBufferHandle_Typedef					tonymeter_ringbuf;
sRingBufferHandle_Typedef					tonymeter_ringbuf_random_feedback;
uint8_t 									tonymeterRxData;

TONYMETER_CMD_PHASE_TYPE 					TonyMeter_CmdPhase;
TONYMETER_CMD_PROCESS_TYPE 					TonyMeter_CmdSend;
Timeout_Type 								TonyMeter_TaskTimeout;

static	TONYMETER_AT_CMD_PACKET_TYPE*		TonyMeter_AtCmd;
static	COMPARE_TYPE 						TonyMeter_CmdOk;
static	COMPARE_TYPE						TonyMeter_CmdFails;
static	STR_PARSER_Type 					TonyMeter_AtCallBackParser;
static	uint8_t 							TonyMeterAtCmdBuf[TONYMETER_ATCOMMAND_MAX_LENGTH];
static 	uint8_t 							TonyMeter_CmdParsedChar[1] 			=  {0};
static 	STR_INFO_Type 						TonyMeter_AtProcessCmd[1];
static	void*								TonyMeter_CmdList_list				= NULL;
list_t 										TonyMeter_CmdList 					= (list_t)&TonyMeter_CmdList_list;


HAL_StatusTypeDef tonymeter_init(void)
{
	HAL_StatusTypeDef stt = HAL_ERROR;

	/* Creating ringbuf for store data */
	assert_param(ringBuffer_init(&tonymeter_ringbuf, eRingBuffer_Type8bit, TONYMETER_RINGBUFFER_SIZE) == eResult_OK);
	assert_param(ringBuffer_init(&tonymeter_ringbuf_random_feedback, eRingBuffer_Type8bit, TONYMETER_RINGBUFFER_SIZE) == eResult_OK);
	ringBuffer_flush(&tonymeter_ringbuf);
	ringBuffer_flush(&tonymeter_ringbuf_random_feedback);

	__HAL_UART_FLUSH_DRREGISTER(&TONY_UART_HANDLE);

	/* First trigger to receive data */
	stt  = HAL_UART_Receive_IT(&TONY_UART_HANDLE, (uint8_t*) &tonymeterRxData, 1);

	assert_param(stt == HAL_OK);
	return stt;
}


void tonymeter_puts(uint8_t *string)
{
	assert_param(string != NULL);

	uint8_t	length = strlen((const char*)string);
#if (TONYMETER_DEBUG_ARG == true)
	printf("\r\n%s",string);
#endif

	HAL_UART_Transmit(&TONY_UART_HANDLE, (uint8_t*)string, length, TONYMETER_TRANSMIT_TIMEOUT_DEFAULT);
}


void tonymeterRXCallback(void)
{
	/* Save data to the ring buf */
	ringBuffer_write(&tonymeter_ringbuf, tonymeterRxData);
	ringBuffer_write(&tonymeter_ringbuf_random_feedback, tonymeterRxData);
	printf("%c",tonymeterRxData);
#if (TONYMETER_DEBUG_ARG == true)

#endif

	/* Continue trigger to receive data */
	HAL_UART_Receive_IT(&TONY_UART_HANDLE, (uint8_t*) &tonymeterRxData, 1);
}





uint8_t TonyMeter_SendAtCheck_IsEmpty(void)
{
	return TonyMeter_CmdCheck_IsEmpty(&TonyMeter_CmdSend);
}


uint8_t TonyMeter_AtCmdCheckResponse(uint8_t *str,uint32_t t)
{
	COMPARE_TYPE cmp;
	uint32_t tempData;
	uint8_t c;
	InitFindData(&cmp,str);

	while(t--)
	{
		SysTick_DelayMs(1);
		if(ringBuffer_read(&tonymeter_ringbuf, (uint32_t*) &tempData) == eResult_OK)
		{
			c = tempData;
			if(FindData(&cmp,c) == eResult_OK)
			{
				return eResult_OK;
			}
		}
	}
	return 0xff;
}

uint8_t TonyMeter_SendCommandBlocking(const uint8_t *resOk,const uint8_t *resFails,uint32_t timeout,uint8_t tryAgainNum,const uint8_t *format, ...)
{
	static  uint8_t  buffer[TONYMETER_ATCOMMAND_MAX_LENGTH];
	COMPARE_TYPE cmp1,cmp2;
	Timeout_Type tOut;
	uint8_t c;
	uint32_t tempData;
	va_list     vArgs;
	va_start(vArgs, format);
	vsprintf((char *)&buffer[0], (char const *)format, vArgs);
	va_end(vArgs);

	if (tryAgainNum == 0)
	{
		tryAgainNum = 1;
	}

	while (tryAgainNum)
	{
		SysTick_DelayMs(100);
		InitFindData(&cmp1, (uint8_t *)resOk);
		InitFindData(&cmp2, (uint8_t *)resFails);
		InitTimeout(&tOut, SYSTICK_TIME_MS(timeout));

		ringBuffer_flush(&tonymeter_ringbuf);
		tonymeter_puts(buffer);

		while(CheckTimeout(&tOut))
		{
			if(ringBuffer_read(&tonymeter_ringbuf,(uint32_t*) &tempData) == eResult_OK)
			{
				c = tempData;
				if(FindData(&cmp1, c) == eResult_OK)
				{
					return 0;
				}
				if(FindData(&cmp2, c) == eResult_OK)
				{
					break;
				}
			}
		}
		tryAgainNum--;
	}
	return 0xff;
}

uint8_t TonyMeter_SetCmdOK(TONYMETER_CMD_PHASE_TYPE phase)
{
	TonyMeter_CmdPhase = TONYMETER_CMD_OK;
	return 0;
}



void TonyMeter_SendCommandNow(const TONYMETER_CMD_PROCESS_TYPE *cmd)
{
	TonyMeter_SendCmdNow(cmd);
}

void TonyMeter_Start(void)
{
	TonyMeter_CmdPhase = TONYMETER_CMD_SEND;
}


uint32_t TonyMeter_SendCallbackDefault(void)
{
	TonyMeter_AtCmd = (TONYMETER_AT_CMD_PACKET_TYPE *)TonyMeter_CmdSend.commandPacketStruct;
	ringBuffer_flush(&tonymeter_ringbuf);

	if(TonyMeter_AtCmd->sprintf)
	{
		TonyMeter_AtCmd->sprintf(TonyMeterAtCmdBuf);
		TonyMeterAtCmdBuf[TONYMETER_ATCOMMAND_MAX_LENGTH - 1] = 0;
		tonymeter_puts(TonyMeterAtCmdBuf);
	}
	else
	{
		tonymeter_puts(TonyMeter_AtCmd->command);
	}

	InitFindData(&TonyMeter_CmdOk, (uint8_t *)TonyMeter_AtCmd->resOk);
	InitFindData(&TonyMeter_CmdFails, (uint8_t *)TonyMeter_AtCmd->resFails);

	if((TonyMeter_AtCmd->callback != NULL) && (TonyMeter_AtCmd->response != NULL))
	{
		StrParserInit(&TonyMeter_AtCallBackParser,TonyMeter_CmdParsedChar,1,TONYMETER_ATCOMMAND_MAX_LENGTH);
		TonyMeter_AtCallBackParser.timeoutSet = TonyMeter_CmdSend.timeout;
		TonyMeter_AtProcessCmd[0].callback  = TonyMeter_AtCmd->callback;
		TonyMeter_AtProcessCmd[0].command  = TonyMeter_AtCmd->response;
	}
	return 1;
}

uint32_t TonyMeter_RecvCallbackDefault(void)
{
	uint32_t tempData;
	uint8_t c;
	while (ringBuffer_read(&tonymeter_ringbuf,(uint32_t*) &tempData) == eResult_OK)
	{
		c = tempData;
		if((TonyMeter_AtCmd->callback != NULL) && (TonyMeter_AtCmd->response != NULL))
		{
			StrComnandParser(SysTick_Get(),TonyMeter_AtProcessCmd,&TonyMeter_AtCallBackParser,TonyMeter_CmdParsedChar,c);
		}
		if (FindData(&TonyMeter_CmdOk, c) == 0)
		{
			return TONYMETER_CMD_DELAY;
		}
		else if (FindData(&TonyMeter_CmdFails, c) == 0)
		{
			return TONYMETER_CMD_RESEND;
		}
	}
	return TONYMETER_CMD_WAIT;
}





