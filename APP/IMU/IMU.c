#include "IMU.h"

//*********************************************************
F_INT16_ANGLE Q_ANGLE;

s16	ACC_X_BUF[FILTER_NUM], ACC_Y_BUF[FILTER_NUM], ACC_Z_BUF[FILTER_NUM];	//���ٶȻ��������˲�����
double last_yaw = 0;

void Calculation_ACC_Offset(void)//������ٶȼƣ�������ƫ��
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
���ټ��� 1/Sqrt(x)��Դ������3��һ�δ��룬�����0x5f3759df���������Ĵ����4��
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

//����ת��Ϊ���ٶ�(�Ƕ�ÿ��)
float number_to_dps(s16 number)
{
	float temp;
	temp = (float)number * L3G4200_s500dps;
	return temp;
}

//����ת��Ϊ���ٶ�(����ÿ��)
float number_to_dps1(s16 number)
{
	float temp;
	temp = (float)number * Gyro_GainR;
	return temp;
}

//����ת��Ϊ���ٶ�
float number_to_g(s16 number)
{
	float temp;
	temp = (float)number * Acc_Gain;
	return temp;
}
//ʹ�ü��ٶȼ����ݼ���ŷ����
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



//float gyro_m�������ǲ�õ��������ٶȣ�
//float incAngle:�ӼƲ�õĽǶ�ֵ
#define dt 0.0015f//�������˲�����Ƶ��
#define R_angle 0.69f //����������Э������ǲ���ƫ�
#define Q_angle 0.0001f  //����������Э����
#define Q_gyro  0.0003f //����������Э����  ��������Э����Ϊһ��һ�����о���
float kalmanUpdate(const float gyro_m, const float incAngle)
{
	float K_0;//���п��������������һ�����������ڼ������Ź���ֵ
	float K_1;//���п���������ĺ��������ڼ������Ź���ֵ��ƫ��
	float Y_0;
	float Y_1;
	float Rate;//ȥ��ƫ���Ľ��ٶ�
	float Pdot[4];//����Э��������΢�־���

	float angle_err;//�Ƕ�ƫ��
	float E;//����Ĺ�����
	static float angle = 0; //��ʱ�����Ź���ֵ�Ƕ�  ֻ���ʼ��һ�Σ�ϵͳ��Ϊ�����̶����ڴ�
	static float q_bias = 0;
	//�����ǵ�ƫ��
	static float P[2][2] = {{ 1, 0 }, { 0, 1 }};//����Э�������
	Rate = gyro_m - q_bias;
	//�������Э��������΢�־���
	Pdot[0] = Q_angle - P[0][1] - P[1][0];//������������
	Pdot[1] = - P[1][1];
	Pdot[2] = - P[1][1];
	Pdot[3] = Q_gyro;//������������
	angle += Rate * dt; //���ٶȻ��ֵó��Ƕ�

	P[0][0] += Pdot[0] * dt; //����Э�������
	P[0][1] += Pdot[1] * dt;
	P[1][0] += Pdot[2] * dt;
	P[1][1] += Pdot[3] * dt;

	angle_err = incAngle - angle; //����Ƕ�ƫ��

	E = R_angle + P[0][0];
	K_0 = P[0][0] / E; //���㿨��������
	K_1 = P[1][0] / E;

	Y_0 = P[0][0];
	Y_1 = P[0][1];

	P[0][0] -= K_0 * Y_0;//����Э�������
	P[0][1] -= K_0 * Y_1;
	P[1][0] -= K_1 * Y_0;
	P[1][1] -= K_1 * Y_1;
	angle += K_0 * angle_err; //�������Ź���ֵ
	q_bias += K_1 * angle_err;//�������Ź���ֵƫ��
	return angle;
}





