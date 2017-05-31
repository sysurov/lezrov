#ifndef OnChipH
#define OnChipH
#include "SmartTuna.h"
#define ConfigAddress 0x08008000
#define VERSION 1
#if defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL) || defined (STM32F10X_XL)
#define FLASH_PAGE_SIZE ((uint16_t)0x800)
#else
#define FLASH_PAGE_SIZE ((uint16_t)0x400)
#endif

extern u8 FlashWriteBytes(u32 Address, u8 *Buffer, u16 ByteCount);
extern void FlashReadBytes(u32 Address, u8 *Buffer, u16 ByteCount);
extern void FlashReadWords(u32 Address, u32 *Buffer, u16 WordCount);
extern void SaveParameter(void);
extern void ReadParameter(void);
typedef struct Parameter
{
	int32_t ServoOffset[3];
	int32_t Version;
}Config;
#endif
