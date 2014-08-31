#include "SendData.h"


static u8 buf[24];//DMA发送静态缓存数组
void SCI_DIS(u32 ch0,u32 ch1,u32 ch2)//显示数据范围为-4000~+4000
{
    buf[0]=((ch0+4000)/90+' ');
    buf[1]=((ch0+4000)%90+' ');
    buf[2]=((ch1+4000)/90+' ');
    buf[3]=((ch1+4000)%90+' ');
    buf[4]=((ch2+4000)/90+' ');
    buf[5]=((ch2+4000)%90+' ');
		DMA1_USART1_SEND((u32)buf,6);
}


//数据通过串口1上传上位机驱动程序

/**************************实现函数********************************************
*函数原型:		void UART1_ReportIMU(s16 yaw,s16 pitch,s16 roll
				,s16 alt,s16 tempr,s16 press)
*功　　能:		向上位机发送经过解算后的姿态数据
输入参数：
		s16 yaw 经过解算后的航向角度。单位为0.1度 0 -> 3600  对应 0 -> 360.0度
		s16 pitch 解算得到的俯仰角度，单位 0.1度。-900 - 900 对应 -90.0 -> 90.0 度
		s16 roll  解算后得到的横滚角度，单位0.1度。 -1800 -> 1800 对应 -180.0  ->  180.0度
		s16 alt   气压高度。 单位0.1米。  范围一个整型变量
		s16 tempr 温度 。 单位0.1摄氏度   范围：直到你的电路板不能正常工作
		s16 press 气压压力。单位10Pa  一个大气压强在101300pa 这个已经超过一个整型的范围。需要除以10再发给上位机
		s16 IMUpersec  姿态解算速率。运算IMUpersec每秒。
输出参数：没有	
*******************************************************************************/
void DMA_UART1_ReportIMU(s16 yaw,s16 pitch,s16 roll,s16 alt,s16 tempr,s16 press,s16 IMUpersec)
{
 	unsigned int temp=0xaF+2;
	static char ctemp;
	//USART_SendData(USART1,0xa5);
  //USART_SendData(USART1,0x5a);
  //USART_SendData(USART1,14+2);
	//USART_SendData(USART1,0xA1);
	buf[0]=0xA5;
	buf[1]=0x5a;
	buf[2]=16;
	buf[3]=0xa1;


	if(yaw<0)yaw=32768-yaw;
	ctemp=yaw>>8;
  //USART_SendData(USART1,ctemp);
	buf[4]=ctemp;
	temp+=ctemp;
	ctemp=yaw;
  //USART_SendData(USART1,ctemp);
	buf[5]=ctemp;
	temp+=ctemp;

	if(pitch<0)pitch=32768-pitch;
	ctemp=pitch>>8;
	//USART_SendData(USART1,ctemp);
	buf[6]=ctemp;
	temp+=ctemp;
	ctemp=pitch;
	///USART_SendData(USART1,ctemp);
	buf[7]=ctemp;
	temp+=ctemp;

	if(roll<0)roll=32768-roll;
	ctemp=roll>>8;
	//USART_SendData(USART1,ctemp);
	buf[8]=ctemp;
	temp+=ctemp;
	ctemp=roll;
	//USART_SendData(USART1,ctemp);
	buf[9]=ctemp;
	temp+=ctemp;

	if(alt<0)alt=32768-alt;
	ctemp=alt>>8;
	//USART_SendData(USART1,ctemp);
	buf[10]=ctemp;
	temp+=ctemp;
	ctemp=alt;
	//USART_SendData(USART1,ctemp);
	buf[11]=ctemp;
	temp+=ctemp;

	if(tempr<0)tempr=32768-tempr;
	ctemp=tempr>>8;
	//USART_SendData(USART1,ctemp);
	buf[12]=ctemp;
	temp+=ctemp;
	ctemp=tempr;
	//USART_SendData(USART1,ctemp);
	buf[13]=ctemp;
	temp+=ctemp;

	if(press<0)press=32768-press;
	ctemp=press>>8;
	//USART_SendData(USART1,ctemp);
	buf[14]=ctemp;
	temp+=ctemp;
	ctemp=press;
	//USART_SendData(USART1,ctemp);
	buf[15]=ctemp;
	temp+=ctemp;

	//USART_SendData(USART1,temp%256);
	//USART_SendData(USART1,0xaa);
  buf[16]=temp%256;
	buf[17]=0xaa;
	DMA1_USART1_SEND((u32)buf,18);
}


