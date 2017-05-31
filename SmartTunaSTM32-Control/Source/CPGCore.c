/*
 * CPGCore.c
 *
 * Created: 2014/4/21 9:53:09
 * Debug v1:2014/5/12 19:12:00
 *  Author: Meano
 */ 
#include "CPGCore.h"
/*
double	F;									//频率
double	Miu;								//决定频率改变的响应速度
*/
u8 CPGMode=0;

u8 ModeCount=0;
u8 ModeKeep=0;
u8 ModeTotal=0;
u8 ReadyGetData=0;

double	rNow[JointCount];						//摆动幅值
double	rLast[JointCount];						//摆动幅值记录
double	drNow[JointCount];						//摆动幅值一阶微分
double	drLast[JointCount];						//摆动幅值一阶微分记录
double	RSet[JointCount];						//设定摆动幅值

double	xNow[JointCount];						//偏置角
double	xLast[JointCount];						//偏置角记录
double	dxNow[JointCount];						//偏置角一阶微分
double	dxLast[JointCount];						//偏置角一阶微分记录
double	XSet[JointCount];						//设定偏置角

double	FaiBuffer[JointCount][JointCount];		//滞后角变化变量
double	FaiNow[JointCount];						//滞后角当前值
double	FaiLast[JointCount];					//滞后角上一次的值记录
double	dFaiNow[JointCount];					//滞后角一阶微分
double	dFaiLast[JointCount];					//滞后角一阶微分记录

double	Sita[JointCount];						//输出角

CPGPara CPGP;

const double FTem[16]=
{ 
	// 0,1.357,0.2,0.2,0.2,0.3,0.3,0.3,0.3,0.4,0.4,0.5,0.6,0.7,0.8,1.1
	//0,255,240,225,210,195,180,165,150,135,120,105,90,75,60,45
	0, 1 ,0.375+KF,0.4+KF,0.429+KF,0.462+KF,0.5+KF,0.546+KF,0.6+KF,0.66+KF,0.75+KF,0.858+KF,1.0+KF,1.2+KF,1.5+KF,2.0+KF
	//0,1.5,1.613,0.2232,0.2392,0.2577,0.2793,0.3049,0.3356,0.3731,0.4202,0.4808,0.5618,0.6757,0.8475,1.1364
};

//滞后角数组
const double FaiTem[JointCount][16]=
{
	{0      , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0    },
	{0      , 4.147, 0.698, 0.698, 0.698, 0.698, 0.698, 0.698, 0.698, 0.698, 0.698, 0.698, 0.698, 0.698, 0.698, 0.698},
	{0      , 2.953, 2.513, 2.513, 2.513, 2.513, 2.513, 2.513, 2.513, 2.513, 2.513, 2.513, 2.513, 2.513, 2.513, 2.513}
};

