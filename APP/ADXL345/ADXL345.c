/*==============================================================================================*/
/*==============================================================================================*/

#include "ADXL345.h"


u8 Buf[6];

S_INT16_XYZ 	ACC_AVG, ADXL345_ACC_LAST, ACC_OFFSET; //陀螺仪和加速度计偏移

void ADXL345_Init( void )
{
	//在13位模式下，1 LSB 代表3.9 mg
	u8 ADXL345_Init_Data[7] = {
		0x09,	/* ADXL345_DATA_FORMAT	灵敏度: 7.8mg/LSB */
		0x0C,	/* ADXL345_BW_RATE			转换频率 400Hz											*/
		0x08,	/* ADXL345_POWER_CTL		筿方家Α : 代秖家Α												*/
		0x80,	/* ADXL345_INT_ENABLE		ㄏ DATA_READY い耞											*/
		0xFC,	/* ADXL345_OFSX					X 熬簿秖 : -4	3.9*17/15.6 = 4.25					*/
		0xFF,	/* ADXL345_OFSX					Y 熬簿秖 : -1  3.9* 4/15.6 = 1						*/
		0x09	/* ADXL345_OFSZ					Z 熬簿秖 : +9	(1000-3.9*219)/15.6 = 9.35	*/
	};
	Single_Write(ADXL345_I2C_ADDR, ADXL345_DATA_FORMAT, ADXL345_Init_Data[0]); delay_ms(1);
	Single_Write(ADXL345_I2C_ADDR, ADXL345_BW_RATE,     ADXL345_Init_Data[1]); delay_ms(1);
	Single_Write(ADXL345_I2C_ADDR, ADXL345_POWER_CTL,   ADXL345_Init_Data[2]); delay_ms(1);
	Single_Write(ADXL345_I2C_ADDR, ADXL345_INT_ENABLE,  ADXL345_Init_Data[3]); delay_ms(1);
	Single_Write(ADXL345_I2C_ADDR, ADXL345_OFSX ,       ADXL345_Init_Data[4]); delay_ms(1);
	Single_Write(ADXL345_I2C_ADDR, ADXL345_OFSY,        ADXL345_Init_Data[5]); delay_ms(1);
	Single_Write(ADXL345_I2C_ADDR, ADXL345_OFSZ,        ADXL345_Init_Data[6]); delay_ms(1);
}




//****************************************
void Read_ADXL345(S_INT16_XYZ*ptResult)
{
	uint8_t tmp[6];
	if (true == Mult_Read(ADXL345_I2C_ADDR, ADXL345_DATAX0, tmp, 6))
	{
		ptResult->X      = *( (int16_t *)(&tmp[0]) );      //优化效果明显
		ptResult->Y      = *( (int16_t *)(&tmp[2]) );
		ptResult->Z      = *( (int16_t *)(&tmp[4]) );
	}
}
//*****************************************