void UART1_ReportIMU(s16 yaw,s16 pitch,s16 roll,s16 alt,s16 tempr,s16 press,s16 IMUpersec)
{
 	unsigned int temp=0xaF+2;
	static char ctemp;
	USART_SendData(USART1,0xa5);
  USART_SendData(USART1,0x5a);
  USART_SendData(USART1,14+2);
	USART_SendData(USART1,0xA1);

	if(yaw<0)yaw=32768-yaw;
	ctemp=yaw>>8;
  USART_SendData(USART1,ctemp);
	temp+=ctemp;
	ctemp=yaw;
  USART_SendData(USART1,ctemp);
	temp+=ctemp;

	if(pitch<0)pitch=32768-pitch;
	ctemp=pitch>>8;
	USART_SendData(USART1,ctemp);
	temp+=ctemp;
	ctemp=pitch;
	USART_SendData(USART1,ctemp);
	temp+=ctemp;

	if(roll<0)roll=32768-roll;
	ctemp=roll>>8;
	USART_SendData(USART1,ctemp);
	temp+=ctemp;
	ctemp=roll;
	USART_SendData(USART1,ctemp);
	temp+=ctemp;

	if(alt<0)alt=32768-alt;
	ctemp=alt>>8;
	USART_SendData(USART1,ctemp);
	temp+=ctemp;
	ctemp=alt;
	USART_SendData(USART1,ctemp);
	temp+=ctemp;

	if(tempr<0)tempr=32768-tempr;
	ctemp=tempr>>8;
	USART_SendData(USART1,ctemp);
	temp+=ctemp;
	ctemp=tempr;
	USART_SendData(USART1,ctemp);
	temp+=ctemp;

	if(press<0)press=32768-press;
	ctemp=press>>8;
	USART_SendData(USART1,ctemp);
	temp+=ctemp;
	ctemp=press;
	USART_SendData(USART1,ctemp);
	temp+=ctemp;

	USART_SendData(USART1,temp%256);
	USART_SendData(USART1,0xaa);
}

