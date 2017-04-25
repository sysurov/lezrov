#include "NVIC.h"

void NVICInitialize(void)
{
//	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
    //定时器2中断
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;					//定时器中断2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;			//中断抢占优先级1	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;				//中断响应优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;					//使能中断
	NVIC_Init(&NVIC_InitStructure);	
	
    //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;					//定时器中断3
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;			//中断抢占优先3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;				//中断响应优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;					//使能中断
	NVIC_Init(&NVIC_InitStructure);									//初始化
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;				//通道设置为串口1中断 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 		//中断抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				//中断响应优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//打开中断 
	NVIC_Init(&NVIC_InitStructure);									//初始化 
	
	    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;				//通道设置为串口1中断 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 		//中断抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;				//中断响应优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//打开中断 
	NVIC_Init(&NVIC_InitStructure);									//初始化 
	
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;				//通道设置为串口1中断 
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 		//中断抢占优先级1
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//中断响应优先级2
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//打开中断 
//	NVIC_Init(&NVIC_InitStructure);									//初始化 
	
}





