#include "SmartTuna.h"

RegStruct Reg;

void RCCInitialize(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
}

int main(void){
    RCCInitialize();                //RCC初始化
    //Timer2Initialize();             //定时器2 初始化PWM产生
    UARTInitialize();               //串口初始化
    NVICInitialize();               //中断初始化
    Timer3Initialize(500);          //定时器3 500us定时器中断
    while(1);
}
