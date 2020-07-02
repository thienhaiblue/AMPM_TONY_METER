/*
 * ringbuffer.h
 *
 *  Created on:
 *      Author: 
 */

#ifndef RINGBUFFER_RINGBUFFER_H_
#define RINGBUFFER_RINGBUFFER_H_

#include	"stdio.h"
#include 	"stdlib.h"
#include 	"stm32f0xx_hal.h"


typedef enum
{
	eResult_OK = 0,
	eResult_Error
}eRingBufferResult;

typedef enum
{
	eRingBuffer_Type8bit = 0,
	eRingBuffer_Type32bit
}eRingBufferType;

typedef struct
{
	uint8_t* 	bufferData8;
	uint32_t* 	bufferData32;
	uint16_t 	totalSize;
	uint16_t 	writeIndex;
	uint16_t 	readIndex;
	eRingBufferType type;
}sRingBufferHandle_Typedef;


eRingBufferResult ringBuffer_init(sRingBufferHandle_Typedef* handle, eRingBufferType ringbuffType, uint16_t numberOfElements);
eRingBufferResult ringBuffer_write(sRingBufferHandle_Typedef* handle, uint32_t writeData);
eRingBufferResult ringBuffer_read(sRingBufferHandle_Typedef* handle, uint32_t* readData);
eRingBufferResult ringBuffer_flush(sRingBufferHandle_Typedef* handle);



#endif /* RINGBUFFER_RINGBUFFER_H_ */
