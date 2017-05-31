#include "MPU.h"
u8			MPUData[14];
u8			fOffset = 1;
MPUStruct	GyroOffset,AccelOffset;
MPUStruct	Accel,Gyro;
void MPUReadData(void)
{
    IICReadData(MPUAddress,MPUData,MPU6050_RA_ACCEL_XOUT_H,14);
}
void MPU6050_setClockSource(uint8_t source)
{
	IICwriteBits(MPUAddress, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, source);
}
void MPU6050_setFullScaleGyroRange(uint8_t range)
{
	IICwriteBits(MPUAddress, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range);
}
void MPU6050_setFullScaleAccelRange(uint8_t range)
{
	IICwriteBits(MPUAddress, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
}
void MPU6050_setSleepEnabled(uint8_t enabled)
{
	IICwriteBit(MPUAddress, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, enabled);
}

void MPU6050_setI2CMasterModeEnabled(uint8_t enabled)
{
	IICwriteBit(MPUAddress, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_I2C_MST_EN_BIT, enabled);
}

void MPU6050_setI2CBypassEnabled(uint8_t enabled)
{
	IICwriteBit(MPUAddress, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, enabled);
}

void MPU6050_setDLPF(uint8_t mode)
{
	IICwriteBits(MPUAddress, MPU6050_RA_CONFIG, MPU6050_CFG_DLPF_CFG_BIT, MPU6050_CFG_DLPF_CFG_LENGTH, mode);
}
void Delay_ms(u16 time)
{
	u16 i=0;
	while(time--)
	{
		i=12000;
		while(i--);
	}
}
void MPUInitialize(void)
{
	Delay_ms(200);
	MPU6050_setSleepEnabled(0);
	Delay_ms(200);
	MPU6050_setClockSource(MPU6050_CLOCK_PLL_XGYRO);
	Delay_ms(50);
	MPU6050_setFullScaleGyroRange(MPU6050_GYRO_FS_2000);
	Delay_ms(50);
	MPU6050_setFullScaleAccelRange(MPU6050_ACCEL_FS_4);
	Delay_ms(50);
	MPU6050_setDLPF(MPU6050_DLPF_BW_42);
	Delay_ms(50);
	MPU6050_setI2CMasterModeEnabled(0);
	Delay_ms(50);
	MPU6050_setI2CBypassEnabled(1);
	Delay_ms(50);
	AccelOffset.Z=-1500;
}
void MPUCalculateData(void)
{
	MPUReadData();
	Accel.X	=((((int16_t)MPUData[0]) << 8) | MPUData[1])  - AccelOffset.X;
	Accel.Y	=((((int16_t)MPUData[2]) << 8) | MPUData[3]) - AccelOffset.Y;
	Accel.Z	=((((int16_t)MPUData[4]) << 8) | MPUData[5]) - AccelOffset.Z;
	Reg.MPUX = Accel.X;
	Reg.MPUY = Accel.Y;
	Reg.MPUZ = Accel.Z;
	Gyro.X	=((((int16_t)MPUData[8]) << 8) | MPUData[9]) - GyroOffset.X;
	Gyro.Y	=((((int16_t)MPUData[10]) << 8) | MPUData[11]) - GyroOffset.Y;
	Gyro.Z	=((((int16_t)MPUData[12]) << 8) | MPUData[13]) - GyroOffset.Z;
}
