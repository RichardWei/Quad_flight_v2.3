/*==============================================================================================*/
/*==============================================================================================*/

#include "L3G4200.h"
/*==============================================================================================*/
/*==============================================================================================*
**ㄧ计 : L3G4200_Init
** : ﹍てL3G4200
**块 : 礚
**块 : 礚
**ㄏノ : L3G4200_Init()
**==============================================================================================*/
/*==============================================================================================*/
u8 BUF[6];
S_INT16_XYZ 	GYRO_I,L3G4200_GYRO_LAST,GYRO_OFFSET;//角速度和加速度	
void L3G4200_Init( void )
{
	u8 L3G4200_Init_Data[6] = {
			0x8F,	/* L3G4200_CTRL_REG1	普通模式,三轴使能， ODR:400Hz, CutOff:20, XYZEnable */
			0x00,	/* L3G4200_CTRL_REG2	普通模式，高通滤波截止频率30hz*/
			0x08,	/* L3G4200_CTRL_REG3	数据准备使能*/
			0x10,	/* L3G4200_CTRL_REG4	持续更新，满量程Range:2000dps */  //0X20  2000dps   0x10 500dps
			0x00,	/* L3G4200_CTRL_REG5	默认*/
			0x00	/* L3G4200_REFERENCE	默认*/
		};//这里以后要改指针，高端
	Single_Write(L3G4200_I2C_ADDR, L3G4200_CTRL_REG1,L3G4200_Init_Data[0]);delay_ms(1);
	Single_Write(L3G4200_I2C_ADDR, L3G4200_CTRL_REG2,L3G4200_Init_Data[1]);delay_ms(1);
	Single_Write(L3G4200_I2C_ADDR, L3G4200_CTRL_REG3,L3G4200_Init_Data[2]);delay_ms(1);
  Single_Write(L3G4200_I2C_ADDR, L3G4200_CTRL_REG4,L3G4200_Init_Data[3]);delay_ms(1);
	Single_Write(L3G4200_I2C_ADDR, L3G4200_CTRL_REG5,L3G4200_Init_Data[4]);delay_ms(1);
	Single_Write(L3G4200_I2C_ADDR, L3G4200_REFERENCE,L3G4200_Init_Data[5]);delay_ms(1);
}

void Read_L3G4200D(S_INT16_XYZ*ptResult)
{
  /*BUF[0]=Single_Read(L3G4200_OUT_X_L,L3G4200_I2C_ADDR);
   BUF[1]=Single_Read(L3G4200_OUT_X_H,L3G4200_I2C_ADDR);
   L3G4200_GYRO_LAST.X =(BUF[1]<<8)|BUF[0]; 
   BUF[2]=Single_Read(L3G4200_OUT_Y_L,L3G4200_I2C_ADDR);
   BUF[3]=Single_Read(L3G4200_OUT_Y_H,L3G4200_I2C_ADDR);
   L3G4200_GYRO_LAST.Y=	(BUF[3]<<8)|BUF[2];  
   BUF[4]=Single_Read(L3G4200_OUT_Z_L,L3G4200_I2C_ADDR);
   BUF[5]=Single_Read(L3G4200_OUT_Z_H,L3G4200_I2C_ADDR);
   L3G4200_GYRO_LAST.Z=	(BUF[5]<<8)|BUF[4];*/
	 uint8_t tmp[6];
    if(true == Mult_Read(L3G4200_I2C_ADDR,(L3G4200_OUT_X_L|0x80),tmp,6))
    {   //ptResult->gx    = (int16_t)((tmp[1]<<8)|tmp[0]);
        //ptResult->gy    = (int16_t)((tmp[3]<<8)|tmp[2]);
        //ptResult->gz    = (int16_t)((tmp[5]<<8)|tmp[4]);
        ptResult->X      = *( (int16_t *)(&tmp[0]) );      //优化效果明显
        ptResult->Y      = *( (int16_t *)(&tmp[2]) );
        ptResult->Z      = *( (int16_t *)(&tmp[4]) );
    }
}
