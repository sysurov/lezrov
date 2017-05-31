#include "UART.h"
#define TXBufferLength 128
#define RXBufferLength 128
#define WR 1
#define RD 0
u8 DeviceID = 0xF0;
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
USARTData USART1Data;
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

void PacketWrite(u8 *PacketBuffer,u8 Length){
    u8 RegAddress;
    u8 ReCount = 0;
    if(!AddressCheck(PacketBuffer))
        return;
    else if(AddressCheck(PacketBuffer) == 2){
        for(ReCount = 1; ReCount < Length; ReCount++){
            if((*(PacketBuffer + ReCount)&0xF0) == (DeviceID&0xF0)){
                DeviceID = *(PacketBuffer + ReCount) + 1;
                *(PacketBuffer + ReCount) = DeviceID;
                break;
            }
        }
        if(ReCount == Length){                                          //循环完成，没有找到同样的舱体
            *(PacketBuffer + Length) = DeviceID;                        //插入自己的舱体ID到数据包末尾
            Packet1Data(WR, PacketBuffer, Length+1);                    //发送到总线上传回上位机
        }                        
    }else{
        RegAddress = *(PacketBuffer + 1);
        for(ReCount = 2; ReCount < Length; ReCount++)
            *((u8 *)&Reg + RegAddress + ReCount - 2) = *(PacketBuffer + ReCount);
				if(RegAddress == 0x0A){
					SaveParameter();
				}
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
        Packet1Data(RD, PacketBuffer, RegLength + 2);
    }
}

void USART1_IRQHandler(void)                            		//串口1中断 
{ 
	u8 RXBuffer,i,Rc;                                                       	
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
//                    if((RXData[2]&0xF0) == 0xD0 ){
//                        Speed = RXData[2]&0x0F;
//                        Rc = Speed + 0x10;
//                        USART1Send(&Rc,1);
//                    }else if((RXData[2]&0xF0) == 0xE0){
//                        Direct = RXData[2]&0x0F;
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
                //free(USART1Data.RXData);
            }
        }
        //USART1Send(&RXBuffer,1);
		//USART_SendData(USART1, RXBuffer);				 		//发送数据
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

void USART2_IRQHandler(void)                            		//串口2中断 
{ 
	u16 RXBuffer;                                                       	
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)		//判断发生接收中断 
	{
        RXBuffer=USART_ReceiveData(USART2);						//接收数据
		USART_SendData(USART1,RXBuffer);
        
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);		    //清除中断标志 
	}
	if (USART_GetITStatus(USART2, USART_IT_TC) != RESET)		//判断发生发送中断 
	{
        USART_ClearITPendingBit(USART2, USART_IT_TC);           //清除中断标志
	} 
}
