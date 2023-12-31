#include "rc522.h"

extern SPI_HandleTypeDef hspi1;

void Write_MFRC522(uint8_t addr, uint8_t val)
{
  uint8_t addr_bits = (((addr << 1) & 0x7E));
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&hspi1, &addr_bits, 1, 100);
  HAL_SPI_Transmit(&hspi1, &val, 1, 100);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}

uint8_t Read_MFRC522(uint8_t addr)
{
  uint8_t rx_bits;
  uint8_t addr_bits = (((addr << 1) & 0x7E) | 0x80);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&hspi1, &addr_bits, 1, 100);
  HAL_SPI_Receive(&hspi1, &rx_bits, 1, 100);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

  return (uint8_t)rx_bits;
}

void SetBitMask(uint8_t reg, uint8_t mask)
{
  uint8_t tmp;
  tmp = Read_MFRC522(reg);
  Write_MFRC522(reg, (tmp | mask)); // set bit mask
}

void ClearBitMask(uint8_t reg, uint8_t mask)
{
  uint8_t tmp;
  tmp = Read_MFRC522(reg);
  Write_MFRC522(reg, (tmp & (~mask))); // clear bit mask
}

void AntennaOn(void)
{
  SetBitMask(TxControlReg, 0x03);
}

void AntennaOff(void)
{
  ClearBitMask(TxControlReg, 0x03);
}

void MFRC522_Reset(void)
{
  Write_MFRC522(CommandReg, PCD_RESETPHASE);
}

void MFRC522_Init(void)
{
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 1);
  MFRC522_Reset();

  // Timer: TPrescaler*TreloadVal/6.78MHz = 24ms

  Write_MFRC522(TModeReg, 0x8D);
  Write_MFRC522(TPrescalerReg, 0x3E);

  Write_MFRC522(TReloadRegH, 0x00);
  Write_MFRC522(TReloadRegL, 30);

  Write_MFRC522(RFCfgReg, 0x70);
  Write_MFRC522(TxAutoReg, 0x40);
  Write_MFRC522(ModeReg, 0x3D);

  // turn antenna on
  AntennaOn();
}

uint8_t MFRC522_Request(uint8_t reqMode, uint8_t *TagType)
{
  uint8_t status;
  uint backBits; // The received data bits

  Write_MFRC522(BitFramingReg, 0x07); // TxLastBists = BitFramingReg[2..0]

  TagType[0] = reqMode;

  status = MFRC522_ToCard(PCD_TRANSCEIVE, TagType, 1, TagType, &backBits);
  if ((status != MI_OK) || (backBits != 0x10))
  {
    status = MI_ERR;
  }

  return status;
}

uint8_t MFRC522_ToCard(uint8_t command, uint8_t *sendData, uint8_t sendLen, uint8_t *backData, uint *backLen)
{
  uint8_t status = MI_ERR;
  uint8_t irqEn = 0x00;
  uint8_t waitIRq = 0x00;
  uint8_t lastBits;
  uint8_t n;
  uint16_t i;

  switch (command)
  {
  case PCD_AUTHENT: // Certification cards close
  {
    irqEn = 0x12;
    waitIRq = 0x10;
    break;
  }
  case PCD_TRANSCEIVE: // Transmit FIFO data
  {
    irqEn = 0x77;
    waitIRq = 0x30;
    break;
  }
  default:
    break;
  }

  Write_MFRC522(CommIEnReg, irqEn | 0x80); // Interrupt request
  ClearBitMask(CommIrqReg, 0x80);          // Clear all interrupt request bit
  SetBitMask(FIFOLevelReg, 0x80);          // FlushBuffer=1, FIFO Initialization

  Write_MFRC522(CommandReg, PCD_IDLE); // NO action; Cancel the current command

  // Writing data to the FIFO
  for (i = 0; i < sendLen; i++)
  {
    Write_MFRC522(FIFODataReg, sendData[i]);
  }

  // Execute the command
  Write_MFRC522(CommandReg, command);
  if (command == PCD_TRANSCEIVE)
  {
    SetBitMask(BitFramingReg, 0x80); // StartSend=1,transmission of data starts
  }

  // Waiting to receive data to complete
  i = 2000; // i according to the clock frequency adjustment, the operator M1 card maximum waiting time 25ms
  do
  {
    // CommIrqReg[7..0]
    // Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
    n = Read_MFRC522(CommIrqReg);
    i--;
  } while ((i != 0) && (!(n & 0x01)) && (!(n & waitIRq)));

  ClearBitMask(BitFramingReg, 0x80); // StartSend=0

  if (i != 0)
  {
    if (!(Read_MFRC522(ErrorReg) & 0x1B))
    { // BufferOvfl Collerr CRCErr ProtecolErr
      status = MI_OK;
      if (n & irqEn & 0x01)
      {
        status = MI_NOTAGERR; // ??
      }

      if (command == PCD_TRANSCEIVE)
      {
        n = Read_MFRC522(FIFOLevelReg);
        lastBits = Read_MFRC522(ControlReg) & 0x07;
        if (lastBits)
        {
          *backLen = (n - 1) * 8 + lastBits;
        }
        else
        {
          *backLen = n * 8;
        }

        if (n == 0)
        {
          n = 1;
        }
        if (n > MAX_LEN)
        {
          n = MAX_LEN;
        }

        // Reading the received data in FIFO
        for (i = 0; i < n; i++)
        {
          backData[i] = Read_MFRC522(FIFODataReg);
        }
      }
    }
    else
    {
      status = MI_ERR;
    }
  }
  return status;
}