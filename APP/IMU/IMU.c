#include "IMU.h"

//*********************************************************
F_INT16_ANGLE Q_ANGLE;

s16	ACC_X_BUF[FILTER_NUM], ACC_Y_BUF[FILTER_NUM], ACC_Z_BUF[FILTER_NUM];	//加速度滑动窗口滤波数组
double last_yaw = 0;

void Calculation_ACC_Offset(void)//计算加速度计，陀螺仪偏移
{
	u8 i = 0;
	s32 count_gx = 0, count_gy = 0, count_gz = 0;
	for (i = 0; i < 40; i++)
	{
		Read_L3G4200D(&L3G4200_GYRO_LAST);
		count_gx += L3G4200_GYRO_LAST.X;
		count_gy += L3G4200_GYRO_LAST.Y;
		count_gz += L3G4200_GYRO_LAST.Z;
	}
	GYRO_OFFSET.X = count_gx / 40;
	GYRO_OFFSET.Y = count_gy / 40;
	GYRO_OFFSET.Z = count_gz / 40;
}




/*******************************************************************************
快速计算 1/Sqrt(x)，源自雷神3的一段代码，神奇的0x5f3759df！比正常的代码快4倍
*******************************************************************************/
float InvSqrt(float x)
{
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i >> 1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}

//把数转化为角速度(角度每秒)
float number_to_dps(s16 number)
{
	float temp;
	temp = (float)number * L3G4200_s500dps;
	return temp;
}

//把数转化为角速度(弧度每秒)
float number_to_dps1(s16 number)
{
	float temp;
	temp = (float)number * Gyro_GainR;
	return temp;
}

//把数转化为加速度
float number_to_g(s16 number)
{
	float temp;
	temp = (float)number * Acc_Gain;
	return temp;
}
//使用加速度计数据计算欧拉角
void Get_Accel_Angle(s16 x, s16 y, s16 z, float* roll, float* pitch)
{
	float temp;

	temp = InvSqrt((float)(y * y + z * z));
	//temp = atan2((float)x,temp)*REDIAN_ANGLE;
	*pitch = temp * x * REDIAN_ANGLE;

	temp = InvSqrt((float)(x * x + z * z));
	//temp = atan2((float)y,temp)*REDIAN_ANGLE;
	*roll =  temp * y * REDIAN_ANGLE;
}



//float gyro_m：陀螺仪测得的量（角速度）
//float incAngle:加计测得的角度值
#define dt 0.0015f//卡尔曼滤波采样频率
#define R_angle 0.69f //测量噪声的协方差（即是测量偏差）
#define Q_angle 0.0001f  //过程噪声的协方差
#define Q_gyro  0.0003f //过程噪声的协方差  过程噪声协方差为一个一行两列矩阵
float kalmanUpdate(const float gyro_m, const float incAngle)
{
	float K_0;//含有卡尔曼增益的另外一个函数，用于计算最优估计值
	float K_1;//含有卡尔曼增益的函数，用于计算最优估计值的偏差
	float Y_0;
	float Y_1;
	float Rate;//去除偏差后的角速度
	float Pdot[4];//过程协方差矩阵的微分矩阵

	float angle_err;//角度偏量
	float E;//计算的过程量
	static float angle = 0; //下时刻最优估计值角度  只需初始化一次，系统会为其分配固定的内存
	static float q_bias = 0;
	//陀螺仪的偏差
	static float P[2][2] = {{ 1, 0 }, { 0, 1 }};//过程协方差矩阵
	Rate = gyro_m - q_bias;
	//计算过程协方差矩阵的微分矩阵
	Pdot[0] = Q_angle - P[0][1] - P[1][0];//？？？？？？
	Pdot[1] = - P[1][1];
	Pdot[2] = - P[1][1];
	Pdot[3] = Q_gyro;//？？？？？？
	angle += Rate * dt; //角速度积分得出角度

	P[0][0] += Pdot[0] * dt; //计算协方差矩阵
	P[0][1] += Pdot[1] * dt;
	P[1][0] += Pdot[2] * dt;
	P[1][1] += Pdot[3] * dt;

	angle_err = incAngle - angle; //计算角度偏差

	E = R_angle + P[0][0];
	K_0 = P[0][0] / E; //计算卡尔曼增益
	K_1 = P[1][0] / E;

	Y_0 = P[0][0];
	Y_1 = P[0][1];

	P[0][0] -= K_0 * Y_0;//更新协方差矩阵
	P[0][1] -= K_0 * Y_1;
	P[1][0] -= K_1 * Y_0;
	P[1][1] -= K_1 * Y_1;
	angle += K_0 * angle_err; //给出最优估计值
	q_bias += K_1 * angle_err;//跟新最优估计值偏差
	return angle;
}