//16种摆幅速度
const float RTem[JointCount][16] = 
{
	{0.1*KR,	17,	0.2*KR   , 0.25*KR, 0.3*KR,0.35*KR,0.4*KR,0.45*KR,0.5*KR,0.55*KR,0.6*KR,0.65*KR,0.7*KR,0.75*KR,0.8*KR,0.85*KR},
	{0.1*KR+10,	7,  0.2*KR+10, 0.25*KR+10, 0.3*KR+10,0.35*KR+10,0.4*KR+10,0.45*KR+10,0.5*KR+10,0.55*KR+10,0.6*KR+10,0.65*KR+10,0.7*KR+10,0.75*KR+10,0.8*KR+10,0.85*KR+10},
	{0.1*KR+15, 17, 0.2*KR+15, 0.25*KR+15, 0.3*KR+15,0.35*KR+15,0.4*KR+15,0.45*KR+15,0.5*KR+15,0.55*KR+15,0.6*KR+15,0.65*KR+15,0.7*KR+15,0.75*KR+15,0.8*KR+15,0.85*KR+15},
};
//16种转弯值
const float XTem[JointCount][16] = 
{
	{-1.0*KX	, -0.9*KX	, -0.8*KX   , -0.7*KX   , -0.5*KX   ,-0.3*KX   ,-0.1*KX   ,0.0*KX   ,0.1*KX   ,/*0.2*KX   ,*/0.3*KX   ,0.5*KX   ,0.7*KX   ,0.8*KX   ,0.9*KX   ,1.0*KX   ,-70},
	{-1.0*KX-5	, -0.9*KX-5	, -0.8*KX-5 , -0.7*KX-5 , -0.5*KX-5 ,-0.3*KX-5 ,-0.1*KX-5 ,0.0*KX	,0.1*KX+5 ,/*0.2*KX-5 ,*/0.3*KX+5 ,0.5*KX+5 ,0.7*KX+5 ,0.8*KX+5 ,0.9*KX+5 ,1.0*KX+5 ,-75},
	{-1.0*KX-10	, -0.9*KX-10, -0.8*KX-10, -0.7*KX-10, -0.5*KX-10,-0.3*KX-10,-0.1*KX-10,0.0*KX	,0.1*KX+10,/*0.2*KX-10,*/0.3*KX+10,0.5*KX+10,0.7*KX+10,0.8*KX+10,0.9*KX+10,1.0*KX+10,-80},
};

void CPGInitialize(void){
    Reg.Speed=0;
    Reg.Direct=7;
    Reg.Mode=0;
    Reg.ServoOffset[0] = 0;
    Reg.ServoOffset[1] = 0;
    Reg.ServoOffset[2] = 0;
    ReadParameter();
}

void ResetData(void)
{
	u8 i;
	for (i=0;i<JointCount;i++)
	{
		rNow[i]=0;
		xNow[i]=0;
		rLast[i]=0;
		xLast[i]=0;
		drNow[i]=0;
		dxNow[i]=0;
		drLast[i]=0;
		dxLast[i]=0;
		FaiNow[i]=0;
		FaiLast[i]=0;
		dFaiNow[i]=0;
		dFaiLast[i]=0;
		Sita[i]=0;
		XSet[i]=0;
		RSet[i]=0;
	}
	FaiBuffer[0][1]=0;
	FaiBuffer[0][2]=0;
	FaiBuffer[1][0]=0;
	FaiBuffer[2][0]=0;
}
void CalculateR(double A,double* R)
{
	u8 i;
	for(i=0;i<JointCount;i++)
	{
		drNow[i] = drLast[i]   - 0.02*A*(A*(rLast[i]-R[i])+2*drLast[i]);
		rNow[i]   = rLast[i]    + 0.02*drNow[i];
		drLast[i]  = drNow[i];
		rLast[i]    = rNow[i];
	}
}

void CalculateX(double B,double* X)
{
	u8 i;
	for(i=0;i<JointCount;i++)
	{
		dxNow[i] = drLast[i]   - 0.02*(B*B*(xLast[i]-X[i])+2*B*dxLast[i]);
		xNow[i]   = xLast[i]    + 0.02*dxNow[i];
		dxLast[i]  = dxNow[i];
		xLast[i]    = xNow[i];
	}
}

