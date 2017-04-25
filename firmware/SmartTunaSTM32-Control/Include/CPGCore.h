/*
 * CPGCore.h
 *
 * Created: 2014/4/21 9:53:58
 *  Author: Meano
 */ 
#ifndef CPGCoreH
#define CPGCoreH
#include "SmartTuna.h"
#include <string.h>
#define pi	3.141592654		//定义PI
#define pi2	6.283185307		//定义2*pi
#define KR	25				//用于设定摆动幅度
#define KX	55				//用于设定转弯参数
#define KF	0.2				//用于设定频率增加量
#define KSita 20.0			//Sita与PWM分量转换
#define	JointCount 3		//关节数目
#define ScarlCPG 100.0

extern u8 CPGMode;
extern u8 ModeCount;
extern u8 ModeKeep;
extern u8 ModeTotal;
extern u8 ReadyGetData;

extern double	F;										//频率
extern double	Miu;									//决定频率改变的响应速度
extern double	RSet[JointCount];						//设定摆动幅值
extern double	XSet[JointCount];						//设定偏置角
extern double	xNow[JointCount];						//xNow
extern double	FaiBuffer[JointCount][JointCount];		//滞后角变化变量
extern double	Sita[JointCount];

extern const float XTem[JointCount][16];

typedef struct CPGParameter
{
	float Miu;
	float F;
	float A;
	float B;
	float R[3];
	float X[3];
	float Fai[3];
}CPGPara;

extern CPGPara CPGP;

extern void ResetData(void);
extern void CPGInitialize(void);
extern void CalculateX(double B,double* X);
extern void CalculateSitaLevel(double Miu,double A,double B,u8 V,u8 D);
extern void CalculateSita(double Miu,double F,double A,double B,double* R,double* X,double* Fai);
extern void CalculateSitaf(float Miu,float F,float A,float B,float* R,float* X,float* Fai);
#endif /* CPGCORE_H_ */
