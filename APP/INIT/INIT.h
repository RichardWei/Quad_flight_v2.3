#ifndef __INIT_H
#define __INIT_H	 
#include "sys.h"
#include "delay.h"
#include "usart.h"

#include "IMU.h"
#include "L3G4200.h"
 
#include "ADXL345.h"
#include "HMC5883.h"
extern u8 Send_cnt,FLAG_Send;
extern u8 HMC_TIME;
//extern s16 ACC[20][3];
void Calculation_ACC_Offset(void);

void ReadData(void);

void ALL_INIT(void);


		 				    
#endif