void CalculateFai(double Miu,double F,double* Fai)
{
	u8 i,j;								//双循环矩阵处理计数量
	double D[JointCount],a,b;
	for(i=0;i<JointCount;i++)			//循环取值
	{
		FaiBuffer[0][i]=Fai[i];			//相位差
	}
	for (i=1;i<JointCount;i++)
	{
		for (j=0;j<JointCount;j++)
		{
			if (i!=j)
			{
				FaiBuffer[i][j]=FaiBuffer[0][j]-FaiBuffer[0][i];
			}
		}
	}
	for(i=0;i<JointCount;i++)
	{
		D[i]=(JointCount-1)*FaiLast[i];
	}
	for (i=0;i<JointCount;i++)
	{
		for (j=0;j<JointCount;j++)
		{
			if (j!=i)
			{
				D[i]=D[i]-FaiLast[j]-FaiBuffer[i][j];
			}
		}
	}
	a=0.02*Miu*Miu;
	b=0.08*Miu;
	for(i=0;i<JointCount;i++)
	{
		dFaiNow[i]=dFaiLast[i]-a*D[i]-b*(dFaiLast[i]-pi2*F);
		FaiNow[i]=FaiLast[i]+0.02*dFaiNow[i];
		dFaiLast[i]=dFaiNow[i];
		FaiLast[i]=FaiNow[i];
	}
}
double cos(double x)
{
	double y;   //计算返回值
	double z ;  //临时变量

	while(x>=pi2)
	{
		x=x-pi2;
	}
	while(x<0)
	{
		x=x+pi2;  
	}
	 
//********************计算cos值*********************
     if(x>=0&&x<pi/2)
	 {
		 y=1-0.5*x*x+x*x*x*x/24.0;
	 }
	 else if(x>=pi/2&&x<pi)
	 {
	     z=pi-x;
		 y=0.5*z*z-1-z*z*z*z/24.0;
	 }
	 else if(x>=pi&&x<1.5*pi)
	 {
	     z=x-pi;
		 y=0.5*z*z-1-z*z*z*z/24.0;
	 }
	 else
	 {
		z=2*pi-x;
		y=1-0.5*z*z+z*z*z*z/24.0;
	 }
	 return y;
}
/*
	                 Miu 响应速度; F 频率;	  A α;	       B β;	  R[JointCount]:摆幅;   X[JointCount]:偏置角;  Fai[JointCount]:滞后角;
*/
void CalculateSita(double Miu,double F,double A,double B,double* R,double* X,double* Fai)
{
	u8 i;
	double FaiPush[JointCount+1];											//多一个作为缓冲
	CalculateR(A,R);														//偏置角计算
	CalculateX(B,X);														//幅值计算
	CalculateFai(Miu,F,Fai);												//相位差计算
	memcpy((void *)FaiPush,(void *)FaiLast,JointCount*sizeof(double));		//Debug@5.12 2014 memcpy为拷贝字节 原程序限幅不起作用
	for (i=0;i<(JointCount-1);i++)
	{
		if (FaiPush[i]<FaiPush[i+1])
		{
			FaiPush[JointCount]=FaiPush[i];
			FaiPush[i]=FaiPush[i+1];
			FaiPush[i+1]=FaiPush[JointCount];
		}
	}
	if (FaiPush[JointCount-1]>pi2)
	{
		while (FaiPush[JointCount-1]>pi2)
		{
			for (i=0;i<JointCount;i++)
			{
				FaiLast[i]=FaiLast[i]-pi2;
				FaiNow[i]=FaiNow[i]-pi2;
				FaiPush[i]=FaiPush[i]-pi2;
			}
		}
	}
	for(i=0;i<JointCount;i++)
	{
		Sita[i]=KSita*(xNow[i]+rNow[i]*cos(FaiNow[i]));
	}
}

void CalculateSitaf(float Miu,float F,float A,float B,float* R,float* X,float* Fai)
{
	int i=0;
	double Rf[JointCount];
	double Xf[JointCount];
	double Faif[JointCount];
	for(i=0;i<JointCount;i++){
		Rf[i]=R[i];
		Xf[i]=X[i];
		Faif[i]=Fai[i];
	}
	CalculateSita(Miu,F,A,B,Rf,Xf,Faif);
}

void CalculateSitaLevel(double Miu,double A,double B,u8 V,u8 D)				//给入速度和方向 从表中取Fai F X R的值
{
	u8 i;
	double F,Fai[JointCount],X[JointCount],R[JointCount];
	F=FTem[V];																//频率
	for(i=0;i<JointCount;i++)												//循环取值
	{
		Fai[i]=FaiTem[i][V];												//相位差
		X[i]=XTem[i][D];													//偏置角
		R[i]=RTem[i][V];													//摆幅
	}
	CalculateSita(Miu,F,A,B,R,X,Fai);
}