//�����ںϼ���ŷ����
//���룺���ٶ�(��/��)�����ٶ�ԭʼ����
//�����Q_ANGLE
void IMUupdate1(s16 gx, s16 gy, s16 gz, s16 ax, s16 ay, s16 az, s16 yaw_gyro_tar)
{
	static float x1 = 0, y1 = 0, z1 = 0, x2 = 0, y2 = 0, z2 = 0;
	static float roll_a = 0, pitch_a = 0;
	/*ŷ�������޴����ƣ����������շ����������ǿ���ã����ڸ�Ϊ������*/
	Get_Accel_Angle(ax, ay, az, &roll_a, &pitch_a); //�õ����ٶȼƵ�ŷ���Ƕ�  ������


// 	//�����Ǽ���Ƕ�
// 	Q_ANGLE.PITCH = Q_ANGLE.PITCH + number_to_dps(gy) * DT;
// 	Q_ANGLE.ROLL = Q_ANGLE.ROLL + number_to_dps(gx) * DT;
// 	Q_ANGLE.YAW =  Q_ANGLE.YAW + number_to_dps(gz) * DT;

	/*
	//һ�׻����˲�
	Q_ANGLE.ROLL = (Q_ANGLE.ROLL+number_to_dps(gx)*DT)*FILTER_A +roll_a*(1-FILTER_A);

	Q_ANGLE.PITCH = (Q_ANGLE.PITCH+number_to_dps(gy)*DT)*FILTER_A + pitch_a*(1-FILTER_A);

	Q_ANGLE.YAW =  Q_ANGLE.YAW + number_to_dps(gz)*DT;

	*/
	///*
	//���׻����˲�    �ܼ򵥵��㷨
	//L3G4200_s2000dps
	x1 = (pitch_a - L3G4200_GYRO_LAST.X) * FILTER_K * FILTER_K; //�ǶȲ�����˲�ϵ��
	y1 = y1 + x1 * DT; //y1��̬���ݳ�ʼ��һ�� �ڱ������ڲ�����
	z1 = y1 + 2 * FILTER_K * (pitch_a - L3G4200_GYRO_LAST.X) + number_to_dps(gy);
	Q_ANGLE.PITCH = Q_ANGLE.PITCH + z1 * DT;

	x2 = (roll_a - L3G4200_GYRO_LAST.Y) * FILTER_K * FILTER_K;
	y2 = y2 + x2 * DT;
	z2 = y2 + 2 * FILTER_K * (roll_a - L3G4200_GYRO_LAST.Y) + number_to_dps(gx);
	Q_ANGLE.ROLL = Q_ANGLE.ROLL + z2 * DT;

	if (yaw_gyro_tar >= -5 && yaw_gyro_tar <= 5) //YAWҡ����������
	{
		Q_ANGLE.YAW =  Q_ANGLE.YAW + number_to_dps(gz) * DT; //���߽Ǹ���z��Ư���ٶ�
	}
}

float angle_ax = 0, angle_ay = 0, angle_gx = 0, angle_gy = 0, angle_gz = 0;

void IMU_AHRS(void)//������ֻ��Ϊ�˶ԱȽǶȸ�������
{
	//static float angel_ax=0,angle_ay=0,angle_gx=0,angle_gy=0,angle_gz=0;
	Get_Accel_Angle(ADXL345_ACC_LAST.X, ADXL345_ACC_LAST.Y, ADXL345_ACC_LAST.Z, &angle_ay, &angle_ax); //�õ����ٶȼƵ�ŷ���Ƕ�  ������
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
	ACC_X_BUF[filter_cnt] = ADXL345_ACC_LAST.X;//���»�����������
	ACC_Y_BUF[filter_cnt] = ADXL345_ACC_LAST.Y;
	ACC_Z_BUF[filter_cnt] = ADXL345_ACC_LAST.Z;
	for (i = 0; i < FILTER_NUM; i++)
	{
		temp1 += ACC_X_BUF[i];
		temp2 += ACC_Y_BUF[i];
		temp3 += ACC_Z_BUF[i];
	}
	ACC_AVG.X = temp1 / FILTER_NUM + 15; //����ֵ
	ACC_AVG.Y = temp2 / FILTER_NUM + 5;
	ACC_AVG.Z = temp3 / FILTER_NUM;
	filter_cnt++;
	if (filter_cnt == FILTER_NUM)	filter_cnt = 0;
	GYRO_I.X += L3G4200_GYRO_LAST.X * L3G4200_s500dps * 0.003; //0.0001��ʱ����,����prepare��ִ������
	GYRO_I.Y += L3G4200_GYRO_LAST.Y * L3G4200_s500dps * 0.003;
	GYRO_I.Z += L3G4200_GYRO_LAST.Z * L3G4200_s500dps * 0.003;
	if (GYRO_I.X > PPI) GYRO_I.X = 0;
	if (GYRO_I.Y > PPI) GYRO_I.Y = 0;
	if (GYRO_I.Z > PPI) GYRO_I.Z = 0;
}




