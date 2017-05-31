#ifndef SMARTTUNAH
#define SMARTTUNAH
#include "stdlib.h"

#include "stm32f10x.h"
//#include "stm32f10x_adc.h"
//#include "stm32f10x_bkp.h"
//#include "stm32f10x_can.h"
//#include "stm32f10x_crc.h"
//#include "stm32f10x_dac.h"
//#include "stm32f10x_dbgmcu.h"
//#include "stm32f10x_dma.h"
#include "stm32f10x_exti.h"
//#include "stm32f10x_flash.h"
//#include "stm32f10x_fsmc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_i2c.h"
//#include "stm32f10x_iwdg.h"
//#include "stm32f10x_pwr.h"
#include "stm32f10x_rcc.h"
//#include "stm32f10x_rtc.h"
//#include "stm32f10x_sdio.h"
//#include "stm32f10x_spi.h"
#include "stm32f10x_tim.h" 
#include "stm32f10x_usart.h"
//#include "stm32f10x_wwdg.h"
#include "misc.h"

#include "NVIC.h"
#include "Timer.h"
#include "UART.h"
#include "IIC.h"
#include "MPU.h"

typedef struct
{
	int16_t MPUX;
	int16_t MPUY;
	int16_t MPUZ;
}RegStruct;

extern RegStruct Reg;

#endif
