#include "INIT.h"
u8 data[6];
u8 Send_cnt=0,FLAG_Send=0;
u8 HMC_TIME=0;
void ReadData(void)
{ 
	//float angle;
	Read_L3G4200D(&L3G4200_GYRO_LAST);
	Read_ADXL345(&ADXL345_ACC_LAST);
	HMC_TIME++;
	if(HMC_TIME==5)//60ms��ȡһ��
  {
	 HMC_TIME=0;
	 PTB_OUT(10)=1;
   Read_HMC5883(&HMC5883_LAST);
	 PTB_OUT(10)=0;
  }	
}




//ͨ�ö�ʱ��5�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM5_Int_Init(u16 arr,u16 psc)  //49   7199
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	
	
	//TIM_Cmd(TIM5, ENABLE);//ʹ��TIMx,�ĵ���������ʹ����			 
}
//��ʱ��5�жϷ������
void TIM5_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
		{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);  //���TIMx�����жϱ�־ 
	  PTB_OUT(11)=1;
		Send_cnt++;
		ReadData();
	  Prepare_Data();
		IMUupdate(GYRO_I.X,GYRO_I.Y,GYRO_I.Z,ACC_AVG.X,ACC_AVG.Y,ACC_AVG.Z,HMC5883_LAST.X,HMC5883_LAST.Y,HMC5883_LAST.Z);
		//IMUupdate1(GYRO_I.X,GYRO_I.Y,GYRO_I.Z,ACC_AVG.X,ACC_AVG.Y,ACC_AVG.Z,0);	
		IMU_AHRS();
		if(Send_cnt==40)
			{
				FLAG_Send=1;
        Send_cnt=0;
			}
		  PTB_OUT(11)=0;
	}
}

void NVIV_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�	
}


void ALL_INIT(void) 
{ 
  NVIV_Init();//�жϷ���
	uart_init(38400);
	Delay_Init();//�δ�ʱ����ʱ��ʼ��
	LED_Init();//PTC13
	I2C_IO_Init();
  ADXL345_Init();//���Զ�ȡIIC������ ���ٶ�4g
  L3G4200_Init();//������500��/s
	delay_ms(10);
	HMC5883_Init();
	delay_ms(10);
  TIM5_Int_Init(29,7199);//3ms ��ʱ�ж� ��ȷ��С�����4λ
}












