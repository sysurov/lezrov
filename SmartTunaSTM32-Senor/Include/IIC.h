#ifndef IICH
#define IICH
#include "SmartTuna.h"
extern void IICInitialize(void);
extern void IICWriteByte(unsigned char id,unsigned char write_address,unsigned char byte);
extern void IICReadData(u8 slaveAddr, u8* pBuffer, u8 readAddr, u16 NumByteToRead);
extern void IICwriteBit(uint8_t dev, uint8_t reg, uint8_t bitNum, uint8_t data);
extern void IICwriteBits(uint8_t dev,uint8_t reg,uint8_t bitStart,uint8_t length,uint8_t data);
#endif
