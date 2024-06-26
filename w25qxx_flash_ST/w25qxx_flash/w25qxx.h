#ifndef _W25QXX_H
#define _W25QXX_H

#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdbool.h>

#define WRITESTAT1    0x01

typedef enum
{
	W25Q10=1,
	W25Q20,
	W25Q40,
	W25Q80,
	W25Q16,
	W25Q32,
	W25Q64,
	W25Q128,
	W25Q256,
	W25Q512,

}W25QXX_ID_t;


typedef struct
{
	W25QXX_ID_t	ID;
	uint8_t		UniqID[8];
	uint16_t	PageSize;
	uint32_t	PageCount;
	uint32_t	SectorSize;
	uint32_t	SectorCount;
	uint32_t	BlockSize;
	uint32_t	BlockCount;
	uint32_t	CapacityInKiloByte;
	uint8_t		StatusRegister1;
	uint8_t		StatusRegister2;
	uint8_t		StatusRegister3;
	uint8_t		Lock;

}w25qxx_t;

extern w25qxx_t	w25qxx;

/*
 * @brief :- 	 w25qxx_init
 *
 * @param[in] :- none
 * @note :-      fixes chip ID , block size , sector size , page size. Read unique ID or read IDNone
 *
 * return :- 	true or false
 */
bool	W25qxx_Init(void);

/*
 * @brief :- 	 Read ID - JDEC ID
 *
 * @param[in] :- none
 * @note :-      None
 *
 * return :- 	4 BYTE ID
 */
uint32_t   W25qxx_ReadID();

/*
 * @brief :- 	 Read Unique ID
 *
 * @param[in] :- none
 * @note :-      None
 *
 * return :- 	4 BYTE ID
 */
void   		W25qxx_ReadUniqID();

//############################################################################
                // ERASE SECTOR API //
//############################################################################

void		W25qxx_EraseChip(void);
void 		W25qxx_EraseSector(uint32_t SectorAddr);
void 		W25qxx_EraseBlock(uint32_t BlockAddr);



//############################################################################
                // returns address of the page verse sector //
//############################################################################


uint32_t	W25qxx_PageToSector(uint32_t PageAddress);
uint32_t	W25qxx_PageToBlock(uint32_t PageAddress);
uint32_t	W25qxx_SectorToBlock(uint32_t SectorAddress);
uint32_t	W25qxx_SectorToPage(uint32_t SectorAddress);
uint32_t	W25qxx_BlockToPage(uint32_t BlockAddress);

//############################################################################
                // Check whether the page, sector or block is empty or not //
//############################################################################

bool 		W25qxx_IsEmptyPage(uint32_t Page_Address, uint32_t OffsetInByte, uint32_t NumByteToCheck_up_to_PageSize);
bool 		W25qxx_IsEmptySector(uint32_t Sector_Address, uint32_t OffsetInByte, uint32_t NumByteToCheck_up_to_SectorSize);
bool 		W25qxx_IsEmptyBlock(uint32_t Block_Address, uint32_t OffsetInByte, uint32_t NumByteToCheck_up_to_BlockSize);

//############################################################################
                // WRITE CHIPS API //
//############################################################################

void 		W25qxx_WriteByte(uint8_t pBuffer, uint32_t Bytes_Address);

/*
 * @brief        :- write on the specific page in chip
 *
 * @param[in] 	 :- buffer to write on flash chip
 * @param[in] 	 :- page address
 * @param[in] 	 :- offset byte - starting byte from where the data needs to be written
 * @param[in] 	 :- how much bytes you need to write
 * @note :-       write the data on the specific page in chip
 *
 * return :- 	none
 */

void 		W25qxx_WritePage(uint8_t *pBuffer, uint32_t Page_Address, uint32_t OffsetInByte, uint32_t NumByteToWrite_up_to_PageSize);
void 		W25qxx_WriteSector(uint8_t *pBuffer, uint32_t Sector_Address, uint32_t OffsetInByte, uint32_t NumByteToWrite_up_to_SectorSize);
void 		W25qxx_WriteBlock(uint8_t* pBuffer, uint32_t Block_Address, uint32_t OffsetInByte, uint32_t NumByteToWrite_up_to_BlockSize);

//############################################################################
                // READ CHIPS API //
//############################################################################

void 		W25qxx_ReadByte(uint8_t *pBuffer, uint32_t Bytes_Address);

/*
 * @brief        :- read the data on the specific page in chip
 *
 * @param[in] 	 :- buffer to read the data from flash chip
 * @param[in] 	 :- page address
 * @param[in] 	 :- offset byte - starting byte from where the data needs to be written
 * @param[in] 	 :- how much bytes you need to write
 * @note :-       read the data on the specific page in chip
 *
 * return :- 	none
 */

void 		W25qxx_ReadBytes(uint8_t *pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);
void 		W25qxx_ReadPage(uint8_t *pBuffer, uint32_t Page_Address,uint32_t OffsetInByte, uint32_t NumByteToRead_up_to_PageSize);
void 		W25qxx_ReadSector(uint8_t *pBuffer, uint32_t Sector_Address,uint32_t OffsetInByte, uint32_t NumByteToRead_up_to_SectorSize);
void 		W25qxx_ReadBlock(uint8_t* pBuffer, uint32_t Block_Address, uint32_t OffsetInByte, uint32_t NumByteToRead_up_to_BlockSize);
//############################################################################
extern w25qxx_t w25qxx;

#ifdef __cplusplus
}
#endif

#endif