/**************************实现函数********************************************
*函数原型:		void UART1_ReportMotion(int16_t ax,int16_t ay,int16_t az,int16_t gx,int16_t gy,int16_t gz,
					int16_t hx,int16_t hy,int16_t hz)
*功　　能:		向上位机发送当前传感器的输出值
输入参数：
	int16_t ax  加速度 X轴ADC输出 范围 ：一个有符号整型
	int16_t ay  加速度 Y轴ADC输出 范围 ：一个有符号整型
	int16_t az  加速度 Z轴ADC输出 范围 ：一个有符号整型
	int16_t gx  陀螺仪 X轴ADC输出 范围 ：一个有符号整型
	int16_t gy  陀螺仪 Y轴ADC输出 范围 ：一个有符号整型
	int16_t gz  陀螺仪 Z轴ADC输出 范围 ：一个有符号整型
	int16_t hx  磁罗盘 X轴ADC输出 范围 ：一个有符号整型
	int16_t hy  磁罗盘 Y轴ADC输出 范围 ：一个有符号整型
	int16_t hz  磁罗盘 Z轴ADC输出 范围 ：一个有符号整型
	
输出参数：没有	
*******************************************************************************/
void UART1_ReportMotion(s16 ax,s16 ay,s16 az,s16 gx,s16 gy,s16 gz,
					s16 hx,s16 hy,s16 hz)
{
 	unsigned int temp=0xaF+9;
	static char ctemp;
	USART_SendData(USART1,0xa5);
	USART_SendData(USART1,0x5a);
	USART_SendData(USART1,14+8);
	USART_SendData(USART1,0xA2);

	if(ax<0)ax=32768-ax;
	ctemp=ax>>8;
	USART_SendData(USART1,ctemp);
	temp+=ctemp;
	ctemp=ax;
	USART_SendData(USART1,ctemp);
	temp+=ctemp;

	if(ay<0)ay=32768-ay;
	ctemp=ay>>8;
	USART_SendData(USART1,ctemp);
	temp+=ctemp;
	ctemp=ay;
	USART_SendData(USART1,ctemp);
	temp+=ctemp;

	if(az<0)az=32768-az;
	ctemp=az>>8;
	USART_SendData(USART1,ctemp);
	temp+=ctemp;
	ctemp=az;
	USART_SendData(USART1,ctemp);
	temp+=ctemp;

	if(gx<0)gx=32768-gx;
	ctemp=gx>>8;
	USART_SendData(USART1,ctemp);
	temp+=ctemp;
	ctemp=gx;
	USART_SendData(USART1,ctemp);
	temp+=ctemp;

	if(gy<0)gy=32768-gy;
	ctemp=gy>>8;
	USART_SendData(USART1,ctemp);
	temp+=ctemp;
	ctemp=gy;
	USART_SendData(USART1,ctemp);
	temp+=ctemp;
//-------------------------
	if(gz<0)gz=32768-gz;
	ctemp=gz>>8;
	USART_SendData(USART1,ctemp);
	temp+=ctemp;
	ctemp=gz;
	USART_SendData(USART1,ctemp);
	temp+=ctemp;

	if(hx<0)hx=32768-hx;
	ctemp=hx>>8;
	USART_SendData(USART1,ctemp);
	temp+=ctemp;
	ctemp=hx;
	USART_SendData(USART1,ctemp);
	temp+=ctemp;

	if(hy<0)hy=32768-hy;
	ctemp=hy>>8;
	USART_SendData(USART1,ctemp);
	temp+=ctemp;
	ctemp=hy;
	USART_SendData(USART1,ctemp);
	temp+=ctemp;

	if(hz<0)hz=32768-hz;
	ctemp=hz>>8;
	USART_SendData(USART1,ctemp);
	temp+=ctemp;
	ctemp=hz;
	USART_SendData(USART1,ctemp);
	temp+=ctemp;

	USART_SendData(USART1,temp%256);
	USART_SendData(USART1,0xaa);
}
void DMA_UART1_ReportMotion(s16 ax,s16 ay,s16 az,s16 gx,s16 gy,s16 gz,
					s16 hx,s16 hy,s16 hz)
{
 	unsigned int temp=0xaF+9;
	static char ctemp;
	//USART_SendData(USART1,0xa5);
	//USART_SendData(USART1,0x5a);
	//USART_SendData(USART1,14+8);
	//USART_SendData(USART1,0xA2);
	buf[0]=0xA5;
	buf[1]=0x5a;
	buf[2]=22;
	buf[3]=0xA2;

	if(ax<0)ax=32768-ax;
	ctemp=ax>>8;
	//USART_SendData(USART1,ctemp);
	buf[4]=ctemp;
	temp+=ctemp;
	ctemp=ax;
	//USART_SendData(USART1,ctemp);
	buf[5]=ctemp;
	temp+=ctemp;

	if(ay<0)ay=32768-ay;
	ctemp=ay>>8;
	//USART_SendData(USART1,ctemp);
	buf[6]=ctemp;
	temp+=ctemp;
	ctemp=ay;
	//USART_SendData(USART1,ctemp);
	buf[7]=ctemp;
	temp+=ctemp;

	if(az<0)az=32768-az;
	ctemp=az>>8;
	//USART_SendData(USART1,ctemp);
	buf[8]=ctemp;
	temp+=ctemp;
	ctemp=az;
	//USART_SendData(USART1,ctemp);
	buf[9]=ctemp;
	temp+=ctemp;

	if(gx<0)gx=32768-gx;
	ctemp=gx>>8;
	//USART_SendData(USART1,ctemp);
	buf[10]=ctemp;
	temp+=ctemp;
	ctemp=gx;
	//USART_SendData(USART1,ctemp);
	buf[11]=ctemp;
	temp+=ctemp;

	if(gy<0)gy=32768-gy;
	ctemp=gy>>8;
	//USART_SendData(USART1,ctemp);
	buf[12]=ctemp;
	temp+=ctemp;
	ctemp=gy;
	//USART_SendData(USART1,ctemp);
	buf[13]=ctemp;
	temp+=ctemp;
//-------------------------
	if(gz<0)gz=32768-gz;
	ctemp=gz>>8;
	//USART_SendData(USART1,ctemp);
	buf[14]=ctemp;
	temp+=ctemp;
	ctemp=gz;
	//USART_SendData(USART1,ctemp);
	buf[15]=ctemp;
	temp+=ctemp;

	if(hx<0)hx=32768-hx;
	ctemp=hx>>8;
	//USART_SendData(USART1,ctemp);
	buf[16]=ctemp;
	temp+=ctemp;
	ctemp=hx;
	//USART_SendData(USART1,ctemp);
	buf[17]=ctemp;
	temp+=ctemp;

	if(hy<0)hy=32768-hy;
	ctemp=hy>>8;
	//USART_SendData(USART1,ctemp);
	buf[18]=ctemp;
	temp+=ctemp;
	ctemp=hy;
	//USART_SendData(USART1,ctemp);
	buf[19]=ctemp;
	temp+=ctemp;

	if(hz<0)hz=32768-hz;
	ctemp=hz>>8;
	//USART_SendData(USART1,ctemp);
	buf[20]=ctemp;
	temp+=ctemp;
	ctemp=hz;
	//USART_SendData(USART1,ctemp);
	buf[21]=ctemp;
	temp+=ctemp;

	//USART_SendData(USART1,temp%256);
	//USART_SendData(USART1,0xaa);
	buf[22]=temp%256;
	buf[23]=0xaa;
	DMA1_USART1_SEND((u32)buf,24);
}




