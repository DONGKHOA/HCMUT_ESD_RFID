/*
 * rc522.c
 *
 *  Created on: Oct 1, 2023
 *      Author: khoa dong
 */


#include "rc522.h"
#include "main.h"

void MFRC522_Init(MFRC522_t *RC522_x)
{
//    HAL_GPIO_WritePin(RC522_x->reset.port, RC522_x->reset.pin, GPIO_PIN_SET);
//    HAL_Delay(100);

    MFRC522_Reset(RC522_x);

#if 0
  // Timer: TPrescaler*TreloadVal/6.78MHz = 24msx
   MFRC522_Write(RC522_x, TModeReg, 0x80); // 0x8D);      // Tauto=1; f(Timer) = 6.78MHz/TPreScaler
   MFRC522_Write(RC522_x, TPrescalerReg, 0xA9); //0x34); // TModeReg[3..0] + TPrescalerReg
   MFRC522_Write(RC522_x, TReloadRegL, 0x03); //30);
   MFRC522_Write(RC522_x, TReloadRegH, 0xE8); //0);
   MFRC522_Write(RC522_x, TxAutoReg, 0x40);     // force 100% ASK modulation
   MFRC522_Write(RC522_x, ModeReg, 0x3D);       // CRC Initial value 0x6363

  // interrupts, still playing with these
  //  MFRC522_Write(RC522_x,CommIEnReg, 0xFF);
  //  MFRC522_Write(RC522_x,DivlEnReg, 0xFF);

#else
    MFRC522_Write(RC522_x, TModeReg, 0x8D);
	MFRC522_Write(RC522_x, TPrescalerReg, 0x3E);
	MFRC522_Write(RC522_x, TReloadRegL, 30);           
	MFRC522_Write(RC522_x, TReloadRegH, 0);

	/* 48dB gain */
	MFRC522_Write(RC522_x, RFCfgReg, 0x70);
	
	MFRC522_Write(RC522_x, TxAutoReg, 0x40);
	MFRC522_Write(RC522_x, ModeReg, 0x3D);
#endif
  // turn antenna on
  MFRC522_Antenna_On(RC522_x);
}

MFRC522_Status_t MFRC522_Request(MFRC522_t *RC522_x, uint8_t reqMode, uint8_t *tagType)
{
    MFRC522_Status_t status;
    uint16_t backBits;   // The received data bits
    MFRC522_Write(RC522_x, BitFramingReg, 0x07);   // TxLastBists = BitFramingReg[2..0]

    tagType[0] = reqMode;

    status = MFRC522_ToCard(RC522_x, PCD_TRANSCEIVE, tagType, 1, tagType, &backBits);
    if ((status != MFRC_522_OK) || (backBits != 0x10)) 
    {
        status = MFRC_522_ERR;
    }

  return status;
}

MFRC522_Status_t MFRC522_Check(MFRC522_t *RC522_x, uint8_t* id)
{
	MFRC522_Status_t status;
	//Find cards, return card type
	status = MFRC522_Request(RC522_x, PICC_REQIDL, id);	
	if (status == MFRC_522_OK) {
		//Card detected
		//Anti-collision, return card serial number 4 bytes
		status = MFRC522_Anticoll(RC522_x, id);
	}
//	MFRC522_Halt();			//Command card into hibernation

	return status;
}

MFRC522_Status_t MFRC522_Anticoll(MFRC522_t *RC522_x, uint8_t* serNum)
{
	MFRC522_Status_t status;
	uint8_t i;
	uint8_t serNumCheck = 0;
	uint16_t unLen;

	MFRC522_Write(RC522_x, BitFramingReg, 0x00);		//TxLastBists = BitFramingReg[2..0]

	serNum[0] = PICC_ANTICOLL;
	serNum[1] = 0x20;
	status = MFRC522_ToCard(RC522_x, PCD_TRANSCEIVE, serNum, 2, serNum, &unLen);

	if (status == MFRC_522_OK) 
    {
		//Check card serial number
		for (i = 0; i < 4; i++) 
        {   
			serNumCheck ^= serNum[i];
		}
		if (serNumCheck != serNum[i]) 
        {   
			status = MFRC_522_ERR;    
		}
	}
	return status;
}

