/*********************************Copyright (c)*********************************
**                               
**
**--------------File Info-------------------------------------------------------
** File Name:               sim_i2c.h
** Last modified Date:      
** Last Version:            
** Description:             i2cÅäÖÃÎÄ¼þ
**
**------------------------------------------------------------------------------
** Created By:              wanxuncpx
** Created date:            
** Version:                 
** Descriptions:            
**
*******************************************************************************/

/******************************************************************************
¸üÐÂËµÃ÷:
    
******************************************************************************/

/******************************************************************************
*********************************  Ó¦ ÓÃ ×Ê ÁÏ ********************************
******************************************************************************/


#ifndef _IIC_H_
#define _IIC_H_

#ifdef __cplusplus
 extern "C" {
#endif
/******************************************************************************
********************************* ÎÄ¼þÒýÓÃ²¿·Ö ********************************
******************************************************************************/
#include "SYS.h"
#include <stdbool.h>



//************************** ICCÒý½Å *********************************
//´«¸ÐÆ÷I2C

/***** ²Ù×÷ºê¶¨Òå *****/
#define SCL_H         (GPIOB->BSRR = GPIO_Pin_6)
#define SCL_L         (GPIOB->BRR  = GPIO_Pin_6)    
#define SDA_H         (GPIOB->BSRR = GPIO_Pin_7)
#define SDA_L         (GPIOB->BRR  = GPIO_Pin_7)
#define SCL_read      (GPIOB->IDR  & GPIO_Pin_6)
#define SDA_read      (GPIOB->IDR  & GPIO_Pin_7)


//none
#define I2C_DELAY_VAL   2   //30¶ÔÓ¦100K,¾­²âÊÔ×îµÍµ½4»¹ÄÜÐ´È,È¡Öµ[7..255]
                             //4¶ÔÓ¦400K
                                                          

//I2CÍêÕû¶ÁÐ´º¯Êý
extern void    I2C_IO_Init(void);               //ÅäÖÃI2C½Ó¿Ú
extern bool    Single_Write(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t REG_data);
extern uint8_t Single_Read(uint8_t SlaveAddress,uint8_t REG_Address);
extern bool    Fast_Write(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t REG_data);
extern bool    Mult_Read(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t * ptChar,uint8_t size);

//I2C·ÖÁ¢²Ù×÷º¯Êý
extern bool I2C_Start(void);              //×ÜÏßÃ¦»î»ò³ö´íÔò·µ»Øfalse
extern void I2C_Stop(void);
extern void I2C_Ack(void);
extern void I2C_NoAck(void);
extern bool I2C_WaitAck(void); 	          //·µ»ØÎª:=1ÓÐACK,=0ÎÞACK
extern void I2C_SendByte(u8 SendByte);    //Êý¾Ý´Ó¸ßÎ»µ½µÍÎ»
extern uint8_t I2C_RadeByte(void);  //Êý¾Ý´Ó¸ßÎ»µ½µÍÎ»



/*************************************************************
************************* Æä Ëü ÔÓ Ïî ************************
*************************************************************/
//none
 
/******************************************************************************
***********************************   END  ************************************
******************************************************************************/
#ifdef __cplusplus
}
#endif

#endif



