/*
 * console_config.h
 *
 *  Created on: 
 *      Author:
 */

#ifndef COMMAND_LINE_INTERFACE_CONSOLE_CONFIG_H_
#define COMMAND_LINE_INTERFACE_CONSOLE_CONFIG_H_


#include "hardware_config.h"
#include "ringbuffer.h"


#define	RINGBUFFER_CONSOLE_MAX_LENGTH			(128)


extern uint8_t gUart_Dbg_Temp;
extern sRingBufferHandle_Typedef	sRingBuffer_Console_Handle;


/* Declaring function prototype for console feature */
void console_putchar(uint8_t data);
eRingBufferResult console_getchar(uint8_t* data);
void console_receiveData(void);
void console_RXCallback(void);


/* Declaring function prototype for console command handle */
void help_callback(void *pConsole, int argc, char **argv);
void setRelay_callback(void *pConsole, int argc, char **argv);
void resetEnergy_callback(void *pConsole, int argc, char **argv);
#endif /* COMMAND_LINE_INTERFACE_CONSOLE_CONFIG_H_ */