//#define Kp 10.0f                        // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Kp 10.0f
#define Ki 0.015f  //0.008f                        // integral gain governs rate of convergence of gyroscope biases
#define halfT 0.0015f                   // half the sample period�������ڵ�һ��

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

	// �Ȱ���Щ�õõ���ֵ���
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

	norm = sqrt(ax * ax + ay * ay + az * az); //acc���ݹ�һ��
	ax = ax / norm;
	ay = ay / norm;
	az = az / norm;

	// estimated direction of gravity and flux (v and w)              �����������������/��Ǩ
	vx = 2 * (q1q3 - q0q2);												//��Ԫ����xyz�ı�ʾ
	vy = 2 * (q0q1 + q2q3);
	vz = q0q0 - q1q1 - q2q2 + q3q3 ;

	// error is sum of cross product between reference direction of fields and direction measured by sensors
	ex = (ay * vz - az * vy) ;                           					 //�������������õ���־������
	ey = (az * vx - ax * vz) ;
	ez = (ax * vy - ay * vx) ;

	exInt = exInt + ex * Ki;								  //�������л���
	eyInt = eyInt + ey * Ki;
	ezInt = ezInt + ez * Ki;

	// adjusted gyroscope measurements
	gx = gx + Kp * ex + exInt;					   							//�����PI�󲹳��������ǣ����������Ư��
	gy = gy + Kp * ey + eyInt;
	gz = gz + Kp * ez + ezInt;				   							//�����gz����û�й۲��߽��н��������Ư�ƣ����ֳ����ľ��ǻ����������Լ�

	// integrate quaternion rate and normalise						   //��Ԫ�ص�΢�ַ���
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



	norm = sqrt(mx * mx + my * my + mz * mz); //acc���ݹ�һ��
	mx = mx / norm;
	my = my / norm;
	mz = mz / norm;

	Q_ANGLE.PITCH  = asin(-2 * q1 * q3 + 2 * q0 * q2) * 57.3; // pitch
	Q_ANGLE.ROLL = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 57.3; // roll
	//Q_ANGLE.YAW   = -(0.9 * (-Q_ANGLE.YAW +gz*2*halfT) + 5.73 * atan2(mx*cos(Q_ANGLE.ROLL) + my*sin(Q_ANGLE.ROLL)*sin(Q_ANGLE.PITCH) + mz*sin(Q_ANGLE.ROLL)*cos(Q_ANGLE.PITCH), my*cos(Q_ANGLE.PITCH) - mz*sin(Q_ANGLE.PITCH)));

	if (HMC_TIME == 0)
	{
		yaw_mag = atan2(2.0 * (q1q3 - q0q2) * mz - (q0q0 - q1q1 - q2q2 + q3q3) * mx, (q0q0 - q1q1 - q2q2 + q3q3) * my - 2.0 * (q2q3 + q0q1) * mz) + PI / 2 + 0.05061; //��λ������ (0~2*PI),�����Ǽ��ϴ�ƫ�Ǳ�Ϊ�汱����ƫ�������5.5�����
		if (yaw_mag < 0)
			yaw_mag += (2 * PI);
		if (yaw_mag > 2 * PI)
			yaw_mag -= (2 * PI); //�Ѵ����ƽ���Ƕ��޶��� 0~2PI

		if (fabs(yaw_mag - last_yaw) < PI) //������˳ʱ��ת�Ǹ���
		{

			yaw_mag += -1 * gz + (yaw_mag - last_yaw) * 0.015;

		}

		if (fabs(yaw_mag - last_yaw) >= PI) // ����ƫ��������2PIΪ���ڵģ����������ֵ����PI��˵��Ӧ�÷�����ת���������
		{
			yaw_mag += -1 * gz -	(yaw_mag - last_yaw) * 0.015;

		}
		//�ѽ��ٶȼƺʹ����������ںϣ�gzΪ���ٶ����ݣ�����ÿ2ms����һ��IMUupdata()������gz*0.002����2msת���ĽǶȣ��Դ˻��־���ʵʱ�ĽǶ�
		//�Ѵ����ƽ���ǶȺ��ϴν����ƫ��������õ�ƫ���ǵ�������һ��ϵ��0.002������ֵ���ٻ��������������ٶȼƵ��ۻ����
		//ʵ����������ٶȼ��ٿ���ת��ʱ����С������ת���ٶȽ���ʱͨ�����ּ����ò���ƫ���ǵı仯����ʱ����Ҫ�ô�����У��
		//ͨ�������ںϣ��������˽��ٶȼƵ��ۻ�����ֿ����ý��ٶȼƽ���ĽǶ�У����ת����ʱ�����Ʋ��������ӡ�15���С����3�㣩
		//
		if (yaw_mag < 0)
			yaw_mag += (PPI);
		if (yaw_mag > PPI)
			yaw_mag -= (PPI);	//��ƫ���ǽ���Ƕ��޶��� 0~2PI
		last_yaw = yaw_mag;
		Q_ANGLE.YAW = yaw_mag * 57.3;
	}
}