//数据融合计算欧拉角
//输入：角速度(度/秒)，加速度原始数据
//输出：Q_ANGLE
void IMUupdate1(s16 gx, s16 gy, s16 gz, s16 ax, s16 ay, s16 az, s16 yaw_gyro_tar)
{
	static float x1 = 0, y1 = 0, z1 = 0, x2 = 0, y2 = 0, z2 = 0;
	static float roll_a = 0, pitch_a = 0;
	/*欧拉角在无磁力计，飞行器不空翻的情况下勉强适用，后期改为卡尔曼*/
	Get_Accel_Angle(ax, ay, az, &roll_a, &pitch_a); //得到加速度计的欧拉角度  即弧度


// 	//陀螺仪计算角度
// 	Q_ANGLE.PITCH = Q_ANGLE.PITCH + number_to_dps(gy) * DT;
// 	Q_ANGLE.ROLL = Q_ANGLE.ROLL + number_to_dps(gx) * DT;
// 	Q_ANGLE.YAW =  Q_ANGLE.YAW + number_to_dps(gz) * DT;

	/*
	//一阶互补滤波
	Q_ANGLE.ROLL = (Q_ANGLE.ROLL+number_to_dps(gx)*DT)*FILTER_A +roll_a*(1-FILTER_A);

	Q_ANGLE.PITCH = (Q_ANGLE.PITCH+number_to_dps(gy)*DT)*FILTER_A + pitch_a*(1-FILTER_A);

	Q_ANGLE.YAW =  Q_ANGLE.YAW + number_to_dps(gz)*DT;

	*/
	///*
	//二阶互补滤波    很简单的算法
	//L3G4200_s2000dps
	x1 = (pitch_a - L3G4200_GYRO_LAST.X) * FILTER_K * FILTER_K; //角度差乘以滤波系数
	y1 = y1 + x1 * DT; //y1静态数据初始化一次 在本函数内部调用
	z1 = y1 + 2 * FILTER_K * (pitch_a - L3G4200_GYRO_LAST.X) + number_to_dps(gy);
	Q_ANGLE.PITCH = Q_ANGLE.PITCH + z1 * DT;

	x2 = (roll_a - L3G4200_GYRO_LAST.Y) * FILTER_K * FILTER_K;
	y2 = y2 + x2 * DT;
	z2 = y2 + 2 * FILTER_K * (roll_a - L3G4200_GYRO_LAST.Y) + number_to_dps(gx);
	Q_ANGLE.ROLL = Q_ANGLE.ROLL + z2 * DT;

	if (yaw_gyro_tar >= -5 && yaw_gyro_tar <= 5) //YAW摇杆死区控制
	{
		Q_ANGLE.YAW =  Q_ANGLE.YAW + number_to_dps(gz) * DT; //航线角更具z的漂移速度
	}
}

float angle_ax = 0, angle_ay = 0, angle_gx = 0, angle_gy = 0, angle_gz = 0;

void IMU_AHRS(void)//本函数只是为了对比角度跟踪曲线
{
	//static float angel_ax=0,angle_ay=0,angle_gx=0,angle_gy=0,angle_gz=0;
	Get_Accel_Angle(ADXL345_ACC_LAST.X, ADXL345_ACC_LAST.Y, ADXL345_ACC_LAST.Z, &angle_ay, &angle_ax); //得到加速度计的欧拉角度  即弧度
	angle_gx += L3G4200_GYRO_LAST.X * 0.0000525; //0.07*0.003=0.00021
	angle_gy += L3G4200_GYRO_LAST.Y * 0.0000525;
	angle_gz += L3G4200_GYRO_LAST.Z * 0.0000525;
	//DMA_UART1_ReportIMU(s16 yaw,s16 pitch,s16 roll,s16 alt,s16 tempr,s16 press,s16 IMUpersec);
}




