/* created by Vikrant Thakur(R&D)
 * version :- v.0
 * date :-  08/07/2021
 */
#ifndef SPI_FLASH
#define SPI_FLASH

#include "r_cg_macrodriver.h"

/******************************************************************************
	 	 	 	 	 	 	 W25Q16V INS
******************************************************************************/

#define	MANID         0x90
#define PAGEPROG      0x02
#define READDATA      0x03
#define FASTREAD      0x0B

#define WRITEDISABLE  0x04
#define READSTAT1     0x05
#define READSTAT2     0x35
#define READSTAT3     0x15
#define WRITESTATEN   0x50
#define WRITESTAT1    0x01
#define WRITESTAT2    0x31
#define WRITESTAT3    0x11
#define WRITEENABLE   0x06

#define ADDR4BYTE_EN  0xB7
#define ADDR4BYTE_DIS 0xE9
#define SECTORERASE   0x20
#define BLOCK32ERASE  0x52
#define BLOCK64ERASE  0xD8
#define CHIPERASE     0x60
#define ALT_CHIPERASE 0xC7    // Some flash chips use a different chip erase command
#define SUSPEND       0x75
#define ID            0x90
#define RESUME        0x7A

#define JEDECID       0x9F
#define POWERDOWN     0xB9

#define RELEASE       0xAB
#define READSFDP      0x5A
#define UNIQUEID      0x4B
#define FRAMSERNO     0xC3
#define W25QXX_DUMMY_BYTE         0xA5

#define READ_BLOCK_ADD_2 0x20000
#define READ_BLOCK_ADD_3 0x30000
#define READ_BLOCK_ADD_4 0x40000
#define RESET 0x99

/******************************************************************************
	 	 	 	 	 	 	 W25Q16V Parameters
******************************************************************************/

typedef struct __attribute__((__packed__))
{
	uint32_t JDEC_ID;
	uint16_t  pagesize;
	uint32_t  sectorsize;
	uint8_t  blocksize;
	uint8_t  blockcount;
	uint16_t sectorcount;
	uint32_t CapacityInKiloByte;
}w25qxx_parm;

/******************************************************************************
	 	 	 	 	 	 	 W25Q16V API
******************************************************************************/
void w25xx_init(void);
uint8_t w25xx_read_status_reg(uint8_t read_status_register);
void w25xx_erase_chip();
uint32_t w25xxJDEG_ID();
void w25xxwritebytes(uint8_t buff , uint32_t write_add);
void w25xxreadbytes(uint8_t *pbuff , uint32_t read_add);
void w25xxblockerase(uint32_t BlockAddr);
void w25xxerasesector(uint32_t SectorAddr);
void w25xxwritepage(uint8_t *pBuffer, uint32_t Page_Address, uint32_t OffsetInByte, uint32_t NumByteToWrite_up_to_PageSize);
void w25xxreadpage(uint8_t *pBuffer	,uint32_t Page_Address, uint32_t OffsetInByte, uint32_t NumByteToWrite_up_to_PageSize);

extern volatile  uint8_t flash_sen_wait_flag;
extern volatile  uint8_t flash_rec_wait_flag;

#endif
