#include "spi_flash.h"
#include "r_cg_sci.h"
#include "r_cg_userdefine.h"
#include "r_cg_macrodriver.h"
#include  "commondefines.h"
#include "rtc.h"
#include "uart.h"

//uint32_t Temp = 0 , Temp0 = 0, Temp1 = 0, Temp2 = 0;
//uint8_t res = 0;
//uint8_t size0 = 0 , size1 = 0, size2 = 0, size3 = 0;
//uint8_t status_reg = 0 , status_reg_write_enable = 0;

volatile uint8_t flash_sen_wait_flag = 0;
volatile uint8_t flash_rec_wait_flag = 0;
static uint8_t w25qxx(uint8_t add);
static void w25qxx_WaitForWriteEnd(void);
static void w25qxx_writeenable(void);
void w25xx_stop(void);
void w25xx_init(void);
uint32_t w25xxJDEG_ID();
w25qxx_parm w25qxx_parm_t;

static void enable_cs_pin()
{
	PORT5.PODR.BIT.B6 = 0;
	delay(2);
}
static void disable_cs_pin()
{
	PORT5.PODR.BIT.B6 = 1;
	delay(2);
}
void w25xx_reset_chip(void)
{
	enable_cs_pin();
	w25qxx(RESET);
	disable_cs_pin();
	delay(2);
}
void w25xx_erase_chip(void)
{
	uint8_t status_reg_write_enable = 0;
	w25qxx_writeenable();

	enable_cs_pin();

	w25qxx(WRITESTAT1);                   // disabling block protection register
	w25qxx(0x03);
	disable_cs_pin();

	status_reg_write_enable = w25xx_read_status_reg(1);

	w25qxx_writeenable();

	enable_cs_pin();
	w25qxx(CHIPERASE);
	disable_cs_pin();

	w25qxx_WaitForWriteEnd();
}
static void w25qxx_writeenable(void)
{
  enable_cs_pin();
  w25qxx(WRITEENABLE);
  disable_cs_pin();
}
static uint8_t w25qxx(uint8_t add)
{
	uint8_t res = 0;
	R_SCI5_SPI_Master_Send_Receive(&add ,1 , &res, 1);
	while(flash_sen_wait_flag == 0 && flash_rec_wait_flag == 0);
    if(flash_sen_wait_flag == 1 && flash_rec_wait_flag == 1){
    	flash_sen_wait_flag = 0;
    	flash_rec_wait_flag = 0;
    	return res;
    }

    //}
}
void w25xx_init()
{
	R_SCI5_Start();
	delay(1);
	w25qxx_parm_t.JDEC_ID = w25xxJDEG_ID();
	uint8_t buff = w25qxx_parm_t.JDEC_ID & 0x000000FF;
	switch(buff)
	{

			case 0x17:	//	w25q64

				w25qxx_parm_t.sectorsize = 0x1000;
				w25qxx_parm_t.blockcount = 128;
				w25qxx_parm_t.sectorcount = w25qxx_parm_t.blockcount * 16; // each block contains 16 sectors
				w25qxx_parm_t.blocksize =   w25qxx_parm_t.sectorsize *16;
				w25qxx_parm_t.CapacityInKiloByte = (w25qxx_parm_t.sectorcount * w25qxx_parm_t.sectorsize)/1024;
				w25qxx_parm_t.pagesize = 256;
			break;
	}
}
void w25xx_stop()
{
	R_SCI5_Stop();
}
uint8_t w25xx_read_status_reg(uint8_t read_status_register)
{
	uint8_t status_reg = 0;
	enable_cs_pin();
	if(read_status_register == 1)
		w25qxx(READSTAT1);
	else if(read_status_register == 2)
		w25qxx(READSTAT2);
	else if(read_status_register == 3)
		w25qxx(READSTAT3);

   status_reg = w25qxx(W25QXX_DUMMY_BYTE);
   delay(1);
   disable_cs_pin();
   return status_reg;
}
uint32_t w25xxJDEG_ID()
{
	uint32_t Temp = 0 , Temp0 = 0, Temp1 = 0, Temp2 = 0;
	enable_cs_pin();

	w25qxx(JEDECID);
	Temp0 = w25qxx(W25QXX_DUMMY_BYTE);
	Temp1 = w25qxx(W25QXX_DUMMY_BYTE);
	Temp2 = w25qxx(W25QXX_DUMMY_BYTE);
	disable_cs_pin();

	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
	return Temp;
}
static void w25qxx_WaitForWriteEnd(void)
{
	uint8_t status_reg = 0;
	enable_cs_pin();
	w25qxx(READSTAT1);
	do{
	  status_reg = w25qxx(W25QXX_DUMMY_BYTE);
      delay(1);
    }
	while ((status_reg & 0x01) == 0x01);
	disable_cs_pin();
}
void w25xxwritebytes(uint8_t buff , uint32_t write_add)
{
	uint8_t write_size0 = 0 , write_size1 = 0, write_size2 = 0;
	write_size2 = (write_add >> 16) & 0xFF;
	write_size1 = (write_add >> 8) & 0xFF;
	write_size0 = (write_add) & 0xFF;

	w25qxx_WaitForWriteEnd();
	w25qxx_writeenable();

	enable_cs_pin();
	w25qxx(PAGEPROG);
	w25qxx(write_size2);
	w25qxx(write_size1);
	w25qxx(write_size0);
	w25qxx(buff);

	disable_cs_pin();
	w25qxx_WaitForWriteEnd();
}
void w25xxreadbytes(uint8_t *pbuff , uint32_t read_add)
{
	uint8_t read_size0 = 0 , read_size1 = 0, read_size2 = 0;
	read_size2 = (read_add >> 16) & 0xFF;
	read_size1 = (read_add >> 8) & 0xFF;
	read_size0 = (read_add) & 0xFF;

	enable_cs_pin();

	w25qxx(READDATA);
	w25qxx(read_size2);
	w25qxx(read_size1);
	w25qxx(read_size0);
	*pbuff = w25qxx(W25QXX_DUMMY_BYTE);

	disable_cs_pin();

}
void w25xxblockerase(uint32_t BlockAddr)
{
	uint8_t status_reg_write_enable = 0;

	BlockAddr = BlockAddr * w25qxx_parm_t.sectorsize * 16;
	w25qxx_writeenable();

	enable_cs_pin();

	w25qxx(WRITESTAT1);
	w25qxx(0x03);
	disable_cs_pin();

	status_reg_write_enable = w25xx_read_status_reg(1);

	enable_cs_pin();
	w25qxx(BLOCK64ERASE);
	w25qxx((BlockAddr & 0xFF0000) >> 16);
	w25qxx((BlockAddr & 0xFF00) >> 8);
	w25qxx(BlockAddr & 0xFF);

	disable_cs_pin();

	w25qxx_WaitForWriteEnd();
}
void w25xxerasesector(uint32_t SectorAddr)
{
	uint8_t status_reg_write_enable = 0;
	w25qxx_WaitForWriteEnd();
	SectorAddr = SectorAddr *0x1000;
	w25qxx_writeenable();

	enable_cs_pin();

	w25qxx(WRITESTAT1);
	w25qxx(0x03);
	disable_cs_pin();

	status_reg_write_enable = w25xx_read_status_reg(1);

	enable_cs_pin();
	w25qxx(SECTORERASE);
	w25qxx((SectorAddr & 0xFF0000) >> 16);
	w25qxx((SectorAddr & 0xFF00) >> 8);
	w25qxx(SectorAddr & 0xFF);

	disable_cs_pin();

	w25qxx_WaitForWriteEnd();

}
void w25xxwritepage(uint8_t *pBuffer, uint32_t Page_Address, uint32_t OffsetInByte, uint32_t NumByteToWrite_up_to_PageSize)
{
	Page_Address = (Page_Address * w25qxx_parm_t.pagesize) + OffsetInByte;
	if(((NumByteToWrite_up_to_PageSize+OffsetInByte) > w25qxx_parm_t.pagesize)||(NumByteToWrite_up_to_PageSize==0))

			NumByteToWrite_up_to_PageSize = w25qxx_parm_t.pagesize-OffsetInByte;

	if((OffsetInByte+NumByteToWrite_up_to_PageSize) >w25qxx_parm_t.pagesize)

			NumByteToWrite_up_to_PageSize = w25qxx_parm_t.pagesize - OffsetInByte;

	w25qxx_WaitForWriteEnd();
	w25qxx_writeenable();

//	enable_cs_pin();
//	w25qxx(WRITESTAT1);
//	w25qxx(0x03);
//	disable_cs_pin();

	enable_cs_pin();
	w25qxx(PAGEPROG);

	w25qxx((Page_Address & 0xFF0000) >> 16);
	w25qxx((Page_Address & 0xFF00) >> 8);
	w25qxx(Page_Address & 0xFF);

	for(uint8_t i = 0; i < NumByteToWrite_up_to_PageSize; i++)
	{
			w25qxx(pBuffer[i]);

	}
	disable_cs_pin();
	w25qxx_WaitForWriteEnd();
	delay(10);
}
void w25xxreadpage(uint8_t *pBuffer	,uint32_t Page_Address, uint32_t OffsetInByte, uint32_t NumByteToWrite_up_to_PageSize)
{
	Page_Address = (Page_Address * w25qxx_parm_t.pagesize) + OffsetInByte;
	if(((NumByteToWrite_up_to_PageSize+OffsetInByte) > w25qxx_parm_t.pagesize)||(NumByteToWrite_up_to_PageSize==0))

			NumByteToWrite_up_to_PageSize = w25qxx_parm_t.pagesize-OffsetInByte;

	if((OffsetInByte+NumByteToWrite_up_to_PageSize) >w25qxx_parm_t.pagesize)

			NumByteToWrite_up_to_PageSize = w25qxx_parm_t.pagesize - OffsetInByte;

	enable_cs_pin();
	w25qxx(READDATA);

	w25qxx((Page_Address & 0xFF0000) >> 16);
	w25qxx((Page_Address & 0xFF00) >> 8);
	w25qxx(Page_Address & 0xFF);

	for(uint8_t i = 0; i < NumByteToWrite_up_to_PageSize; i++)
	{
			pBuffer[i] = w25qxx(W25QXX_DUMMY_BYTE);
	}
	disable_cs_pin();
	w25qxx_WaitForWriteEnd();
	delay(10);
}