void Prepare_Data(void)
{
	static uint8_t filter_cnt = 0;
	int32_t temp1 = 0, temp2 = 0, temp3 = 0;
	uint8_t i;
	ACC_X_BUF[filter_cnt] = ADXL345_ACC_LAST.X;//更新滑动窗口数组
	ACC_Y_BUF[filter_cnt] = ADXL345_ACC_LAST.Y;
	ACC_Z_BUF[filter_cnt] = ADXL345_ACC_LAST.Z;
	for (i = 0; i < FILTER_NUM; i++)
	{
		temp1 += ACC_X_BUF[i];
		temp2 += ACC_Y_BUF[i];
		temp3 += ACC_Z_BUF[i];
	}
	ACC_AVG.X = temp1 / FILTER_NUM + 15; //经验值
	ACC_AVG.Y = temp2 / FILTER_NUM + 5;
	ACC_AVG.Z = temp3 / FILTER_NUM;
	filter_cnt++;
	if (filter_cnt == FILTER_NUM)	filter_cnt = 0;
	GYRO_I.X += L3G4200_GYRO_LAST.X * L3G4200_s500dps * 0.003; //0.0001是时间间隔,两次prepare的执行周期
	GYRO_I.Y += L3G4200_GYRO_LAST.Y * L3G4200_s500dps * 0.003;
	GYRO_I.Z += L3G4200_GYRO_LAST.Z * L3G4200_s500dps * 0.003;
	if (GYRO_I.X > PPI) GYRO_I.X = 0;
	if (GYRO_I.Y > PPI) GYRO_I.Y = 0;
	if (GYRO_I.Z > PPI) GYRO_I.Z = 0;
}




//#define Kp 10.0f                        // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Kp 10.0f
#define Ki 0.015f  //0.008f                        // integral gain governs rate of convergence of gyroscope biases
#define halfT 0.0015f                   // half the sample period采样周期的一半

