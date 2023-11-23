/**
 * @file flash_memory.h
 *
 */
#ifndef __FLASH_MEMOTY_H
#define __FLASH_MEMOTY_H

/*********************
 *      INCLUDES
 *********************/
#include "stm32f1xx.h"
#include "main.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/

void FLASH_WriteByte(uint32_t addr , uint8_t data);
uint8_t FLASH_ReadByte(uint32_t addr);

#endif