MFRC522_Status_t MFRC522_ToCard(MFRC522_t *RC522_x, uint8_t command, uint8_t *sendData, 
            uint8_t sendLen, uint8_t *backData, uint16_t *backLen)
{
    MFRC522_Status_t status; 
    uint8_t irqEn = 0x00;
    uint8_t waitIrq = 0x00;
    uint8_t temp;
    uint8_t lastBits;

    switch (command)
    {
        case PCD_AUTHENT:     // Certification cards close
        {
            irqEn = 0x12;
            waitIrq = 0x10;
            break;
        }
        case PCD_TRANSCEIVE:  // Transmit FIFO data
        {
            irqEn = 0x77;
            waitIrq = 0x30;
            break;
        }
        default:
        break;
    }

    MFRC522_Write(RC522_x, CommIEnReg, irqEn | 0x80);       // Interrupt request
    MFRC522_ClearBIT(RC522_x, CommIrqReg, 0x80);          // Clear all interrupt request bit
    MFRC522_SetBIT(RC522_x, FIFOLevelReg, 0x80);          // FlushBuffer=1, FIFO Initialization

    MFRC522_Write(RC522_x, CommandReg, PCD_IDLE);         // NO action; Cancel the current command

    // Writing data to the FIFO
    for (uint8_t i = 0; i < sendLen; i++)
    {
        MFRC522_Write(RC522_x, FIFODataReg, sendData[i]);
    }
    
    // Execute the command
    if (command == PCD_TRANSCEIVE)
    {
       MFRC522_SetBIT(RC522_x, BitFramingReg, 0x80);      // StartSend=1,transmission of data starts
    }
    
    // Waiting to receive data to complete
    for (uint16_t i = 0; i < 2000; i++)
    {
        // i according to the clock frequency adjustment, the operator M1 card maximum waiting time 25ms
        temp = MFRC522_Read(RC522_x, CommIrqReg);
        if (!(temp & 0x01) && !(temp & waitIrq))
        {
            break;
        }

        if (i == 1999)
        {
            status = MFRC_522_TIMEOUT;
            return status;
        }
    }
    
    MFRC522_ClearBIT(RC522_x, BitFramingReg, 0x80);      // StartSend=0

    if (!(MFRC522_Read(RC522_x, ErrorReg) & 0x1B))   // BufferOvfl Collerr CRCErr ProtecolErr
    {
        status =  MFRC_522_OK;

        if (temp & irqEn & 0x01)
        {
            status = MFRC_522_UNEXPECTED_EVENT;
            return status;
        }

        if (command == PCD_TRANSCEIVE)
        {
            temp = MFRC522_Read(RC522_x, FIFOLevelReg);
            lastBits = MFRC522_Read(RC522_x, ControlReg) & 0x07;
            if (lastBits)
            {
                *backLen = (temp - 1) * 8 + lastBits;
            }
            else
            {
                *backLen = temp * 8;
            }

            if (temp == 0)
            {
                temp = 1;
            }

            if (temp > MAX_LEN)
            {
                temp = MAX_LEN;
            }

            for (uint8_t i = 0; i < temp; i++)
            {
                backData[i] = MFRC522_Read(RC522_x, FIFODataReg);
            }
        }
    }
    else
    {
        status = MFRC_522_ERR;
    }
    return status;
}

void MFRC522_Write(MFRC522_t *RC522_x, uint8_t address, uint8_t value)
{
    uint8_t address_spi = (address << 1) & 0x7E;
    HAL_GPIO_WritePin(RC522_x->nss.port, RC522_x->nss.pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(RC522_x->spi, &address_spi, 1, 500);
    HAL_SPI_Transmit(RC522_x->spi, &value, 1, 500);
    HAL_GPIO_WritePin(RC522_x->nss.port, RC522_x->nss.pin, GPIO_PIN_SET);
}

uint8_t MFRC522_Read(MFRC522_t *RC522_x, uint8_t address)
{
    uint8_t address_spi = ((address << 1) & 0x7E) | 0x80;
    uint8_t rxBuf = 0x00;

    HAL_GPIO_WritePin(RC522_x->nss.port, RC522_x->nss.pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(RC522_x->spi, &address_spi, 1, 500);
    HAL_SPI_Receive(RC522_x->spi, &rxBuf, 1, 500);
    HAL_GPIO_WritePin(RC522_x->nss.port, RC522_x->nss.pin, GPIO_PIN_SET);
    return rxBuf;
}

void MFRC522_Reset(MFRC522_t *RC522_x)
{
    MFRC522_Write(RC522_x, CommandReg, PCD_RESETPHASE);
}

void MFRC522_Antenna_On(MFRC522_t *RC522_x)
{
    MFRC522_SetBIT(RC522_x, TxControlReg, 0x03);
}

void MFRC522_Antenna_Off(MFRC522_t *RC522_x)
{
    MFRC522_ClearBIT(RC522_x, TxControlReg, 0x03);
}

void MFRC522_SetBIT(MFRC522_t *RC522_x, uint8_t reg, uint8_t bit_mask)
{
    uint8_t temp = 0x00;
    temp = MFRC522_Read(RC522_x, reg);
    MFRC522_Write(RC522_x, reg, temp | bit_mask);      //set bit
}

void MFRC522_ClearBIT(MFRC522_t *RC522_x, uint8_t reg, uint8_t bit_mask)
{
    uint8_t temp = 0x00;
    temp = MFRC522_Read(RC522_x, reg);
    MFRC522_Write(RC522_x, reg, temp & (~bit_mask));      //clear bit
}

//void MFRC522_Halt(void)
//{
//	uint16_t unLen;
//	uint8_t buff[4];
//
//	buff[0] = PICC_HALT;
//	buff[1] = 0;
//	MFRC522_CalculateCRC(buff, 2, &buff[2]);
//
//	MFRC522_ToCard(PCD_TRANSCEIVE, buff, 4, buff, &unLen);
//}
