/***********************************************************************
File Name : 	hmc.h
Author :  		Name_006
Version :  		V1.0
Date :  		2011.12.10
Description :  	磁敏传感器头文件
Others：		参考网友Colin程序 特此感谢~~
				本店淘宝 http://shop65888055.taobao.com/
				专营 陀螺仪 加速度计 磁阻传感器 等传感器模块  欢迎选购
				QQ ： 353973314  姿态算法群 ： 199376386
**********************************************************************/
#ifndef  __HMC5883_H__
#define  __HMC5883_H__

#include "sys.h"
#include "IIC.h"
#include "math.h"
#include "stdio.h"

#define DEVICE_IIC_ADDR    0x1E
#define DEVICE_RD_ADDR     0x3C//0x3d
#define DEVICE_WR_ADDR     0x3C		//写

#define RadToDeg 57.29578

#define CFG_A       0x00
#define CFG_B       0x01
#define MODE        0x02
#define X_M         0x03
#define X_L         0x04
#define Z_M         0x05
#define Z_L         0x06
#define Y_M         0x07
#define Y_L         0x08
#define STATUS      0x09
#define IDF_A       0x0A
#define IDF_B       0x0B
#define IDF_C       0x0C

extern S_INT16_XYZ HMC5883_LAST;

void Read_HMC5883(S_INT16_XYZ*ptResult);


void HMC5883L_Printf(S_INT16_XYZ*ptResult);
void HMC5883_Init(void);
#endif

