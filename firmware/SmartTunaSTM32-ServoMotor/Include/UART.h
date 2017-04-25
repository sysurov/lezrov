#ifndef UARTH
#define UARTH
#include "SmartTuna.h"
extern void UARTInitialize(void);
extern void USART1Send(u8 *Send,u16 Length);
extern void SendDec(int16_t Dec);
extern void SendAngle(float Pitch,float Roll);
#endif
