/************************************************************************
Name: Hai Nguyen Van
Cellphone: (84) 97-8779-222
Mail:thienhaiblue@ampm.vn 
----------------------------------
AMPM JSC
Add: 634/6 Phan Van Suu street , Ward 13, Tan Binh District, HCM City, VN
**************************************************************************/
#ifndef __AT_COMMAND_PARSER_H__
#define __AT_COMMAND_PARSER_H__
#include <tonymeter_at_cmd_parser.h>
#include <tonymeter_sparser.h>
#include "stdio.h"
#include "string.h"


extern uint8_t ampm_AtCommandParserEnable;
extern STR_PARSER_Type AT_CmdParser;




void AT_CmdProcessInit(void);
void AT_ComnandParser();
void AT_CommandCtl(void);


#endif
