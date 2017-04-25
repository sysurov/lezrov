#include "Timer.h"

void Timer1Initialize(void)
{
//	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;     
//	TIM_OCInitTypeDef TIM_OCInitStructure;     
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
							
	
	//频率设定
    TIM_BaseInitStructure.TIM_Period = 1000-1;     
	TIM_BaseInitStructure.TIM_Prescaler = 24-1;     
	TIM_BaseInitStructure.TIM_ClockDivision = 0; 
    TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;     
	TIM_BaseInitStructure.TIM_RepetitionCounter = 0;     
	TIM_TimeBaseInit(TIM1, &TIM_BaseInitStructure);     
	//启用ARR的影子寄存器
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	//CH1PWM频率设定
    /*
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;     
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;     
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;     
	TIM_OCInitStructure.TIM_Pulse = 0;     
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	*/
	//启用CH1影子寄存器
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	//死区设定
	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;     
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;     
	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;     
	TIM_BDTRInitStructure.TIM_DeadTime = 0x00;
    TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable; 
    TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High; 
    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
	TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);
	//开启定时器
	
    TIM_Cmd(TIM1, ENABLE);
}
void Timer2Initialize(void)
{
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
	TIM_OCInitTypeDef		TIM_OCInitStructure;
	GPIO_InitTypeDef        GPIO_InitStructure;
    
	//关闭Jtag
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	//四路PWM GPIO 端口重映射
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM2,ENABLE);
	
	//PWM GPIO设定
	//GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;//|GPIO_Pin_3; //四路PWM输出
	//GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	//GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	//GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_10|GPIO_Pin_11;		//四路PWM输出
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;							//四路PWM输出
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	//APB定时器2允许
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	//频率设定
	TIM_TimeBaseStructure.TIM_Period = 40000;						//计数上线		20ms
	TIM_TimeBaseStructure.TIM_Prescaler = 36-1;						//pwm时钟分频	2MHZ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;						
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//向上计数
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	/* PWM1 Mode configuration: Channel */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 3000;								//初始占空比
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	//CH1
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	//CH2
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	//CH3
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
	//CH4
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
	//ARR
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	//开启定时器
	TIM_Cmd(TIM2, ENABLE);
}
void Timer3Initialize(u16 us)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;		//基础设置，时基和比较输出设置，由于这里只需定时，所以不用OC比较输出
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	TIM_DeInit(TIM3);
	TIM_TimeBaseStructure.TIM_Period=us;				//装载值
	TIM_TimeBaseStructure.TIM_Prescaler=72-1;
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM3,ENABLE);
}

u8 fPlus=0;
void TIM2_IRQHandler(void)
{
    //u8 toSend[1];
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);
        if(fPlus==0)
		{
			TIM2->CCR1+=300;
			if((TIM2->CCR1)>=4400)
			{
				fPlus=1;
			}
		}
		else
		{
			TIM2->CCR1-=300;
			if((TIM2->CCR1)<=1600)
			{
				fPlus=0;
			}
		}
        //toSend[0]='A';
        //USART1Send(toSend,1);
	}
}

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM3,TIM_FLAG_Update);
		MPUCalculateData();
	}
}
