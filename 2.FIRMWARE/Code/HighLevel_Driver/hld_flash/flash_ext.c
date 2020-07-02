/**************************************************************************************************
 *
 * Copyright (C) 2019, AMPM JSC. All Rights Reserved.
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 ****************************************************************************************************/

/******************************************************************************/
// INCLUDE
/******************************************************************************/
#include <string.h>
#include "flash_ext.h"
#include "application_config.h"
#include "hardware_config.h"


/** Flash status as Busy */
#define kExFls_Status_Busy  	(0x01U)


spiFlash_t gSpiFlash;
volatile bool isDspiTransferCompleted = false;
uint8_t vendorId = 0U;
uint8_t devId[2];

/******************************************************************************/
// Porting for SPI read/write flash
/******************************************************************************/
/*
 * Set the SPI of Flash to Active to Transfer
 */
static inline void spi_cs_activate(void)
{
	HAL_GPIO_WritePin(MCU_SPI_CS_GPIO_Port, MCU_SPI_CS_Pin, GPIO_PIN_RESET);
}

/*
 * Set the SPI of Flash to Deactive to Transfer
 */
static inline void spi_cs_deactivate(void)
{
	HAL_GPIO_WritePin(MCU_SPI_CS_GPIO_Port, MCU_SPI_CS_Pin, GPIO_PIN_SET);
}

/*
 * Initialize the SPI of Flash
 */
static inline void spi_init(void)
{
	MX_SPI1_Init();
}

/*
 * Un-Initialize the SPI of Flash
 */
static inline void spi_deinit(void)
{

}

/**
 * Transfer data to Flash
 */
static inline bool spi_transfer(uint8_t* out, uint8_t* in, uint32_t len)
{
	bool ret = false;
	HAL_StatusTypeDef stt =  HAL_ERROR;

	/* Check if valid input param */
	if ((0U != len) && ((NULL != out) || (NULL != in)))
	{
		/* Transmit only */
		if((NULL != in) && (NULL == out))
		{
			stt = HAL_SPI_Transmit(&SPI_HANDLE, (uint8_t*)in, len, SPI_FLASH_TIMEOUT);
		}
		/* Receive only */
		else if ((NULL == in) && (NULL != out))
		{
			stt = HAL_SPI_Receive(&SPI_HANDLE, (uint8_t*)out, len, SPI_FLASH_TIMEOUT);
		}
		/* Both Transmit and receive */
		else if  ((NULL != in) && (NULL != out))
		{
			stt = HAL_SPI_TransmitReceive(&SPI_HANDLE, in, out, len, SPI_FLASH_TIMEOUT);
		}
	}
	else
	{
		assert_param(false);
	}
	ret = !(bool)stt;
	return ret;
}

/******************************************************************************/
// External flash communicate
/******************************************************************************/
/**
 * Write to external flash.
 *
 *@return externalFlashError_t code
 */
static externalFlashError_t ExtFlash_rw(uint8_t* cmd, uint32_t cmd_len,
		uint8_t* data_in, uint8_t* data_out, uint32_t data_len)
{
	bool result;
	externalFlashError_t ret = EXTERNAL_FLASH_ERROR;

	if ((NULL != cmd) && (0 != cmd_len))
	{
		spi_cs_activate();
		/* Send CMD to Flash */
		result = spi_transfer(NULL, cmd, cmd_len);
		if (true == result)
		{
			/* Send data */
			if (((NULL != data_in) || (NULL != data_out)) && (0U != data_len))
			{
				result = spi_transfer(data_out, data_in, data_len);
				if (true == result)
				{
					/* Write success */
					ret = EXTERNAL_FLASH_SUCCESS;
				}
				else
				{
					assert_param(false);
				}
			}
			else
			{
				/* Not need to write data. So, just need to send command */
				ret = EXTERNAL_FLASH_SUCCESS;
			}
		}
		else
		{
			assert_param(false);
		}
		spi_cs_deactivate();
	}

	return ret;
}

