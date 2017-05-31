#include "ADC.h"
void ADCInitialize(void)
{
	ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);						//ADC1时钟开启
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;					
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);		
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;						//独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;							//非连续多通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;						//不连续转换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;		//转换不受外界决定
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 					//数据位右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;									//扫描的通道数
	ADC_Init(ADC1,&ADC_InitStructure);										//ADC1参数初试化
	ADC_RegularChannelConfig(ADC1,ADC_Channel_7,1,ADC_SampleTime_1Cycles5);//ADC1通道3顺序1
	ADC_Cmd(ADC1,ENABLE);
}
u16 ADC1GetConvertValue(void)
{
	u16 ADCValue; 
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==RESET);					//
	ADCValue=ADC_GetConversionValue(ADC1);
	return ADCValue;
}
u32 GetRandomValue(int CountLimit)
{
	u32 RandomSeed=0;
	u8 i=0;
	for(i=0;i<4;i++)
	{
		ADC_SoftwareStartConvCmd(ADC1,ENABLE);
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==RESET);
		RandomSeed|=(ADC_GetConversionValue(ADC1)&0xFF)<<(i*8);
	}
	srand(RandomSeed);
	return rand()%CountLimit;
}
