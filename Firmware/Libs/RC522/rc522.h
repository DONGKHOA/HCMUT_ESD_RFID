/*
 * rc522.h
 *
 *  Created on: Oct 1, 2023
 *      Author: khoa dong
 */

#ifndef RC522_H_
#define RC522_H_

#include "stdint.h"

//Maximum length of the array
#define MAX_LEN 16

//MF522 Command word
#define PCD_IDLE              0x00               //NO action; Cancel the current command
#define PCD_AUTHENT           0x0E               //Authentication Key
#define PCD_RECEIVE           0x08               //Receive Data
#define PCD_TRANSMIT          0x04               //Transmit data
#define PCD_TRANSCEIVE        0x0C               //Transmit and receive data,
#define PCD_RESETPHASE        0x0F               //Reset
#define PCD_CALCCRC           0x03               //CRC Calculate

// Mifare_One card command word
# define PICC_REQIDL          0x26               // find the antenna area does not enter hibernation
# define PICC_REQALL          0x52               // find all the cards antenna area
# define PICC_ANTICOLL        0x93               // anti-collision
# define PICC_SElECTTAG       0x93               // election card
# define PICC_AUTHENT1A       0x60               // authentication key A
# define PICC_AUTHENT1B       0x61               // authentication key B
# define PICC_READ            0x30               // Read Block
# define PICC_WRITE           0xA0               // write block
# define PICC_DECREMENT       0xC0               // debit
# define PICC_INCREMENT       0xC1               // recharge
# define PICC_RESTORE         0xC2               // transfer block data to the buffer
# define PICC_TRANSFER        0xB0               // save the data in the buffer
# define PICC_HALT            0x50               // Sleep

//------------------MFRC522 Register---------------
//Page 0:Command and Status
#define     Reserved00            0x00
#define     CommandReg            0x01
#define     CommIEnReg            0x02
#define     DivlEnReg             0x03
#define     CommIrqReg            0x04
#define     DivIrqReg             0x05
#define     ErrorReg              0x06
#define     Status1Reg            0x07
#define     Status2Reg            0x08
#define     FIFODataReg           0x09
#define     FIFOLevelReg          0x0A
#define     WaterLevelReg         0x0B
#define     ControlReg            0x0C
#define     BitFramingReg         0x0D
#define     CollReg               0x0E
#define     Reserved01            0x0F
//Page 1:Command
#define     Reserved10            0x10
#define     ModeReg               0x11
#define     TxModeReg             0x12
#define     RxModeReg             0x13
#define     TxControlReg          0x14
#define     TxAutoReg             0x15
#define     TxSelReg              0x16
#define     RxSelReg              0x17
#define     RxThresholdReg        0x18
#define     DemodReg              0x19
#define     Reserved11            0x1A
#define     Reserved12            0x1B
#define     MifareReg             0x1C
#define     Reserved13            0x1D
#define     Reserved14            0x1E
#define     SerialSpeedReg        0x1F
//Page 2:CFG
#define     Reserved20            0x20
#define     CRCResultRegM         0x21
#define     CRCResultRegL         0x22
#define     Reserved21            0x23
#define     ModWidthReg           0x24
#define     Reserved22            0x25
#define     RFCfgReg              0x26
#define     GsNReg                0x27
#define     CWGsPReg              0x28
#define     ModGsPReg             0x29
#define     TModeReg              0x2A
#define     TPrescalerReg         0x2B
#define     TReloadRegH           0x2C
#define     TReloadRegL           0x2D
#define     TCounterValueRegH     0x2E
#define     TCounterValueRegL     0x2F
//Page 3:TestRegister
#define     Reserved30            0x30
#define     TestSel1Reg           0x31
#define     TestSel2Reg           0x32
#define     TestPinEnReg          0x33
#define     TestPinValueReg       0x34
#define     TestBusReg            0x35
#define     AutoTestReg           0x36
#define     VersionReg            0x37
#define     AnalogTestReg         0x38
#define     TestDAC1Reg           0x39
#define     TestDAC2Reg           0x3A
#define     TestADCReg            0x3B
#define     Reserved31            0x3C
#define     Reserved32            0x3D
#define     Reserved33            0x3E
#define     Reserved34            0x3F

typedef struct 
{
    uint32_t pin;
    void *port;
}MFRC522_gpio_t;


typedef struct 
{
    MFRC522_gpio_t reset;
    MFRC522_gpio_t nss;
    void *spi;
}MFRC522_t;

typedef enum
{
    MFRC_522_OK = 0,
    MFRC_522_ERR,
    MFRC_522_TIMEOUT,
    MFRC_522_UNEXPECTED_EVENT
}MFRC522_Status_t;

void MFRC522_Init(MFRC522_t *RC522_x);
MFRC522_Status_t MFRC522_Request(MFRC522_t *RC522_x, uint8_t reqMode, uint8_t *tagType);
MFRC522_Status_t MFRC522_Check(MFRC522_t *RC522_x, uint8_t* id);
MFRC522_Status_t MFRC522_Anticoll(MFRC522_t *RC522_x, uint8_t* serNum);
MFRC522_Status_t MFRC522_ToCard(MFRC522_t *RC522_x, uint8_t command, uint8_t *sendData, 
            uint8_t sendLen, uint8_t *backData, uint16_t *backLen);
void MFRC522_Write(MFRC522_t *RC522_x, uint8_t address, uint8_t value);
uint8_t MFRC522_Read(MFRC522_t *RC522_x, uint8_t address);
void MFRC522_Reset(MFRC522_t *RC522_x);
void MFRC522_Antenna_On(MFRC522_t *RC522_x);
void MFRC522_Antenna_Off(MFRC522_t *RC522_x);
void MFRC522_SetBIT(MFRC522_t *RC522_x, uint8_t reg, uint8_t bit_mask);
void MFRC522_ClearBIT(MFRC522_t *RC522_x, uint8_t reg, uint8_t bit_mask);
void MFRC522_Halt(void);

#endif /* RC522_H_ */