static externalFlashError_t ExtFlash_readid(uint8_t* vendorId, uint8_t devId[])
{
	externalFlashError_t ret = EXTERNAL_FLASH_SUCCESS;
	uint8_t idcode[IDCODE_LEN] = {0};
	uint8_t cmd = CMD_READ_ID;

	/* Send CMD_READ_ID to get flash chip ID codes */
	ret = ExtFlash_rw(&cmd, 1, NULL, idcode, sizeof(idcode));
	if (EXTERNAL_FLASH_SUCCESS == ret)
	{
		*vendorId = idcode[0];
		devId[0] = idcode[1];
		devId[1] = idcode[2];
	}

	return ret;
}

/**
 * Check current status of flash as ready to use
 */
static externalFlashError_t ExtFlash_chk_status(uint32_t timeout, uint8_t cmd, uint8_t poll_bit)
{
	uint32_t i = 0;
	bool result;
	uint8_t flashStatus;
	externalFlashError_t err = EXTERNAL_FLASH_ERROR;

	spi_cs_activate();
	/* Send CMD to Flash */
	result = spi_transfer(NULL, &cmd, 1U);
	if (true == result)
	{
		for (i = 0; i < timeout; i++)
		{
			/* Read status */
			result = spi_transfer(&flashStatus, NULL, 1U);
			if (true == result)
			{
				if ((flashStatus & poll_bit) == 0)
				{
					/* Read success */
					err = EXTERNAL_FLASH_SUCCESS;
					break;
				}
			}
			else
			{
				assert_param(false);
				/* Read data failed */
				break;
			}
		}
	}
	else
	{
		assert_param(false);
	}
	spi_cs_deactivate();

	return err;
}

/**
 * Init the external flash as Winbond
 */
static externalFlashError_t ExtFlash_init_winbond(uint32_t pageSize, uint32_t sectorSize, uint32_t sectorNum)
{
	spiFlash_t* flash = &gSpiFlash;

	flash->page_size = pageSize;
	flash->sector_size = sectorSize;
	flash->size = flash->sector_size * sectorNum;

	return ExtFlash_chk_status(SPI_FLASH_TIMEOUT, CMD_READ_STATUS, kExFls_Status_Busy);
}


static void ExtFlash_addr2cmd(uint32_t addr, uint8_t* cmd)
{
	cmd[1] = (addr >> 16U) & 0xFFU;
	cmd[2] = (addr >> 8U) & 0xFFU;
	cmd[3] = (addr >> 0U) & 0xFFU;
}



/******************************************************************************/
// GLOBAL FUNCTIONS
/******************************************************************************/
/**
 * Initialize externflash
 */
externalFlashError_t ExtFlash_Init(void)
{
	externalFlashError_t status = EXTERNAL_FLASH_SUCCESS;

	/* Initialize SPI interface to communicate with external flash */
	spi_init();

	/* Init & verify external flash */
	status = ExtFlash_readid(&vendorId, devId);
	if (status == EXTERNAL_FLASH_SUCCESS)
	{
		/* Check external flash is Winbond vendor or not */
		if (SPI_FLASH_WINBOND_VENDER_ID == vendorId)
		{
			status = ExtFlash_init_winbond(EX_FLASH_PAGE_SIZE, EX_FLASH_SECTOR_SIZE, 256);
		}
		else
		{
			status = EXTERNAL_FLASH_INIT_ERROR;
		}
	}

	return status;
}

/**
 * De-Initialize externflash
 */
void ExtFlash_Deinit(void)
{
	spi_deinit();
}

externalFlashError_t ExtFlash_enable_write(uint8_t is_enabled)
{
	uint8_t cmd;

	cmd = is_enabled ? CMD_WRITE_ENABLE : CMD_WRITE_DISABLE;

	return ExtFlash_rw(&cmd, 1, NULL, NULL, 0);
}

