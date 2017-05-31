#include "UART.h"
#define TXBufferLength 64
#define RXBufferLength 64
#define WR 1
#define RD 0
u8 DeviceID = 0x20;
typedef struct
{
	u8 *TXData;
	u16 TXLength;
	u16 TXCount;
	u8	fTX;
	u8 *RXData;
	u16 RXLength;
    u16 RXCount;
    u8  RXType;
    u8  fRX;
}USARTData;
USARTData USART1Data,USART2Data;
u8 RXData[4]={0x00,0x00,0x00,0x00};
u8 RXCount = 0;
void UARTInitialize(void)
{
	USART_InitTypeDef USART_InitStructure;               			//串口设置默认参数声明
	GPIO_InitTypeDef GPIO_InitStructure;							//GPIO设置默认参数声明
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 , ENABLE);			//RCC时钟设置，串口1时钟开启
	USART1Data.fTX = 1;												//串口数据参数初始化
    USART1Data.TXCount = 0;
    USART1Data.TXLength = 0;
    USART1Data.TXData = (u8 *)malloc(sizeof(u8)*(TXBufferLength));  //申请发送数据空间
    USART1Data.RXCount = 0;
    USART1Data.RXLength = 0;
    USART1Data.RXData = (u8 *)malloc(sizeof(u8)*(RXBufferLength));  //申请响应长度的数据空间
	
    USART_InitStructure.USART_BaudRate = 115200;                    //波特率115200 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; 	//字长8位 
    USART_InitStructure.USART_StopBits = USART_StopBits_1;			//1位停止字节 
    USART_InitStructure.USART_Parity = USART_Parity_No;				//无奇偶校验 
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//打开Rx接收和Tx发送功能 
	USART_Init(USART1, &USART_InitStructure);						//串口参数初始化
    USART_ClearITPendingBit(USART1, USART_IT_TC);
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	USART_ITConfig(USART1, USART_IT_TC, ENABLE);					//串口中断开启
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                       //PA9 TX1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;					//默认OD门复用输出，需上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;				//GPIO速度10MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);                          //TX初始化 
    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                     	//PA10 RX1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 			//浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;				//GPIO速度10MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);                          //RX初始化 
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);			//RCC时钟设置，串口1时钟开启
    
    USART2Data.fTX = 1;												//串口数据参数初始化
    USART2Data.TXCount = 0;
    USART2Data.TXLength = 0;
    USART2Data.TXData = (u8 *)malloc(sizeof(u8)*(TXBufferLength));  //申请发送数据空间
    USART2Data.RXCount = 0;
    USART2Data.RXLength = 0;
    USART2Data.RXData = (u8 *)malloc(sizeof(u8)*(RXBufferLength));  //申请响应长度的数据空间
    
    USART_InitStructure.USART_BaudRate = 115200;                    //串口2 波特率115200 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; 	//串口2 字长8位 
    USART_InitStructure.USART_StopBits = USART_StopBits_1;			//串口2 1位停止字节 
    USART_InitStructure.USART_Parity = USART_Parity_No;				//串口2 无奇偶校验 
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//串口2 无流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//串口2 打开Rx接收和Tx发送功能 
	USART_Init(USART2, &USART_InitStructure);						//串口2参数初始化
    USART_ClearITPendingBit(USART2, USART_IT_TC);                   //串口2清空发送中断
    USART_ClearITPendingBit(USART2, USART_IT_RXNE);                 //串口2清空接收
	USART_ITConfig(USART2, USART_IT_TC, ENABLE);					//串口2发送中断开启
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);                  //串口2接收中断开启
    USART_Cmd(USART2, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;                       //PA2 TX2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;					//默认OD门复用输出，需上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;				//GPIO速度10MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);                          //TX初始化 
    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;                     	//PA3 RX2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 			//浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;				//GPIO速度10MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);                          //RX初始化
	
}

void USART1Send(u8 *Send,u16 Length)
{
	if(USART1Data.fTX==1)
	{
        USART1Data.TXLength=Length;
		//USART1Data.TXData=(u8 *)malloc(sizeof(u8)*USART1Data.TXLength);
		for(USART1Data.TXCount=0;USART1Data.TXCount<USART1Data.TXLength;USART1Data.TXCount++)
		{
			*(USART1Data.TXData+USART1Data.TXCount)=*(Send+USART1Data.TXCount);
		}
		USART_SendData(USART1,(u16)*(USART1Data.TXData));
		USART1Data.TXCount=1;
		USART1Data.fTX=0;
	}
}

u8 AddressCheck(u8 *DeviceAddress){
    if(*DeviceAddress == 0xFF){                             //ID获取包
        return 2;
    }
    if(*DeviceAddress != DeviceID){                         //判断ID是否匹配
        return 0;
    }
    return 1;                                               //ID匹配
}

void Packet1Send(u8 Length){
    if(USART1Data.fTX==1)
	{
        USART1Data.TXLength=Length;
		USART_SendData(USART1,(u16)*(USART1Data.TXData));
		USART1Data.TXCount=1;
		USART1Data.fTX=0;
	}
}

