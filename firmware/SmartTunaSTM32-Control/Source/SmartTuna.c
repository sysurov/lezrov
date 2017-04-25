#include "SmartTuna.h"

RegStruct Reg;

void RCCInitialize(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
}

void GPIOInitialize(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;						//LED
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;					
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
    //GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST,ENABLE);
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6;					
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);		
}

int main(void){
    RCCInitialize();                //RCC初始化
    CPGInitialize();
    Timer2Initialize();             //定时器2 初始化PWM产生
    Timer3Initialize(500);          //定时器3 500us定时器中断
    UARTInitialize();               //串口初始化
    NVICInitialize();               //中断初始化
    GPIOInitialize();
    GPIO_SetBits(GPIOC,GPIO_Pin_13);
    GPIO_WriteBit(GPIOB,GPIO_Pin_6,0);
    GPIO_WriteBit(GPIOB,GPIO_Pin_5,0);
    //GPIO_SetBits(GPIOB,GPIO_Pin_5|GPIO_Pin_6);
    while(1);
}
