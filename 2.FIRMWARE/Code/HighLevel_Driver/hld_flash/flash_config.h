
#ifndef COMPONENTS_FLASH_FLASH_CONFIG_H_
#define COMPONENTS_FLASH_FLASH_CONFIG_H_
/******************************************************************************/
// INCLUDE
/******************************************************************************/
#include <stdint.h>

/******************************************************************************/
// MACRO. DEFINE
/******************************************************************************/
/** Length for 1k (1024) */
#define SIZE_1K                                 (0x400U)
/** Length for 4k */
#define SIZE_4K                                 (4U * SIZE_1K)
/** Length for 8k */
#define SIZE_8K                                 (8U * SIZE_1K)
/** Length for 16k */
#define SIZE_16K                                (16U * SIZE_1K)
/** Length for 32k */
#define SIZE_32K                                (32U * SIZE_1K)
/** Length for 48k */
#define SIZE_48K                                (48U * SIZE_1K)
/** Length for 64k */
#define SIZE_64K                                (64U * SIZE_1K)
/** Length for 128k */
#define SIZE_128K                               (128U * SIZE_1K)

/** Length of sector: 4k */
#define SECTOR_SIZE                             (SIZE_4K)

/* Length of page: 2K */
#define PAGE_SIZE								(2U * SIZE_1K)

#define FLASH_NUMBER_OF_PAGE					(128)
/** The maximum of some value */
#ifndef U32_MAX
#define U32_MAX                                 (0xFFFFFFFFU)
#endif /* U32_MAX */

#ifndef U16_MAX
#define U16_MAX                                 (0xFFFFU)
#endif /* U16_MAX */

#ifndef U8_MAX
#define U8_MAX                                  (0xFFU)
#endif /* U8_MAX */

/******************************************************************************/
// TYPEDEF
/******************************************************************************/


/******************************************************************************/
// FUNCTIONS
/******************************************************************************/

#endif /* COMPONENTS_FLASH_FLASH_CONFIG_H_ */
