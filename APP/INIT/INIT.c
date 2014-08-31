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
	if(HMC_TIME==5)//60ms读取一次
  {
	 HMC_TIME=0;
	 PTB_OUT(10)=1;
   Read_HMC5883(&HMC5883_LAST);
	 PTB_OUT(10)=0;
  }	
}




//通用定时器5中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM5_Int_Init(u16 arr,u16 psc)  //49   7199
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
	
	
	//TIM_Cmd(TIM5, ENABLE);//使能TIMx,改到主函数中使能了			 
}
//定时器5中断服务程序
void TIM5_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
		{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);  //清除TIMx更新中断标志 
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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级	
}


void ALL_INIT(void) 
{ 
  NVIV_Init();//中断分组
	uart_init(38400);
	Delay_Init();//滴答时钟延时初始化
	LED_Init();//PTC13
	I2C_IO_Init();
  ADXL345_Init();//测试读取IIC的数据 加速度4g
  L3G4200_Init();//陀螺仪500度/s
	delay_ms(10);
	HMC5883_Init();
	delay_ms(10);
  TIM5_Int_Init(29,7199);//3ms 定时中断 精确到小数点后4位
}












