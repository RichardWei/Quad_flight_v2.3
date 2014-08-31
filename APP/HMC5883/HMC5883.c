/***********************************************************************
File Name : 	hmc.c
Author :  		Name_006
Version :  		V1.0
Date :  		2011.12.10
Description :  	磁敏传感器部分 IC选用的是 HMC5883L
Others：		参考网友Colin程序 特此感谢~~
				本店淘宝 http://shop65888055.taobao.com/
				专营 陀螺仪 加速度计 磁阻传感器 等传感器模块  欢迎选购
				QQ ：353973314  姿态算法群 ： 199376386
**********************************************************************/
#include "HMC5883.h"//xc=68.6236 yc=-43.3018
S_INT16_XYZ HMC5883_LAST;
//static u8 MSD[9];
// 初始化HMC5883L，仅供参考，要根据自己需求进行调整
void HMC5883_Init(void)
{
   /* unsigned char byData[4];
    //CFG_A=0xC1, CFG_B=0xA0, MODE=0x00可测试出模块焊接后芯片是否被烧坏，此时如果输出X,Y,Z数据为400-1000的数据则芯片是正常的
    byData[0] = 0x78; //50HZ//byData[0] = 0x70; //0xC0; 	// SELF-TEST时为0xC1	   		 //平均采样数8  15hz  
    byData[1] = 0x00; //byData[1] = 0x20; // 0x40; 	//0x20; // SELF-TEST时为0xA0 //范围+_1.9Ga   增益820高斯
    byData[2] = 0x00; // MODE // SELF-TEST时为0X00    设置为连续测量模式  			 //
    //Multiple_write(CFG_A,3,DEVICE_WR_ADDR,byData);   //连续写三个寄存器*/
  //Single_Write(0x14,0x00,0x3c);
	//Single_Write(0x40,0x01,0x3c);
	//Single_Write(0x00,0x02,0x3c);
	Single_Write(0x3C,0x00,0x18);//0x14
	Single_Write(0x3C,0x02,0x00);
}


void Read_HMC5883(S_INT16_XYZ*ptResult)
{

    uint8_t tmp[6];
    int32_t s32Val;
    if(true == Mult_Read(DEVICE_WR_ADDR,X_M,tmp,6))   //多读读出传感器数据
    {

		  	ptResult->X  = (int16_t)((tmp[0] << 8) | tmp[1])-87;//matlab校准 求的值
        s32Val = (int16_t)((tmp[4] << 8) | tmp[5])+26;  
			  s32Val = (s32Val*10403)/10000;
        ptResult->Y    = (int16_t)s32Val;
        ptResult->Z    = (int16_t)((tmp[2] << 8) | tmp[3]);
    }  
}

void HMC5883L_Printf(S_INT16_XYZ*ptResult)
{
    int16_t x,y;
    float angle;
    u16 hu;
    x = ptResult->X;
    y = ptResult->Y;
    //求出方向
    if(x>0x7fff)x-=0xffff;
    if(y>0x7fff)y-=0xffff;
    //LED1_ON();
    angle= atan2(y,x) * (180 / 3.14159265) + 180;   //160us计算时间 
	  hu=(u16)angle;
	  //LED1_OFF();
    //ptResult->ha = (int16_t)(angle*10);   // 得到方向精确到0.1°
    printf("HMC5883L:\thx: %4d,\thy: %4d,\thz: %4d,\t%4d\n\r",
            ptResult->X, ptResult->Y, ptResult->Z,hu);
}
