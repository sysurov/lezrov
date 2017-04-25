#include "IIC.h"
void IICInitialize(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	/* Configure IO connected to IIC*********************/
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 =0x09;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 200000;

	I2C_Cmd(I2C1, ENABLE);   

	I2C_Init(I2C1, &I2C_InitStructure);
}
void IICWriteByte(unsigned char id,unsigned char write_address,unsigned char byte)
{
	I2C_GenerateSTART(I2C1,ENABLE);
	//产生起始条件
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	//等待ACK
	I2C_Send7bitAddress(I2C1,id,I2C_Direction_Transmitter);
	//向设备发送设备地址
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	//等待ACK
	I2C_SendData(I2C1, write_address);
	//寄存器地址
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	//等待ACK
	I2C_SendData(I2C1, byte);
	//发送数据
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	//发送完成
	I2C_GenerateSTOP(I2C1, ENABLE);
	//产生结束信号
}
unsigned char IICReadByte(unsigned char  id, unsigned char read_address)
{  
	unsigned char temp; 	
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
	//等待I2C
	I2C_GenerateSTART(I2C1, ENABLE);
	//产生起始信号
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	//EV5
	I2C_Send7bitAddress(I2C1, id, I2C_Direction_Transmitter);
	//发送地址
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	//EV6
	I2C_Cmd(I2C1, ENABLE);
	//重新设置可以清除EV6
	I2C_SendData(I2C1, read_address);  
	//发送读的地址
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	//EV8 
	I2C_GenerateSTART(I2C1, ENABLE);
	//重新发送
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	//EV5
	I2C_Send7bitAddress(I2C1, id, I2C_Direction_Receiver);
	//发送读地址
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	//EV6  
	I2C_AcknowledgeConfig(I2C1, DISABLE);
	I2C_GenerateSTOP(I2C1, ENABLE);
	//关闭应答和停止条件产生
	while(!(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)));     
	temp = I2C_ReceiveData(I2C1);
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	return temp;
}
void IICReadData(u8 slaveAddr, u8* pBuffer, u8 readAddr, u16 NumByteToRead)
{
 // ENTR_CRT_SECTION();

  /* While the bus is busy */
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

  /* Send START condition */
  I2C_GenerateSTART(I2C1, ENABLE);

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

  /* Send MPU6050 address for write */
  I2C_Send7bitAddress(I2C1, slaveAddr, I2C_Direction_Transmitter); 

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  /* Clear EV6 by setting again the PE bit */
  I2C_Cmd(I2C1, ENABLE);

  /* Send the MPU6050's internal address to write to */
  I2C_SendData(I2C1, readAddr);

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* Send STRAT condition a second time */
  I2C_GenerateSTART(I2C1, ENABLE);

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

  /* Send MPU6050 address for read */
  I2C_Send7bitAddress(I2C1, slaveAddr, I2C_Direction_Receiver);

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

  /* While there is data to be read */
  while(NumByteToRead)
  {
    if(NumByteToRead == 1)
    {
      /* Disable Acknowledgement */
      I2C_AcknowledgeConfig(I2C1, DISABLE);

      /* Send STOP Condition */
      I2C_GenerateSTOP(I2C1, ENABLE);
    }

    /* Test on EV7 and clear it */
    if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
    {
      /* Read a byte from the MPU6050 */
      *pBuffer = I2C_ReceiveData(I2C1);

      /* Point to the next location where the byte read will be saved */
      pBuffer++;

      /* Decrement the read bytes counter */
      NumByteToRead--;
    }
  }

  /* Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig(I2C1, ENABLE);
//  EXT_CRT_SECTION();

}

void IICwriteBit(uint8_t dev, uint8_t reg, uint8_t bitNum, uint8_t data){
	u8 b[2];
	IICReadData(dev,b,reg,1);
	if(data==1)
	b[1]|=(uint8_t)(1<<bitNum);
	else
	b[1]&=(uint8_t)~(1<<bitNum);
	IICWriteByte(dev,reg,b[1]);
}
void IICwriteBits(uint8_t dev,uint8_t reg,uint8_t bitStart,uint8_t length,uint8_t data)
{
	u8 b[2],mask;
	IICReadData(dev,b,reg,1);
	b[1]=b[0];
	mask = (uint8_t)((0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1));
	data <<= (8 - length);
	data >>= (7 - bitStart);
	b[1] &= mask;
	b[1] |= data;
	IICWriteByte(dev,reg,b[1]);
}