externalFlashError_t ExtFlash_write_page(uint8_t* buf,
		uint32_t page_offset,
		uint32_t byte_offset,
		uint32_t len)
{
	externalFlashError_t status = EXTERNAL_FLASH_SUCCESS;

	uint8_t cmd[4] = {0};

	cmd[0] = CMD_PAGE_PROGRAM;
	cmd[1] = (uint8_t)(page_offset >> 8);
	cmd[2] = (uint8_t)(page_offset & 0xff);
	cmd[3] = byte_offset;

	/* Each write need to enable write */
	status = ExtFlash_enable_write(1);

	if (status == EXTERNAL_FLASH_SUCCESS)
	{
		status = ExtFlash_rw(cmd, 4, buf, NULL, len);
		printf("\r\nExtFlash_rw(cmd, 4, buf, NULL, len)");
		printf_process();
	}

	if (status == EXTERNAL_FLASH_SUCCESS)
	{
		status = ExtFlash_chk_status(SPI_FLASH_TIMEOUT, CMD_READ_STATUS, kExFls_Status_Busy);
		printf("\r\nExtFlash_chk_status");
		printf_process();
	}
	return status;
}

externalFlashError_t ExtFlash_write(uint32_t offset, uint32_t len,
		const uint8_t * buf)
{
	uint32_t page_offset = 0;
	uint32_t byte_offset = 0;
	uint32_t page_size = 0;
	uint32_t data_chunk_len = 0;
	uint32_t data_transferred = 0;
	externalFlashError_t status = EXTERNAL_FLASH_SUCCESS;

	page_size = EX_FLASH_PAGE_SIZE;
	page_offset = offset / page_size;
	byte_offset = offset % page_size;

	while ((data_transferred < len) && (status == EXTERNAL_FLASH_SUCCESS))
	{
		/* First and last sector might be unaligned to page_size, So transfer unaligned sector
		 * first. */
		data_chunk_len = min(len - data_transferred, page_size - byte_offset);

		status = ExtFlash_write_page(((uint8_t*)buf + data_transferred), page_offset, byte_offset, data_chunk_len);
		if (EXTERNAL_FLASH_SUCCESS == status)
		{
			byte_offset += data_chunk_len;
			if (byte_offset == page_size)
			{
				page_offset++;
				byte_offset = 0;
			}
			data_transferred += data_chunk_len;
		}
	}

	return status;
}

externalFlashError_t ExtFlash_read(uint32_t offset, uint32_t data_len, uint8_t* data)
{
	externalFlashError_t eResult = EXTERNAL_FLASH_ERROR;
	uint8_t cmd[5];

	/* Buffer tmp */
	//uint8_t tmp[SECTOR_SIZE] = {0};

	cmd[0] = CMD_READ_ARRAY_FAST;
	ExtFlash_addr2cmd(offset, cmd);
	cmd[4] = 0x00U;

	eResult = ExtFlash_rw(cmd, sizeof(cmd), NULL, data, data_len);
	assert_param(eResult == EXTERNAL_FLASH_SUCCESS);

	//eResult = ExtFlash_rw(cmd, sizeof(cmd), tmp, data, data_len);
	return eResult;
}

externalFlashError_t ExtFlash_erase_block(uint32_t offset, uint32_t blkSize)
{
	externalFlashError_t status = EXTERNAL_FLASH_SUCCESS;
	uint8_t cmd[4];

	/* Check block size */
	if (offset % blkSize)
	{
		status = EXTERNAL_FLASH_ERROR;
	}

	switch (blkSize)
	{
	case ERASE_4K_SIZE:
		cmd[0] = CMD_ERASE_4K;
		break;
	case ERASE_32K_SIZE:
		cmd[0] = CMD_ERASE_32K;
		break;
	case ERASE_64K_SIZE:
		cmd[0] = CMD_ERASE_64K;
		break;
	default:
		status = EXTERNAL_FLASH_ERROR;
		break;
	}

	/* Enable write */
	if (status == EXTERNAL_FLASH_SUCCESS)
	{
		ExtFlash_addr2cmd(offset, cmd);
		status = ExtFlash_enable_write(1);
	}

	/* Erase flash */
	if (status == EXTERNAL_FLASH_SUCCESS)
	{
		status = ExtFlash_rw(cmd, sizeof(cmd), NULL, NULL, 0);
	}

	/* Check status */
	if (status == EXTERNAL_FLASH_SUCCESS)
	{
		status = ExtFlash_chk_status(SPI_FLASH_TIMEOUT, CMD_READ_STATUS, kExFls_Status_Busy);
	}

	return status;
}