void Packet1Data(u8 PacketType, u8 *PacketBuffer, u8 PacketLength){
    u8 PacketCount = 0;
    u8 PacketCheck = 0;
    *(USART1Data.TXData + 0) = 0x21;
    *(USART1Data.TXData + 1) = (PacketType<<6)|PacketLength;
    for(; PacketCount < PacketLength; PacketCount++){
        *(USART1Data.TXData + 2 + PacketCount) = *(PacketBuffer + PacketCount);
        PacketCheck += *(PacketBuffer + PacketCount);
    }
    *(USART1Data.TXData + 2 + PacketLength) = ~PacketCheck;
    *(USART1Data.TXData + 3 + PacketLength) = 0x52;
    Packet1Send(PacketLength + 4);
}

void Packet2Send(u8 Length){
    if(USART2Data.fTX==1)
	{
        USART2Data.TXLength=Length;
		USART_SendData(USART2,(u16)*(USART2Data.TXData));
		USART2Data.TXCount=1;
		USART2Data.fTX=0;
	}
}

void Packet2Data(u8 PacketType, u8 *PacketBuffer, u8 PacketLength){
    u8 PacketCount = 0;
    u8 PacketCheck = 0;
    *(USART2Data.TXData + 0) = 0x21;
    *(USART2Data.TXData + 1) = (PacketType<<6)|PacketLength;
    for(; PacketCount < PacketLength; PacketCount++){
        *(USART2Data.TXData + 2 + PacketCount) = *(PacketBuffer + PacketCount);
        PacketCheck += *(PacketBuffer + PacketCount);
    }
    *(USART2Data.TXData + 2 + PacketLength) = ~PacketCheck;
    *(USART2Data.TXData + 3 + PacketLength) = 0x52;
    Packet2Send(PacketLength + 4);
}

void PacketWrite(u8 *PacketBuffer,u8 Length){
    u8 RegAddress;
    u8 ReCount = 0;
    if(!AddressCheck(PacketBuffer)){
        Packet2Data(WR, PacketBuffer, Length);
        return;
    }else if(AddressCheck(PacketBuffer) == 2){
        for(ReCount = 1; ReCount < Length; ReCount++){
            if((*(PacketBuffer + ReCount)&0xF0) == (DeviceID&0xF0)){
                DeviceID = *(PacketBuffer + ReCount) + 1;
                *(PacketBuffer + ReCount) = DeviceID;
                break;
            }
        }
        if(ReCount == Length){                                          //循环完成，没有找到同样的舱体
            *(PacketBuffer + Length) = DeviceID;                        //插入自己的舱体ID到数据包末尾
            Packet2Data(WR, PacketBuffer, Length+1);                    //发送到下一级舱体
        }else if(ReCount < Length){
            Packet2Data(WR, PacketBuffer, Length+1);
        }
    }else{
        RegAddress = *(PacketBuffer + 1);
        for(ReCount = 2; ReCount < Length; ReCount++)
            *((u8 *)&Reg + RegAddress + ReCount - 2) = *(PacketBuffer + ReCount);
        Packet1Data(WR, PacketBuffer, Length);
    }
}

void PacketRead(u8 *PacketBuffer,u8 Length){
     u8 RegAddress;
			u8 ReCount = 0;
		u8 RegLength = 0;
    if(!AddressCheck(PacketBuffer)){
        Packet1Data(RD, PacketBuffer, Length);
        return;
    }else if(AddressCheck(PacketBuffer) == 2){
        Packet1Data(RD, PacketBuffer, Length);
        return;
    }else{
        RegAddress = *(PacketBuffer + 1);
				RegLength = *(PacketBuffer + 2);
        for(ReCount = 2; ReCount < RegLength + 2; ReCount++)
            *(PacketBuffer + ReCount) = *((u8 *)&Reg + RegAddress + ReCount - 2);
        Packet2Data(RD, PacketBuffer, RegLength + 2);
    }
}


