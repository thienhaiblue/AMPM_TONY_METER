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
// Module Name:         avnet_console.c
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


#include <printf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "stdint.h"



#if defined(LINUX_CODE)
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#endif

#include <console_core.h>
#include <console_scanput.h>
#include <console_serial_api.h>
#include <console_tokenize.h>


static int console_core_execute_cmd(console_t *pConsole, int cargc, char **cargv);

void console_core_init(console_t *pConsole)
{
    int index;

    pConsole->inchar = ' ';
    pConsole->inline_count = 0;
    pConsole->verbose = 0;
    pConsole->echo = 1;
    pConsole->run_once = 0;
    pConsole->quit = 0;

    // Clear out the command input buffer.
    for (index = 0; index < MAX_LINE_LENGTH; index++)
    {
        pConsole->inline_buffer[index] = 0;
    }

    // Flush the UART receive buffer.
    //console_serial_flush();

    memset(pConsole->cmd_list, 0, MAX_CMD_CNT * sizeof(cmd_entry));
    pConsole->cmd_list_cnt = 0;
    return;
}

void console_core_process(console_t *pConsole)
{
    int  cargc;
    char * cargv[MAX_ARGC];
    int len;

    if (pConsole->echo)
    {
        pConsole->io_hprintf(pConsole->io_handle, "%c", pConsole->inchar);
    }
   
#if 1
    // Check if complete line has been received ...
    if (console_core_get_line_poll(pConsole) == -1)
    {
        return;
    }

    if (pConsole->run_once == 0)
    {
        // Print the first prompt.
        pConsole->io_hprintf(pConsole->io_handle, "\n\r>>");
        pConsole->inline_buffer[0] = 0;
        pConsole->inline_count = 0;
        pConsole->run_once = 1;
        return;
    }

    // Pre-process command line
    len = strlen(pConsole->inline_buffer);
    if (pConsole->inline_buffer[len-1] == '\n')
    {
        pConsole->inline_buffer[len-1] = 0;
    }
    tokenize(pConsole->inline_buffer, &cargc, cargv, MAX_ARGC);

    // Process command line
    if (cargc == 0)
    {
        pConsole->io_hprintf(pConsole->io_handle, "\n\r>>");
        return;
    }
    else if (pConsole->verbose)
    {
        pConsole->io_hprintf(pConsole->io_handle, "\t");

        for (len = 0; len < cargc; len++)
        {
            pConsole->io_hprintf(pConsole->io_handle, "%s ", cargv[len]);
        }

        pConsole->io_hprintf(pConsole->io_handle, "\n\r");
    }

    if (cargv[0][0] == '#')
    {
        // comment, ignore line ...
    }
    else {
    	if(console_core_execute_cmd(pConsole, cargc, cargv) != 0) {
    		pConsole->io_hprintf(pConsole->io_handle, "\r\nUnknown command : %s\n\r", cargv[0]);

    	}
    }
    pConsole->io_hprintf(pConsole->io_handle, "\n\r>>");

#else
    // Get input character from xxx_session
    inchar = pConsole->inchar;
   
    //pConsole->io_hprintf( pConsole->io_handle, "%c (0x%02X)\n\r",inchar,inchar);

    switch (inchar)
    {
        case '?':
        {
            avnet_console_help(p);
            break;
        }
        default:
        {
            break;
        }
    }

    pConsole->io_hprintf( pConsole->io_handle, "\n\r>");
#endif

    return;
}

int console_core_get_line_poll(console_t *pConsole)
{
    int buffer_index;
    char character_copy = 0;
    uint8_t DataBuffer[MAX_LINE_LENGTH];
    unsigned int received_char_count = 0;

    // New characters come in one at a time ...
    DataBuffer[0] = (uint8_t)pConsole->inchar;
    received_char_count = 1;

    for (buffer_index = 0; buffer_index < received_char_count; buffer_index++)
    {
        // Make sure that the line length has not been reached.
        if (pConsole->inline_count == (MAX_LINE_LENGTH-3))
        {
            // Force a line feed character, this is the end of the
            // line that is being polled for.
            pConsole->io_hprintf( pConsole->io_handle, "\r\n" );
            // Null terminate the string so that it is still a
            // valid string.
            pConsole->inline_buffer[pConsole->inline_count] = 0;
            // Reset the total character count in preparation for the
            // next line yet to be received.
            pConsole->inline_count = 0;
            // Return the total count of characters in the current line.
            return MAX_LINE_LENGTH;
        }

        // Get the next character that was received from the UART
        // device.
        character_copy = (char) DataBuffer[buffer_index];

        // Determine what action to take with the next received
        // character.
#if defined(LINUX_CODE)	  
        if (character_copy == '\r')
#else
        if (character_copy == '\n')
#endif	  
        {
            // Ignore it.
            ;
        }
#if defined(LINUX_CODE)	  
        else if (character_copy == '\n')
#else
        else if (character_copy == '\r')
#endif	  
        {
            // A line feed character has been encountered, this is the
            // end of the line that is being polled for.
            pConsole->io_hprintf( pConsole->io_handle, "\r\n" );
            // Null terminate the string so that it is still a
            // valid string.
            pConsole->inline_buffer[pConsole->inline_count] = 0;
            // Determine the number of characters that are in the line.
            received_char_count = pConsole->inline_count;
            // Reset the total character count in preparation for the
            // next line yet to be received.
            pConsole->inline_count = 0;
            // Return the total count of characters in the current line.
            return received_char_count;
        }
        // Check for backspace or delete key.
        else if ((character_copy == '\b') || (character_copy == 0x7F))
        {
            if (pConsole->inline_count > 0)
            {
                pConsole->io_hprintf( pConsole->io_handle, " \b" );
                pConsole->inline_count--;
                pConsole->inline_buffer[pConsole->inline_count] = 0;
            }
            else
            {
                // User backspaced into prompt, fix the prompt and reposition
                // the cursor.
                pConsole->io_hprintf( pConsole->io_handle, ">" );
            }
        }
        // Check for escape key or control-U.
        else if ((character_copy == 0x1b) || (character_copy == 0x15))
        {
            while (pConsole->inline_count > 0)
            {
                pConsole->io_hprintf( pConsole->io_handle, " \b" );
                pConsole->inline_count--;
                pConsole->inline_buffer[pConsole->inline_count] = 0;
            }
        }
        else
        {
            // Echo character back to the user.
            pConsole->inline_buffer[pConsole->inline_count] = character_copy;
            pConsole->inline_count++;
        }
    }

    return -1;
}

int console_core_register_command(console_t *pConsole, const char *cmd, console_handle_t handle)
{
	if(pConsole->cmd_list_cnt >= MAX_CMD_CNT) {
		return -1;
	}

	if(cmd == NULL || handle == NULL) {
		return -1;
	}

	if(strlen(cmd) > MAX_CMD_LENGTH) {
		return -1;
	}

	strcpy(pConsole->cmd_list[pConsole->cmd_list_cnt].cmd, cmd);
	pConsole->cmd_list[pConsole->cmd_list_cnt].handle = handle;

	pConsole->cmd_list_cnt++;
	return pConsole->cmd_list_cnt;
}

static int console_core_execute_cmd(console_t *pConsole, int cargc, char **cargv)
{
	int i = 0;
	int err = -1;

	for(i = 0; i < pConsole->cmd_list_cnt; i++) {
		if(strcmp(pConsole->cmd_list[i].cmd, cargv[0]) == 0) {
			pConsole->cmd_list[i].handle(pConsole, cargc, cargv);
			err = 0;
			break;
		}
	}

	return err;
}