float q0 = 1, q1 = 0, q2 = 0, q3 = 0;    // quaternion elements representing the estimated orientation
float exInt = 0, eyInt = 0, ezInt = 0;    // scaled integral error
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz)
{
	float norm;
//  float hx, hy, hz, bx, bz;
	float vx, vy, vz;// wx, wy, wz;
	float ex, ey, ez;
	//float  yaw_mag;
	double  yaw_mag;

	// 先把这些用得到的值算好
	float q0q0 = q0 * q0;
	float q0q1 = q0 * q1;
	float q0q2 = q0 * q2;
	//float q0q3 = q0*q3;//
	float q1q1 = q1 * q1;
	//float q1q2 = q1*q2;//
	float q1q3 = q1 * q3;
	float q2q2 = q2 * q2;
	float q2q3 = q2 * q3;
	float q3q3 = q3 * q3;

	if (ax * ay * az == 0)
		return;

	norm = sqrt(ax * ax + ay * ay + az * az); //acc数据归一化
	ax = ax / norm;
	ay = ay / norm;
	az = az / norm;

	// estimated direction of gravity and flux (v and w)              估计重力方向和流量/变迁
	vx = 2 * (q1q3 - q0q2);												//四元素中xyz的表示
	vy = 2 * (q0q1 + q2q3);
	vz = q0q0 - q1q1 - q2q2 + q3q3 ;

	// error is sum of cross product between reference direction of fields and direction measured by sensors
	ex = (ay * vz - az * vy) ;                           					 //向量外积在相减得到差分就是误差
	ey = (az * vx - ax * vz) ;
	ez = (ax * vy - ay * vx) ;

	exInt = exInt + ex * Ki;								  //对误差进行积分
	eyInt = eyInt + ey * Ki;
	ezInt = ezInt + ez * Ki;

	// adjusted gyroscope measurements
	gx = gx + Kp * ex + exInt;					   							//将误差PI后补偿到陀螺仪，即补偿零点漂移
	gy = gy + Kp * ey + eyInt;
	gz = gz + Kp * ez + ezInt;				   							//这里的gz由于没有观测者进行矫正会产生漂移，表现出来的就是积分自增或自减

	// integrate quaternion rate and normalise						   //四元素的微分方程
	q0 = q0 + (-q1 * gx - q2 * gy - q3 * gz) * halfT;
	q1 = q1 + (q0 * gx + q2 * gz - q3 * gy) * halfT;
	q2 = q2 + (q0 * gy - q1 * gz + q3 * gx) * halfT;
	q3 = q3 + (q0 * gz + q1 * gy - q2 * gx) * halfT;

	// normalise quaternion
	norm = sqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 = q0 / norm;
	q1 = q1 / norm;
	q2 = q2 / norm;
	q3 = q3 / norm;



	norm = sqrt(mx * mx + my * my + mz * mz); //acc数据归一化
	mx = mx / norm;
	my = my / norm;
	mz = mz / norm;

	Q_ANGLE.PITCH  = asin(-2 * q1 * q3 + 2 * q0 * q2) * 57.3; // pitch
	Q_ANGLE.ROLL = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 57.3; // roll
	//Q_ANGLE.YAW   = -(0.9 * (-Q_ANGLE.YAW +gz*2*halfT) + 5.73 * atan2(mx*cos(Q_ANGLE.ROLL) + my*sin(Q_ANGLE.ROLL)*sin(Q_ANGLE.PITCH) + mz*sin(Q_ANGLE.ROLL)*cos(Q_ANGLE.PITCH), my*cos(Q_ANGLE.PITCH) - mz*sin(Q_ANGLE.PITCH)));

	if (HMC_TIME == 0)
	{
		yaw_mag = atan2(2.0 * (q1q3 - q0q2) * mz - (q0q0 - q1q1 - q2q2 + q3q3) * mx, (q0q0 - q1q1 - q2q2 + q3q3) * my - 2.0 * (q2q3 + q0q1) * mz) + PI / 2 + 0.05061; //单位：弧度 (0~2*PI),后面是加上磁偏角变为真北，磁偏角以天津5.5°计算
		if (yaw_mag < 0)
			yaw_mag += (2 * PI);
		if (yaw_mag > 2 * PI)
			yaw_mag -= (2 * PI); //把磁力计解算角度限定在 0~2PI

		if (fabs(yaw_mag - last_yaw) < PI) //陀螺仪顺时针转是负数
		{

			yaw_mag += -1 * gz + (yaw_mag - last_yaw) * 0.015;

		}

		if (fabs(yaw_mag - last_yaw) >= PI) // 由于偏航角是以2PI为周期的，所以如果差值大于PI就说明应该反方向转动消除误差
		{
			yaw_mag += -1 * gz -	(yaw_mag - last_yaw) * 0.015;

		}
		//把角速度计和磁力计数据融合，gz为角速度数据，由于每2ms调用一次IMUupdata()，所以gz*0.002就是2ms转过的角度，对此积分就是实时的角度
		//把磁力计解算角度和上次解算的偏航角做差得到偏航角的误差，乘以一个系数0.002（经验值）再积分用来修正角速度计的累积误差
		//实验表明，角速度计再快速转动时误差较小，但是转动速度较慢时通过积分几乎得不到偏航角的变化，此时就需要用磁力计校正
		//通过数据融合，即避免了角速度计的累积误差又可以用角速度计解算的角度校正滚转俯仰时磁力计产生的误差（从±15°减小到±3°）
		//
		if (yaw_mag < 0)
			yaw_mag += (PPI);
		if (yaw_mag > PPI)
			yaw_mag -= (PPI);	//把偏航角解算角度限定在 0~2PI
		last_yaw = yaw_mag;
		Q_ANGLE.YAW = yaw_mag * 57.3;
	}
}