externalFlashError_t ExtFlash_erase_sector(uint32_t offset, uint32_t len)
{
	externalFlashError_t status = EXTERNAL_FLASH_SUCCESS;
	uint32_t eraseStart;
	uint32_t eraseEnd;
	uint32_t eraseSize;
	uint8_t cmd[4];
	spiFlash_t* flash = &gSpiFlash;

	eraseSize = flash->sector_size;
	if ((offset % eraseSize != 0) || (len % eraseSize != 0))
	{
		return EXTERNAL_FLASH_SUCCESS;
	}

	/* Select erase Size */
	switch (eraseSize)
	{
	case ERASE_4K_SIZE:
		cmd[0] = CMD_ERASE_4K;
		break;
	case ERASE_32K_SIZE:
		cmd[0] = CMD_ERASE_32K;
		break;
	case ERASE_64K_SIZE:
		cmd[0] = CMD_ERASE_64K;
		break;
	default:
		cmd[0] = CMD_ERASE_4K;
		break;
	}

	eraseStart = offset;
	eraseEnd = eraseStart + len;

	/* erase sectors */
	while ((offset < eraseEnd) && (status == EXTERNAL_FLASH_SUCCESS))
	{
		ExtFlash_addr2cmd(offset, cmd);
		offset += eraseSize;

		if (status == EXTERNAL_FLASH_SUCCESS)
		{
			status = ExtFlash_enable_write(1);
		}

		if (status == EXTERNAL_FLASH_SUCCESS)
		{
			status = ExtFlash_rw(cmd, sizeof(cmd), NULL, NULL, 0);
		}

		if (status == EXTERNAL_FLASH_SUCCESS)
		{
			status = ExtFlash_chk_status(SPI_FLASH_TIMEOUT, CMD_READ_STATUS, kExFls_Status_Busy);
		}
	}

	return status;
}

externalFlashError_t ExtFlash_erase_all(void)
{
	externalFlashError_t status = EXTERNAL_FLASH_SUCCESS;
	uint8_t cmd = CMD_ERASE_CHIP;

	if (status == EXTERNAL_FLASH_SUCCESS)
	{
		status = ExtFlash_enable_write(1);
	}

	if (status == EXTERNAL_FLASH_SUCCESS)
	{
		status = ExtFlash_rw(&cmd, 1, NULL, NULL, 0);
	}

	if (status == EXTERNAL_FLASH_SUCCESS)
	{
		ExtFlash_chk_status(SPI_FLASH_TIMEOUT, CMD_READ_STATUS, kExFls_Status_Busy);
	}

	return status;
}

externalFlashError_t ExtFlash_write_status(uint8_t sts_reg)
{
	uint8_t cmd;
	externalFlashError_t status = EXTERNAL_FLASH_SUCCESS;

	/* Enable write */
	if (status == EXTERNAL_FLASH_SUCCESS)
	{
		status = ExtFlash_enable_write(1);
	}

	/* Send status command */
	cmd = CMD_WRITE_STATUS;
	if (status == EXTERNAL_FLASH_SUCCESS)
	{
		status = ExtFlash_rw(&cmd, 1, &sts_reg, NULL, 1);
	}

	/* Check status */
	if (status == EXTERNAL_FLASH_SUCCESS)
	{
		status = ExtFlash_chk_status(SPI_FLASH_TIMEOUT,
				CMD_READ_STATUS, kExFls_Status_Busy);
	}

	return status;
}
