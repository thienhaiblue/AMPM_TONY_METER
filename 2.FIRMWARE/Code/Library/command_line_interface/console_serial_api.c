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
// Design Name:         Serial Entry for Avnet Console
// Module Name:         avnet_console_serial.c
// Project Name:        Serial Entry for Avnet Console
//
// Tool versions:       ISE 13.3
//
// Description:         Serial entry point for Text-based console
//
// Dependencies:
//
// Revision:            Nov 18, 2010: 1.01 Initial version
//                      May 23, 2013: 1.02 Updated for Zed Display Kit
//
//----------------------------------------------------------------------------


#include <console_serial_api.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "main.h"




// Serial session
static console_t serial_console;


static void *serial_handle = NULL;

#if 0
// this implemenation uses 50Kbytes more code because of vsnprintf(),
// use alternate implementation instead ...
int serial_hprintf( void *serial_handle, const char * fmt, ...)
{
	static char buf[1024];
	va_list marker;
	int n;
	int i;

	va_start( marker, fmt);
	n = vsnprintf( buf, sizeof buf, fmt, marker);
	va_end( marker);

	for ( i = 0; i < n; i++ )
	{
		OS_PUTCHAR( buf[i] );
	}

	return n;
}
#else
/*---------------------------------------------------*/
/* Modified from :                                   */
/* Public Domain version of printf                   */
/* Rud Merriam, Compsult, Inc. Houston, Tx.          */
/* For Embedded Systems Programming, 1991            */
/*                                                   */
/*---------------------------------------------------*/

#include <ctype.h>
#include <string.h>
#include <stdarg.h>



/*----------------------------------------------------*/
/* Use the following parameter passing structure to   */
/* make xil_printf re-entrant.                        */
/*----------------------------------------------------*/
typedef struct params_s {
	int len;
	int num1;
	int num2;
	char pad_character;
	int do_padding;
	int left_flag;
} params_t;

/*---------------------------------------------------*/
/* The purpose of this routine is to output data the */
/* same as the standard printf function without the  */
/* overhead most run-time libraries involve. Usually */
/* the printf brings in many kilobytes of code and   */
/* that is unacceptable in most embedded systems.    */
/*---------------------------------------------------*/

typedef char* charptr;
//typedef int (*func_ptr)(int c);

/*---------------------------------------------------*/
/*                                                   */
/* This routine puts pad characters into the output  */
/* buffer.                                           */
/*                                                   */
static void padding( const int l_flag, params_t *par)
{
	int i;

	if (par->do_padding && l_flag && (par->len < par->num1))
		for (i=par->len; i<par->num1; i++)
			console_putchar( par->pad_character);
}

/*---------------------------------------------------*/
/*                                                   */
/* This routine moves a string to the output buffer  */
/* as directed by the padding and positioning flags. */
/*                                                   */
static void outs( charptr lp, params_t *par)
{
	/* pad on left if needed                         */
	par->len = strlen( lp);
	padding( !(par->left_flag), par);

	/* Move string to the buffer                     */
	while (*lp && (par->num2)--)
		console_putchar( *lp++);

	/* Pad on right if needed                        */
	/* CR 439175 - elided next stmt. Seemed bogus.   */
	/* par->len = strlen( lp);                       */
	padding( par->left_flag, par);
}

/*---------------------------------------------------*/
/*                                                   */
/* This routine moves a number to the output buffer  */
/* as directed by the padding and positioning flags. */
/*                                                   */

static void outnum( const long n, const long base, params_t *par)
{
	charptr cp;
	int negative;
	char outbuf[32];
	const char digits[] = "0123456789ABCDEF";
	unsigned long num;

	/* Check if number is negative                   */
	if (base == 10 && n < 0L) {
		negative = 1;
		num = -(n);
	}
	else{
		num = (n);
		negative = 0;
	}

	/* Build number (backwards) in outbuf            */
	cp = outbuf;
	do {
		*cp++ = digits[(int)(num % base)];
	} while ((num /= base) > 0);
	if (negative)
		*cp++ = '-';
	*cp-- = 0;

	/* Move the converted number to the buffer and   */
	/* add in the padding where needed.              */
	par->len = strlen(outbuf);
	padding( !(par->left_flag), par);
	while (cp >= outbuf)
		console_putchar( *cp--);
	padding( par->left_flag, par);
}

