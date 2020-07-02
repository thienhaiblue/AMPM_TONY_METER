/************************************************************************
Name: Hai Nguyen Van
Cellphone: (84) 97-8779-222
Mail:thienhaiblue@ampm.vn 
----------------------------------
AMPM JSC
Add: 634/6 Phan Van Suu street , Ward 13, Tan Binh District, HCM City, VN
**************************************************************************/
#include <tonymeter_at_cmd_parser.h>
#include <tonymeter_sys_tick.h>
#include "tonymeter_process_function.h"


//static const STR_INFO_Type AT_ProcessCmd[] =
//{
//		/* random feedback */
//		"+OVERLOADALERT:0\r\n",
//};


uint8_t cmdCnt[sizeof(RandomFeedBack_CallBack)/sizeof(STR_INFO_Type)];
STR_PARSER_Type AT_CmdParser;




void AT_CmdProcessInit(void)
{
	StrParserInit(&AT_CmdParser,cmdCnt,STR_PARSER_COUNT(RandomFeedBack_CallBack), TONYMETER_ATCOMMAND_MAX_LENGTH);
}

void AT_ComnandParser()
{
	static uint32_t RingBuffInit = 0;
	uint32_t c;

	/* Int ring buff */
	if(RingBuffInit == 0)
	{
		RingBuffInit = 1;
		AT_CmdProcessInit();
	}

	if((TONYMETER_RANDOM_FEEDBACK_ENABLE) && (ringBuffer_read(&tonymeter_ringbuf_random_feedback, &c) == eResult_OK))
	{
		StrComnandParser(SysTick_Get(),RandomFeedBack_CallBack,&AT_CmdParser,cmdCnt,(uint8_t)c);
	}
}

