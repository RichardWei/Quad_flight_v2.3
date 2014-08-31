/*=====================================================================================================*/
/*=====================================================================================================*/
#include "IIC.h"//�м���stm32f10x_conf.h��ȥ�� IIC��DMA��ע��



__inline void I2C_delay(void)
{
   u8 i=I2C_DELAY_VAL; //��������Ż��ٶ�   ����������͵�5����д��
   while(i) 
   { 
     i--; 
   } 
}

//��ʼ��IIC
void I2C_Init_IO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//����GPIOCʱ��	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_6; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;//GPIO_Mode_Out_OD;//��©��� /*GPIO_Mode_Out_PP;*/
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	GPIO_SetBits(GPIOB, GPIO_Pin_7|GPIO_Pin_6);

}

//����IIC��ʼ�ź�
void I2C_Start(void)
{
	SDA_H;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_L;
}

//����IICֹͣ�ź�
void I2C_Stop(void)
{
	
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
    I2C_delay();
	SDA_H;
	I2C_delay();
} 

//�ȴ�Ӧ���źŵ���
//����Ϊ:=1��ACK,=0��ACK
bool I2C_WaitAck(void) 	 
{
	u8 errtime = 0;
	SCL_L;
	I2C_delay();
	SDA_H;//������������׼��������			
	I2C_delay();
	SCL_H;
	I2C_delay();
	while(SDA_read)
	{
		errtime++;
		if(errtime>250)
		{
			I2C_Stop();
			return FALSE;	
		}
	}
	SCL_L;//ʱ������
	I2C_delay();
	return TRUE;
}

//����ACKӦ��
void I2C_Ack(void)
{	
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
}

//������ACKӦ��
void I2C_NoAck(void)
{	
	SCL_L;
	I2C_delay();
	SDA_H;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
}

//IIC����һ���ֽ�
//���ݴӸ�λ����λ//
void I2C_SendByte(u8 SendByte)
{
    u8 i;
    for(i=0;i<8;i++)
    {
        SCL_L;
        I2C_delay();
      	if(SendByte&(0x80>>i)) SDA_H;  
      	else SDA_L;   
        I2C_delay();
		    SCL_H;
        I2C_delay();
    }
    SCL_L;
	I2C_delay();
}

//��1���ֽ� 
//���ݴӸ�λ����λ// 
u8 I2C_ReadByte(void)  
{ 
    u8 i=0;
    u8 ReceiveByte=0;
				
   	for(i=0;i<8;i++)
    {      
		SCL_L;
		SDA_H;//���������� ׼����ȡ����
		I2C_delay();
		SCL_H;
		ReceiveByte	<<= 1;
        if(SDA_read)
		{
			ReceiveByte++; 
		}
		else
		{

		}

		I2C_delay();
    }
    SCL_L;
	I2C_delay();
  return ReceiveByte;
}

//******���ֽ�д��**********//
bool Single_Write(u8 REG_Address,u8 REG_data,u8 SlaveAddress)
{
	I2C_Start();
	I2C_SendByte(SlaveAddress);//�����豸��ַ+д�ź�
	while(!I2C_WaitAck());
	I2C_SendByte(REG_Address);//���õ���ʼ��ַ
	while(!I2C_WaitAck());
	I2C_SendByte(REG_data);
	while(!I2C_WaitAck());
	I2C_Stop();
	delay_ms(5);
	return TRUE;	
}

//********���ֽڶ�ȡ*********//
u8 Single_Read(u8 REG_Address,u8 SlaveAddress)
{
	u8 data=0;
	I2C_Start();
	I2C_SendByte(SlaveAddress);//�����豸��ַ+д�ź�
	while(!I2C_WaitAck());
	I2C_SendByte(REG_Address);   //���õ���ʼ��ַ
	while(!I2C_WaitAck()); 
	I2C_Start();
	I2C_SendByte(SlaveAddress+1);//�����豸��ַ+���ź�
	while(!I2C_WaitAck());

	data = I2C_ReadByte();
	I2C_NoAck();
    I2C_Stop();

	return data;	
}

//���ֽ�д��//
void Multiple_write(u8 star_addr,u8 num,u8 SlaveAddress,u8* send_buf)
{
	u8 i;
	I2C_Start();
	I2C_SendByte(SlaveAddress);//�����豸��ַ
	while(!I2C_WaitAck());
	I2C_SendByte(star_addr);//������ʼ��ַ
	while(!I2C_WaitAck());
	for(i=0;i<num;i++)
	{
		I2C_SendByte(send_buf[i]);
		while(!I2C_WaitAck());
	}
	I2C_Stop();
}

//��ȡ����ֽڵ�����
void Multiple_read(u8 star_addr,u8 num,u8 SlaveAddress,u8* recv_buf)
{
	u8 i;
	
	I2C_Start();
	I2C_SendByte(SlaveAddress);//�����豸��ַ
	while(!I2C_WaitAck());
	I2C_SendByte(star_addr);//������ʼ��ַ
	while(!I2C_WaitAck());
	
	I2C_Start();
	I2C_SendByte(SlaveAddress+1);//�����豸��ַ+���ź�
	while(!I2C_WaitAck());
	for(i=0;i<num;i++)
	{
		recv_buf[i] = I2C_ReadByte();
		if(i == (num-1)) I2C_NoAck();//���һ��������Ҫ��NOACK
		else I2C_Ack();
	}
	I2C_Stop();//ֹͣ�ź�
}




///////////////////////////////////////////MPU6050�ӿں���////////////////////////////////////////////////////////
/*void i2cRead(u8 SlaveAddress,u8 star_addr,u8 num,u8* recv_buf)
{
	Multiple_read(star_addr,num,SlaveAddress,recv_buf);
}

void i2cWrite(u8 SlaveAddress,u8 REG_Address,u8 REG_data)
{
	Single_Write(REG_Address,REG_data,SlaveAddress);
}*/