/*---------------------------------------------------*/
/*                                                   */
/* This routine gets a number from the format        */
/* string.                                           */
/*                                                   */
static int getnum( charptr* linep)
{
	int n;
	charptr cp;

	n = 0;
	cp = *linep;
	while (isdigit(((int)*cp)))
		n = n*10 + ((*cp++) - '0');
	*linep = cp;
	return(n);
}

/*---------------------------------------------------*/
/*                                                   */
/* This routine operates just like a printf/sprintf  */
/* routine. It outputs a set of data under the       */
/* control of a formatting string. Not all of the    */
/* standard C format control are supported. The ones */
/* provided are primarily those needed for embedded  */
/* systems work. Primarily the floaing point         */
/* routines are omitted. Other formats could be      */
/* added easily by following the examples shown for  */
/* the supported formats.                            */
/*                                                   */

/* void esp_printf( const func_ptr f_ptr,
   const charptr ctrl1, ...) */
static void serial_hprintf( void *serial_handle, const char * fmt, ...)
{
	int long_flag;
	int dot_flag;

	params_t par;

	char ch;
	va_list argp;
	char *ctrl = (char *)fmt;

	va_start( argp, fmt);

	for ( ; *ctrl; ctrl++) {

		/* move format string chars to buffer until a  */
		/* format control is found.                    */
		if (*ctrl != '%') {
			console_putchar(*ctrl);
			continue;
		}

		/* initialize all the flags for this format.   */
		dot_flag   = long_flag = par.left_flag = par.do_padding = 0;
		par.pad_character = ' ';
		par.num2=32767;

		try_next:
		ch = *(++ctrl);

		if (isdigit((int)ch)) {
			if (dot_flag)
				par.num2 = getnum(&ctrl);
			else {
				if (ch == '0')
					par.pad_character = '0';

				par.num1 = getnum(&ctrl);
				par.do_padding = 1;
			}
			ctrl--;
			goto try_next;
		}

		switch (tolower((int)ch)) {
		case '%':
			console_putchar( '%');
			continue;

		case '-':
			par.left_flag = 1;
			break;

		case '.':
			dot_flag = 1;
			break;

		case 'l':
			long_flag = 1;
			break;

		case 'd':
			if (long_flag || ch == 'D') {
				outnum( va_arg(argp, long), 10L, &par);
				continue;
			}
			else {
				outnum( va_arg(argp, int), 10L, &par);
				continue;
			}
		case 'x':
			outnum((long)va_arg(argp, int), 16L, &par);
			continue;

		case 's':
			outs( va_arg( argp, char *), &par);
			continue;

		case 'c':
			console_putchar( va_arg( argp, int));
			continue;

		case '\\':
			switch (*ctrl) {
			case 'a':
				console_putchar( 0x07);
				break;
			case 'h':
				console_putchar( 0x08);
				break;
			case 'r':
				console_putchar( 0x0D);
				break;
			case 'n':
				console_putchar( 0x0D);
				console_putchar( 0x0A);
				break;
			default:
				console_putchar( *ctrl);
				break;
			}
			ctrl++;
			break;

			default:
				continue;
		}
		goto try_next;
	}
	va_end( argp);
}
#endif


int console_serial_process(void)
{
	uint8_t inchar;

	if( console_getchar(&inchar) == eResult_OK)
	{
		serial_console.inchar = inchar;

		console_core_process(&serial_console);

		if (serial_console.quit == 1)
		{
			return 1;
		}
	}

	return 0;
}


int console_serial_init(void)
{
	assert_param(ringBuffer_init(&sRingBuffer_Console_Handle,eRingBuffer_Type8bit, RINGBUFFER_CONSOLE_MAX_LENGTH) == eResult_OK);
	console_receiveData();

	// Initialize serial console
	console_core_init( &serial_console );
	serial_console.io_handle = serial_handle;
	serial_console.io_hprintf = (void*) serial_hprintf;
	serial_console.echo = 1; // characters are not echoed by terminal, need echo on
	serial_console.inchar = '\r';
	strcpy( serial_console.inline_buffer, "help" );
	serial_console.inline_count = strlen( serial_console.inline_buffer );
	console_core_process(&serial_console);
	return 0;
}

int console_serial_add_cmd(char *cmd, console_handle_t handle)
{
	return console_core_register_command(&serial_console, cmd, handle);
}
