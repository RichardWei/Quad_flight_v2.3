#include "SendData.h"


static u8 buf[24];//DMA���;�̬��������
void SCI_DIS(u32 ch0,u32 ch1,u32 ch2)//��ʾ���ݷ�ΧΪ-4000~+4000
{
    buf[0]=((ch0+4000)/90+' ');
    buf[1]=((ch0+4000)%90+' ');
    buf[2]=((ch1+4000)/90+' ');
    buf[3]=((ch1+4000)%90+' ');
    buf[4]=((ch2+4000)/90+' ');
    buf[5]=((ch2+4000)%90+' ');
		DMA1_USART1_SEND((u32)buf,6);
}


//����ͨ������1�ϴ���λ����������

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void UART1_ReportIMU(s16 yaw,s16 pitch,s16 roll
				,s16 alt,s16 tempr,s16 press)
*��������:		����λ�����;�����������̬����
���������
		s16 yaw ���������ĺ���Ƕȡ���λΪ0.1�� 0 -> 3600  ��Ӧ 0 -> 360.0��
		s16 pitch ����õ��ĸ����Ƕȣ���λ 0.1�ȡ�-900 - 900 ��Ӧ -90.0 -> 90.0 ��
		s16 roll  �����õ��ĺ���Ƕȣ���λ0.1�ȡ� -1800 -> 1800 ��Ӧ -180.0  ->  180.0��
		s16 alt   ��ѹ�߶ȡ� ��λ0.1�ס�  ��Χһ�����ͱ���
		s16 tempr �¶� �� ��λ0.1���϶�   ��Χ��ֱ����ĵ�·�岻����������
		s16 press ��ѹѹ������λ10Pa  һ������ѹǿ��101300pa ����Ѿ�����һ�����͵ķ�Χ����Ҫ����10�ٷ�����λ��
		s16 IMUpersec  ��̬�������ʡ�����IMUpersecÿ�롣
���������û��	
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

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void UART1_ReportMotion(int16_t ax,int16_t ay,int16_t az,int16_t gx,int16_t gy,int16_t gz,
					int16_t hx,int16_t hy,int16_t hz)
*��������:		����λ�����͵�ǰ�����������ֵ
���������
	int16_t ax  ���ٶ� X��ADC��� ��Χ ��һ���з�������
	int16_t ay  ���ٶ� Y��ADC��� ��Χ ��һ���з�������
	int16_t az  ���ٶ� Z��ADC��� ��Χ ��һ���з�������
	int16_t gx  ������ X��ADC��� ��Χ ��һ���з�������
	int16_t gy  ������ Y��ADC��� ��Χ ��һ���з�������
	int16_t gz  ������ Z��ADC��� ��Χ ��һ���з�������
	int16_t hx  ������ X��ADC��� ��Χ ��һ���з�������
	int16_t hy  ������ Y��ADC��� ��Χ ��һ���з�������
	int16_t hz  ������ Z��ADC��� ��Χ ��һ���з�������
	
���������û��	
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




