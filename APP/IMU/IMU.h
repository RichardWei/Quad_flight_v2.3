#ifndef __IMU_H
#define __IMU_H	 
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "math.h"
#include "INIT.h"


#include "L3G4200.h"
 
#include "ADXL345.h"
#define FILTER_NUM   40//��ֵ�˲�����

#define REDIAN_ANGLE  57.29578;     //һ���ȶ�Ӧ�ĽǶ�
#define Gyro_GainR 		0.0012217f;		//���������̣�+-2000����ÿ��  L3G4200_s2000dps*3.14/180
#define Acc_Gain 			3.65;			//ȫ�ֱ���ģʽ

#define DT 	0.003f					//ÿ���˲���ʱ��Ƭ  3ms
#define FILTER_A  0.9983f //һ�׻����˲����˲�ϵ��
#define FILTER_K  0.45f//���׻����˲����˲�ϵ��
#define PI  3.141592654f
#define PPI 6.283185307f

//����������ʽ����ɾ��
extern float angle_ax,angle_ay,angle_gx,angle_gy,angle_gz;
extern double last_yaw;
//extern volatile u8 f; 
typedef struct
{
	float PITCH;
	float ROLL;
	float YAW;
}F_INT16_ANGLE;//����һ���ṹ��

extern F_INT16_ANGLE Q_ANGLE;

void Get_Accel_Angle(s16 x,s16 y,s16 z,float* roll,float* pitch);	
void IMUupdate1(s16 gx, s16 gy, s16 gz, s16 ax, s16 ay, s16 az,s16 yaw_gyro_tar);
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az,float mx,float my,float mz);
void IMU_AHRS(void);
void Prepare_Data(void);
//void Init_Quaternion(void);


void Calculation_ACC_Offset(void);

#endif
