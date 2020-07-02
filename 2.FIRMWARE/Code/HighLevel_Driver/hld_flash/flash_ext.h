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
#ifndef DRIVER_T_SPI_FLASH_T_H_
#define DRIVER_T_SPI_FLASH_T_H_

#include <stdint.h>


/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* WINBOND vendor id: 0xEF */
#define SPI_FLASH_WINBOND_VENDER_ID         	(0xEF)
/* Length ID Code */
#define IDCODE_LEN                          	(0x3U)

/* flash capacity configuration */
/* 256B page size */
#define EX_FLASH_PAGE_SIZE                     	(256U)
/* 4K sector size */
#define EX_FLASH_SECTOR_SIZE                   	(4U * 1024U)
/* 2MB total size */
#define EX_FLASH_TOTAL_SIZE                    	(2048U * 1024U)

/** Helper functions */
#define ROUND(a, b) (((a) + (b)) & ~((b)-1))
#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifdef SFDEBUG
#define SF_DEBUG(str) printf(str)
#else
#define SF_DEBUG(str)
#endif

#define SPI_FLASH_TIMEOUT                   	(200000U)
/* flash commands */
#define CMD_READ_ID                         	(0x9f)
#define CMD_READ_ARRAY_SLOW                 	(0x03)
#define CMD_READ_ARRAY_FAST                 	(0x0b)
#define CMD_WRITE_STATUS                    	(0x01)
#define CMD_PAGE_PROGRAM                    	(0x02)
#define CMD_WRITE_DISABLE                   	(0x04)
#define CMD_READ_STATUS                     	(0x05)
#define CMD_WRITE_ENABLE                    	(0x06)
#define CMD_ERASE_4K                        	(0x20)
#define CMD_ERASE_32K                       	(0x52)
#define CMD_ERASE_64K                       	(0xd8)
#define CMD_ERASE_CHIP                      	(0xc7)


/* Erase block size */
#define ERASE_4K_SIZE                       	(4U * 1024U)
#define ERASE_32K_SIZE                      	(32U * 1024U)
#define ERASE_64K_SIZE                      	(64U * 1024U)

typedef enum
{
    EXTERNAL_FLASH_SUCCESS = 0U, 				/**< @brief External flash access success */
    EXTERNAL_FLASH_ERROR,        				/**< @brief External flash access error */
    EXTERNAL_FLASH_INIT_ERROR,   				/**< @brief External flash init fail */
    EXTERNAL_FLASH_WRITE_ERROR, 				/**< @brief External flash write fail */
    EXTERNAL_FLASH_READ_ERROR,   				/**< @brief External flash read fail */
    EXTERNAL_FLASH_ERASE_ERROR,  				/**< @brief External flash erase fail */
} externalFlashError_t;

typedef struct
{
    uint32_t size;
    uint32_t page_size;
    uint32_t sector_size;
} spiFlash_t;


/*******************************************************************************
 * API
 ******************************************************************************/
externalFlashError_t ExtFlash_enable_write(uint8_t is_enabled);
externalFlashError_t ExtFlash_write_page(uint8_t* buf,
                                         uint32_t page_offset,
                                         uint32_t byte_offset,
                                         uint32_t len);
externalFlashError_t ExtFlash_write(uint32_t offset, uint32_t len,const uint8_t * buf);
externalFlashError_t ExtFlash_read(uint32_t offset, uint32_t data_len,uint8_t * data);
externalFlashError_t ExtFlash_erase_block(uint32_t offset, uint32_t blkSize);
externalFlashError_t ExtFlash_erase_sector(uint32_t offset, uint32_t len);
externalFlashError_t ExtFlash_erase_all(void);
externalFlashError_t ExtFlash_write_status(uint8_t sts_reg);
externalFlashError_t ExtFlash_Init(void);
void ExtFlash_Deinit(void);

#if defined(__cplusplus)
}
#endif

#endif /* DRIVER_T_SPI_FLASH_T_H_ */
