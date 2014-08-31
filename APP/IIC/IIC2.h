/* #include "STM32_I2C.h" */

#ifndef _I2C_H
#define _I2C_H


#include "sys.h"
#include "delay.h"
//IO����
#define SCL_H         GPIOB->BSRR = GPIO_Pin_6
#define SCL_L         GPIOB->BRR  = GPIO_Pin_6 
    
#define SDA_H         GPIOB->BSRR = GPIO_Pin_7
#define SDA_L         GPIOB->BRR  = GPIO_Pin_7

#define SCL_read      GPIOB->IDR  & GPIO_Pin_6 
#define SDA_read      GPIOB->IDR  & GPIO_Pin_7

#define I2C_DELAY_VAL 2
typedef enum {FALSE = 0, TRUE = !FALSE} bool;

//IIC���в�������
void I2C_Init_IO(void);      //��ʼ��IIC��IO��				 
void I2C_Start(void);				//����IIC��ʼ�ź�
void I2C_Stop(void);	  			//����IICֹͣ�ź�

bool I2C_WaitAck(void); 				//IIC�ȴ�ACK�ź�
void I2C_Ack(void);					//IIC����ACK�ź�
void I2C_NoAck(void);				//IIC������ACK�ź�

void I2C_SendByte(u8 SendByte);			//IIC����һ���ֽ�
u8 I2C_ReadByte(void);            //IIC��ȡһ���ֽ�

//******���ֽ�д��**********//
bool Single_Write(u8 REG_Address,u8 REG_data,u8 SlaveAddress);
//********���ֽڶ�ȡ*********//
u8 Single_Read(u8 REG_Address,u8 SlaveAddress);
//���ֽ�д��//
void Multiple_write(u8 star_addr,u8 num,u8 SlaveAddress,u8* send_buf);
//��ȡ����ֽڵ�����//
void Multiple_read(u8 star_addr,u8 num,u8 SlaveAddress,u8* recv_buf);

/////////////////////////////mpu6050�ӿں���///////////////////////////////////////////////////////////
void i2cRead(u8 SlaveAddress,u8 star_addr,u8 num,u8* recv_buf);
void i2cWrite(u8 SlaveAddress,u8 REG_Address,u8 REG_data);

#endif


