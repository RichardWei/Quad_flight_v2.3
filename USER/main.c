#include "include.h"//所有头文件全部包含了
int main(void)
{
	u8 ReadID1 = 0, ReadID2 = 0;
	ALL_INIT();
	delay_ms(50);
	/* while(1)
	{
		Read_HMC5883(&HMC5883_LAST);
		printf("%d,%d\n",HMC5883_LAST.X,HMC5883_LAST.Y);
		delay_ms(20);
	 }*/
	printf("硬件初始化完成\n");
	ReadID1 = Single_Read(ADXL345_I2C_ADDR, ADXL345_ID);
	ReadID2 = Single_Read(L3G4200_I2C_ADDR, L3G4200_ID);
	if (ReadID1 == 0XE5 && ReadID2 == 0XD3)  printf("设备ID验证成功\n");
	else while (1) {  printf("设备硬件验证错误---请检查IIC连线\n"); delay_ms(100);	}
	printf("陀螺仪的水平校准--请保持设备水平静止至少3秒\n");
	Calculation_ACC_Offset();
	printf("陀螺仪静态误差校准完成\n");
	Read_HMC5883(&HMC5883_LAST);
	last_yaw = atan2(HMC5883_LAST.X, HMC5883_LAST.Y) + PI;
	TIM_Cmd(TIM5, ENABLE); //定时器中断使能
	while (1)
	{
		if (FLAG_Send == 1)
		{
			FLAG_Send = 0;
			//PTB_OUT(11)=0;
			DMA_UART1_ReportIMU(Q_ANGLE.YAW * 10, -1 * Q_ANGLE.ROLL * 10, -1 * Q_ANGLE.PITCH * 10, 0, 0, 0, 0);
			//DMA_UART1_ReportIMU(Q_ANGLE.PITCH*10,-1*angle_ax*10,0,0,0,0,0);
			//DMA_UART1_ReportMotion(ACC_AVG.X,ACC_AVG.Y,ACC_AVG.Z,L3G4200_GYRO_LAST.X,L3G4200_GYRO_LAST.Y,L3G4200_GYRO_LAST.Z,HMC5883_LAST.X,HMC5883_LAST.Y,HMC5883_LAST.Z);
			//DMA_UART1_ReportMotion(Q_ANGLE.PITCH*20,-1*angle_ax*20,0,0,0,0,0,0,0); //ax   ay
			//PTB_OUT(11)=1;
		}

	}
}


