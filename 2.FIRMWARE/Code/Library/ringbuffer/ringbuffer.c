/*
 * ringbuffer.c
 *
 *  Created on:
 *      Author: 
 */


#include "ringbuffer.h"


eRingBufferResult ringBuffer_init(sRingBufferHandle_Typedef* handle, eRingBufferType ringbuffType, uint16_t numberOfElements)
{
	if(ringbuffType == eRingBuffer_Type8bit)
	{
		handle->bufferData8 = (uint8_t*)malloc(sizeof(uint8_t) * (numberOfElements + 1));
		assert_param(handle->bufferData8  != NULL);
	}
	else if (ringbuffType == eRingBuffer_Type32bit)
	{
		handle->bufferData32 = (uint32_t*)malloc(sizeof(uint32_t) * (numberOfElements + 1));
		assert_param(handle->bufferData32  != NULL);
	}
	handle->type = ringbuffType;

	handle->totalSize = numberOfElements + 1;
	handle->readIndex = 0;
	handle->writeIndex = 0;

	if(handle->type == eRingBuffer_Type8bit)
	{
		if(handle->bufferData8 != NULL)
		{
			return eResult_OK;
		}
		else
		{
			return eResult_Error;
		}
	}
	else if (handle->type == eRingBuffer_Type32bit)
	{
		if(handle->bufferData32 != NULL)
		{
			return eResult_OK;
		}
		else
		{
			return eResult_Error;
		}
	}
	else
	{
		return eResult_Error;
	}
}



eRingBufferResult ringBuffer_write(sRingBufferHandle_Typedef* handle, uint32_t writeData)
{
	if(handle->type == eRingBuffer_Type8bit)
	{
		assert_param(handle->bufferData8 != NULL);
	}
	else if (handle->type == eRingBuffer_Type32bit)
	{
		assert_param(handle->bufferData32 != NULL);
	}

	uint16_t next_index = (handle->writeIndex + 1) % (handle->totalSize);
	if(next_index != handle->readIndex)
	{
		if(handle->type == eRingBuffer_Type8bit)
		{
			*((uint8_t*)handle->bufferData8 + handle->writeIndex) = (uint8_t)writeData;
		}
		else if ( handle->type == eRingBuffer_Type32bit)
		{
			*((uint32_t*)handle->bufferData32 + handle->writeIndex) = (uint32_t)writeData;
		}
		else
		{
			return eResult_Error;
		}

		handle->writeIndex = next_index;
		return eResult_OK;
	}
	else
	{
		return eResult_Error;
	}
}



eRingBufferResult ringBuffer_read(sRingBufferHandle_Typedef* handle, uint32_t* readData)
{
	if(handle->type == eRingBuffer_Type8bit)
	{
		assert_param(handle->bufferData8 != NULL);
	}
	else if (handle->type == eRingBuffer_Type32bit)
	{
		assert_param(handle->bufferData32 != NULL);
	}
	if(handle->writeIndex != handle->readIndex)
	{
		if(handle->type == eRingBuffer_Type8bit)
		{
			*readData = *(handle->bufferData8 + handle->readIndex);
		}
		else if(handle->type == eRingBuffer_Type32bit)
		{
			*readData = *(handle->bufferData32 + handle->readIndex);
		}
		else
		{
			return eResult_Error;
		}
		handle->readIndex = (handle->readIndex + 1) % (handle->totalSize);
		return eResult_OK;
	}
	else
	{
		return eResult_Error;
	}
}


eRingBufferResult ringBuffer_flush(sRingBufferHandle_Typedef* handle)
{
	uint32_t tempData;
	while(ringBuffer_read(handle, (uint32_t*) &tempData) == eResult_OK);

	return eResult_OK;
}