void USART1_IRQHandler(void)                            		//串口1中断 
{ 
	u8 RXBuffer,i;                                                       	
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)		//判断发生接收中断 
	{
        RXBuffer=USART_ReceiveData(USART1);						//接收数据
//        if(RXCount==0){
//            if(RXBuffer == 0xAA){
//                RXCount = 1;
//                RXData[0] = 0xAA; 
//            }
//        }else if(RXCount > 0 && RXCount < 3){
//            RXData[RXCount] = RXBuffer;
//            RXCount++;
//        }else if(RXCount == 3){
//            RXData[3] = RXBuffer;
//            if(RXBuffer == 0xFC){
//                if(RXData[1]==0x91){
//                    if((RXData[2]&0xF0) == 0xA0 ){
//                        //LServo = ((int)(RXData[2]&0x0F)-7)*200;
//                    }else if((RXData[2]&0xF0) == 0xC0){
//                        //RServo = ((int)(RXData[2]&0x0F)-7)*200;
//                    }else if((RXData[2]&0xF0) == 0x50){
//                        //LMotor = abs((int)(RXData[2]&0x0F)-7)*5000;
//                        if((RXData[2]&0x0F)==7){
//                            GPIO_ResetBits(GPIOB,GPIO_Pin_8|GPIO_Pin_9);
//                        }else if((RXData[2]&0x0F)>7){
//                            GPIO_ResetBits(GPIOB,GPIO_Pin_8);
//                            GPIO_SetBits(GPIOB,GPIO_Pin_9);
//                        }else if((RXData[2]&0x0F)<7){
//                            GPIO_ResetBits(GPIOB,GPIO_Pin_9);
//                            GPIO_SetBits(GPIOB,GPIO_Pin_8);
//                        }
//                    }else if((RXData[2]&0xF0) == 0x60){
//                        //RMotor = abs((int)(RXData[2]&0x0F)-7)*5000;
//                        if((RXData[2]&0x0F)==7){
//                            GPIO_ResetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_13);
//                        }else if((RXData[2]&0x0F)<7){
//                            GPIO_ResetBits(GPIOB,GPIO_Pin_12);
//                            GPIO_SetBits(GPIOB,GPIO_Pin_13);
//                        }else if((RXData[2]&0x0F)>7){
//                            GPIO_ResetBits(GPIOB,GPIO_Pin_13);
//                            GPIO_SetBits(GPIOB,GPIO_Pin_12);
//                        }
//                    }
//                }
//            }
//            RXCount = 0;
//        }

        //新版数据包
        if(USART1Data.RXCount == 0){
            if(RXBuffer == 0x21){                                                       //包头判断
                USART1Data.RXCount = 1;                                                 //首字节记录
            }
        }else if(USART1Data.RXCount == 1){                                              //记录第二个关键字
            USART1Data.RXLength = RXBuffer&0x3F;                                        //接收数据长度（<63）
            USART1Data.RXType = RXBuffer>>6;                                            //接收数据类型
            USART1Data.RXCount = 2;                                                     //地址包第二步判断
        }else if(USART1Data.RXCount > 1){                                               //开始记录数据
            if(USART1Data.RXCount < USART1Data.RXLength+3){                             //判断是否到包尾
                *(USART1Data.RXData+USART1Data.RXCount-2) = RXBuffer;                   //记录数据
                USART1Data.RXCount++;                                                   //计数器累加
            }else if(USART1Data.RXCount == USART1Data.RXLength+3){                      //到包尾后的处理
                if(RXBuffer == 0x52){                                                   //接收包尾正确
                    RXBuffer = 0;                                                       
                    for(i = 0; i < USART1Data.RXLength; i++)
                    {
                        RXBuffer += *(USART1Data.RXData+i);
                    }
                    RXBuffer = ~RXBuffer;                                               //计算实际数据校验结果
                    if(RXBuffer == *(USART1Data.RXData+USART1Data.RXLength)){           //判断校验是否成功
                        switch(USART1Data.RXType){                                      
                            case 0:
                                PacketRead(USART1Data.RXData,USART1Data.RXLength);
                                break;                                                  //读包 
                            case 1:
                                PacketWrite(USART1Data.RXData,USART1Data.RXLength);
                                break;                                                  //写包
                            case 2:;break;
                            case 3:;break;
                            default:;break;
                        }
                    }
                }
                USART1Data.RXType = 0;
                USART1Data.RXLength = 0;
                USART1Data.RXCount = 0;
            }
        }
        
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);		    //清除中断标志 
	}
	if (USART_GetITStatus(USART1, USART_IT_TC) != RESET)		//判断发生发送中断 
	{
		if(USART1Data.fTX==0)
		{
			if(USART1Data.TXCount==USART1Data.TXLength)
			{
				//free(USART1Data.TXData);
                USART1Data.fTX=1;
			}
			else
			{
				USART_SendData(USART1,(u16)*(USART1Data.TXData+USART1Data.TXCount));
				USART1Data.TXCount++;
			}
		}
        USART_ClearITPendingBit(USART1, USART_IT_TC);           //清除中断标志
	} 
}

void USART2_IRQHandler(void)                            		//串口1中断 
{ 
	u16 RXBuffer;                                                       	
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)		//判断发生接收中断 
	{
        RXBuffer=USART_ReceiveData(USART2);						//接收数据
        //USART1Send(&RXBuffer,1);
		//USART_SendData(USART1,RXBuffer);
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);		    //清除中断标志 
	}
	if (USART_GetITStatus(USART2, USART_IT_TC) != RESET)		//判断发生发送中断 
	{
        USART_ClearITPendingBit(USART2, USART_IT_TC);           //清除中断标志
        if(USART2Data.fTX==0)
		{
			if(USART2Data.TXCount==USART2Data.TXLength)
			{
                USART2Data.fTX=1;
			}
			else
			{
				USART_SendData(USART2,(u16)*(USART2Data.TXData+USART2Data.TXCount));
				USART2Data.TXCount++;
			}
		}
	} 
}
