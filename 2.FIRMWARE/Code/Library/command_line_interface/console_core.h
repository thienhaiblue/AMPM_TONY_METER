//----------------------------------------------------------------------------
//      _____
//     *     *
//    *____   *____
//   * *===*   *==*
//  *___*===*___**  AVNET
//       *======*
//        *====*
//----------------------------------------------------------------------------
//
// This design is the property of Avnet.  Publication of this
// design is not authorized without written consent from Avnet.
// 
// Please direct any questions to:  technical.support@avnet.com
//
// Disclaimer:
//    Avnet, Inc. makes no warranty for the use of this code or design.
//    This code is provided  "As Is". Avnet, Inc assumes no responsibility for
//    any errors, which may appear in this code, nor does it make a commitment
//    to update the information contained herein. Avnet, Inc specifically
//    disclaims any implied warranties of fitness for a particular purpose.
//                     Copyright(c) 2011 Avnet, Inc.
//                             All rights reserved.
//
//----------------------------------------------------------------------------
//
// Create Date:         Nov 18, 2011
// Design Name:         Avnet Console
// Module Name:         avnet_console.h
// Project Name:        Avnet Console
//
// Tool versions:       Vivado/SDK 2014.4
//
// Description:         Text-based console for 
//                      Zed MAC Address Flash Example
//
// Dependencies:        
//
// Revision:            Nov 18, 2010: 1.01 Initial version
//                      May 23, 2013: 1.02 Updated for Zed Display Kit
//                      May 23, 2013: 1.03 Updated for Zynq SW SpeedWay 2013
//                      Mar 10, 2015: 1.04 Updated for Zynq SW SpeedWay 2015
//
//----------------------------------------------------------------------------

#ifndef __AVNET_CONSOLE_H__
#define __AVNET_CONSOLE_H__

#include <stdio.h>
#include "stdint.h"

#define MAX_LINE_LENGTH             128
#define MAX_ARGC                    16

#define MAX_CMD_LENGTH				16
#define MAX_CMD_CNT					32


typedef void (*console_handle_t)(void *pConsole, int argc, char **argv);

typedef struct {
	char	cmd[MAX_CMD_LENGTH];
	console_handle_t handle;
} cmd_entry;


// This structure allows the text-based console to be accessed from any interface
//   for example: serial port, ethernet connection, etc...
typedef struct struct_avnet_console_t
{
	// For input:
	// - new character received from user
	int32_t inchar;
	char inline_buffer[MAX_LINE_LENGTH];
	int inline_count;

	cmd_entry cmd_list[MAX_CMD_CNT];
	int cmd_list_cnt;

	// For output:
	// - context handle
	// - function pointer to formatted print routine
	void * io_handle;
	void (*io_hprintf)( void *h, const char *fmt, ... );

	// General status:
	int verbose;
	int echo;
	int run_once;
	int quit;
} console_t;

void console_core_init(console_t *pConsole);
void console_core_process(console_t *pConsole);
int  console_core_register_command(console_t *pConsole,
		const char *cmd, console_handle_t handle);
int	console_core_get_line_poll(console_t *pConsole);

#endif // __AVNET_CONSOLE_H__
