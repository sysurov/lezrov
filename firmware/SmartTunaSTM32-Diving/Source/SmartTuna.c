#include "SmartTuna.h"

RegStruct Reg;
#define TapOn GPIO_WriteBit(GPIOA,GPIO_Pin_0,1);
#define TapOff GPIO_WriteBit(GPIOA,GPIO_Pin_0,0);
#define PumpOff GPIO_WriteBit(GPIOA,GPIO_Pin_8,1);
#define PumpOn GPIO_WriteBit(GPIOA,GPIO_Pin_8,0);

void RCCInitialize(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
}

void GPIOInitialize(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;						//LED
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_12|GPIO_Pin_13;					
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);					
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_0;					
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    PumpOff
    TapOn
}

int main(void){
    RCCInitialize();                //RCC初始化
    GPIOInitialize();
    ADCInitialize();
    //Timer2Initialize();             //定时器2 初始化PWM产生
    Timer3Initialize(40000);          //定时器3 500us定时器中断
    UARTInitialize();               //串口初始化
    NVICInitialize();               //中断初始化
    Reg.Pump = 0;
    while(1);
}
