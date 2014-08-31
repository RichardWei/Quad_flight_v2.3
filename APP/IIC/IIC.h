/*********************************Copyright (c)*********************************
**                               
**
**--------------File Info-------------------------------------------------------
** File Name:               sim_i2c.h
** Last modified Date:      
** Last Version:            
** Description:             i2c�����ļ�
**
**------------------------------------------------------------------------------
** Created By:              wanxuncpx
** Created date:            
** Version:                 
** Descriptions:            
**
*******************************************************************************/

/******************************************************************************
����˵��:
    
******************************************************************************/

/******************************************************************************
*********************************  Ӧ �� �� �� ********************************
******************************************************************************/


#ifndef _IIC_H_
#define _IIC_H_

#ifdef __cplusplus
 extern "C" {
#endif
/******************************************************************************
********************************* �ļ����ò��� ********************************
******************************************************************************/
#include "SYS.h"
#include <stdbool.h>



//************************** ICC���� *********************************
//������I2C

/***** �����궨�� *****/
#define SCL_H         (GPIOB->BSRR = GPIO_Pin_6)
#define SCL_L         (GPIOB->BRR  = GPIO_Pin_6)    
#define SDA_H         (GPIOB->BSRR = GPIO_Pin_7)
#define SDA_L         (GPIOB->BRR  = GPIO_Pin_7)
#define SCL_read      (GPIOB->IDR  & GPIO_Pin_6)
#define SDA_read      (GPIOB->IDR  & GPIO_Pin_7)


//none
#define I2C_DELAY_VAL   2   //30��Ӧ100K,��������͵�4����д�,ȡֵ[7..255]
                             //4��Ӧ400K
                                                          

//I2C������д����
extern void    I2C_IO_Init(void);               //����I2C�ӿ�
extern bool    Single_Write(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t REG_data);
extern uint8_t Single_Read(uint8_t SlaveAddress,uint8_t REG_Address);
extern bool    Fast_Write(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t REG_data);
extern bool    Mult_Read(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t * ptChar,uint8_t size);

//I2C������������
extern bool I2C_Start(void);              //����æ�������򷵻�false
extern void I2C_Stop(void);
extern void I2C_Ack(void);
extern void I2C_NoAck(void);
extern bool I2C_WaitAck(void); 	          //����Ϊ:=1��ACK,=0��ACK
extern void I2C_SendByte(u8 SendByte);    //���ݴӸ�λ����λ
extern uint8_t I2C_RadeByte(void);  //���ݴӸ�λ����λ



/*************************************************************
************************* �� �� �� �� ************************
*************************************************************/
//none
 
/******************************************************************************
***********************************   END  ************************************
******************************************************************************/
#ifdef __cplusplus
}
#endif

#endif



