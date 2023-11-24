/**
 * @file flash_memory.c
 */

/*********************
 *      INCLUDES
 *********************/
#include "main.h"
#include "flash_memory.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void FLASH_WriteByte(uint32_t addr , uint8_t data)
  {
  	FLASH_Unlock();		//unlocked flash memory
  	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR); //clear flags use flash memory
  	FLASH_EraseOptionBytes(); // clear content of the address
   	FLASH_ProgramOptionByteData(addr,data);	//save the content to the address of flash memory
  	FLASH_Lock();    // locked flash memory

  }

uint8_t FLASH_ReadByte(uint32_t addr)
  {
			uint8_t data;
			data=*((uint8_t*)addr);	//read 1 byte at the address of FLASH
			return data;
  }
/**********************
 *   STATIC FUNCTIONS
 **********************/
