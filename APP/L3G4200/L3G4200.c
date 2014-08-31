/*==============================================================================================*/
/*==============================================================================================*/

#include "L3G4200.h"
/*==============================================================================================*/
/*==============================================================================================*
**��� : L3G4200_Init
**�\�� : ��l��L3G4200
**��J : �L
**��X : �L
**�ϥ� : L3G4200_Init()
**==============================================================================================*/
/*==============================================================================================*/
u8 BUF[6];
S_INT16_XYZ 	GYRO_I,L3G4200_GYRO_LAST,GYRO_OFFSET;//���ٶȺͼ��ٶ�	
void L3G4200_Init( void )
{
	u8 L3G4200_Init_Data[6] = {
			0x8F,	/* L3G4200_CTRL_REG1	��ͨģʽ,����ʹ�ܣ� ODR:400Hz, CutOff:20, XYZEnable */
			0x00,	/* L3G4200_CTRL_REG2	��ͨģʽ����ͨ�˲���ֹƵ��30hz*/
			0x08,	/* L3G4200_CTRL_REG3	����׼��ʹ��*/
			0x10,	/* L3G4200_CTRL_REG4	�������£�������Range:2000dps */  //0X20  2000dps   0x10 500dps
			0x00,	/* L3G4200_CTRL_REG5	Ĭ��*/
			0x00	/* L3G4200_REFERENCE	Ĭ��*/
		};//�����Ժ�Ҫ��ָ�룬�߶�
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
        ptResult->X      = *( (int16_t *)(&tmp[0]) );      //�Ż�Ч������
        ptResult->Y      = *( (int16_t *)(&tmp[2]) );
        ptResult->Z      = *( (int16_t *)(&tmp[4]) );
    }
}
