/***********************************************************************
File Name : 	hmc.c
Author :  		Name_006
Version :  		V1.0
Date :  		2011.12.10
Description :  	�������������� ICѡ�õ��� HMC5883L
Others��		�ο�����Colin���� �ش˸�л~~
				�����Ա� http://shop65888055.taobao.com/
				רӪ ������ ���ٶȼ� ���贫���� �ȴ�����ģ��  ��ӭѡ��
				QQ ��353973314  ��̬�㷨Ⱥ �� 199376386
**********************************************************************/
#include "HMC5883.h"//xc=68.6236 yc=-43.3018
S_INT16_XYZ HMC5883_LAST;
//static u8 MSD[9];
// ��ʼ��HMC5883L�������ο���Ҫ�����Լ�������е���
void HMC5883_Init(void)
{
   /* unsigned char byData[4];
    //CFG_A=0xC1, CFG_B=0xA0, MODE=0x00�ɲ��Գ�ģ�麸�Ӻ�оƬ�Ƿ��ջ�����ʱ������X,Y,Z����Ϊ400-1000��������оƬ��������
    byData[0] = 0x78; //50HZ//byData[0] = 0x70; //0xC0; 	// SELF-TESTʱΪ0xC1	   		 //ƽ��������8  15hz  
    byData[1] = 0x00; //byData[1] = 0x20; // 0x40; 	//0x20; // SELF-TESTʱΪ0xA0 //��Χ+_1.9Ga   ����820��˹
    byData[2] = 0x00; // MODE // SELF-TESTʱΪ0X00    ����Ϊ��������ģʽ  			 //
    //Multiple_write(CFG_A,3,DEVICE_WR_ADDR,byData);   //����д�����Ĵ���*/
  //Single_Write(0x14,0x00,0x3c);
	//Single_Write(0x40,0x01,0x3c);
	//Single_Write(0x00,0x02,0x3c);
	Single_Write(0x3C,0x00,0x18);//0x14
	Single_Write(0x3C,0x02,0x00);
}


void Read_HMC5883(S_INT16_XYZ*ptResult)
{

    uint8_t tmp[6];
    int32_t s32Val;
    if(true == Mult_Read(DEVICE_WR_ADDR,X_M,tmp,6))   //�����������������
    {

		  	ptResult->X  = (int16_t)((tmp[0] << 8) | tmp[1])-87;//matlabУ׼ ���ֵ
        s32Val = (int16_t)((tmp[4] << 8) | tmp[5])+26;  
			  s32Val = (s32Val*10403)/10000;
        ptResult->Y    = (int16_t)s32Val;
        ptResult->Z    = (int16_t)((tmp[2] << 8) | tmp[3]);
    }  
}

void HMC5883L_Printf(S_INT16_XYZ*ptResult)
{
    int16_t x,y;
    float angle;
    u16 hu;
    x = ptResult->X;
    y = ptResult->Y;
    //�������
    if(x>0x7fff)x-=0xffff;
    if(y>0x7fff)y-=0xffff;
    //LED1_ON();
    angle= atan2(y,x) * (180 / 3.14159265) + 180;   //160us����ʱ�� 
	  hu=(u16)angle;
	  //LED1_OFF();
    //ptResult->ha = (int16_t)(angle*10);   // �õ�����ȷ��0.1��
    printf("HMC5883L:\thx: %4d,\thy: %4d,\thz: %4d,\t%4d\n\r",
            ptResult->X, ptResult->Y, ptResult->Z,hu);
}
