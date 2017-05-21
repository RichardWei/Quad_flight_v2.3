/*********************************Copyright (c)*********************************
**                               
**
**--------------File Info-------------------------------------------------------
** File Name:               sim_i2c.c
** Last modified Date:      
** Last Version:            
** Description:             ģI2Cӿ(Ĭ100kbps)
**
**------------------------------------------------------------------------------
** Created By:              wanxuncpx
** Created date:            
** Version:                 
** Descriptions:            
**
*******************************************************************************/

/******************************************************************************
˵:
    
******************************************************************************/

/******************************************************************************
*********************************  Ӧ    ********************************
******************************************************************************/

/******************************************************************************
********************************* ļò ********************************
******************************************************************************/
#include "IIC.h"


/******************************************************************************
******************************* Զ ********************************
******************************************************************************/


/******************************************************************************
*********************************     *********************************
******************************************************************************/
/*---------------------* 
*    IMPORT:ṩ   * 
*----------------------*/
//none

/*---------------------* 
*    EXPORT:ṩ   * 
*----------------------*/
//none

/******************************************************************************
*********************************     *********************************
******************************************************************************/
char  test=0;
/*---------------------* 
*    IMPORT:ṩ   * 
*----------------------*/
//none

/*---------------------* 
*    EXPORT:ṩ   * 
*----------------------*/
//none



/******************************************************************************
*********************************  ʼ  **********************************
******************************************************************************/
/******************************************************************************
/ :msʱ
/ ޸:none
/ :none
/ :none
/ ʹ˵:none
******************************************************************************/
void Delayms(uint32_t time)
{
   uint16_t i=0;  
   while(time--)
   {
      i=8000;
      while(i--);
   }  
}

/******************************************************************************
/ :I2Cӿó
/ ޸:none
/ :none
/ :none
/ ʹ˵:none
******************************************************************************/
void I2C_IO_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
    
  //رJTAGʹSWDͷIO
  uint32_t u32Temp;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB , ENABLE);
  u32Temp = AFIO->MAPR;
  u32Temp &= ~AFIO_MAPR_SWJ_CFG ;
  u32Temp |= AFIO_MAPR_SWJ_CFG_1;
  AFIO->MAPR = u32Temp;
  
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB , ENABLE);
  //GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//GPIOCʱ	 
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_7|GPIO_Pin_6; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;//GPIO_Mode_Out_OD;//© /*GPIO_Mode_Out_PP;*/
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_7|GPIO_Pin_6);
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11);

}

/******************************************************************************
/ :Simulation IIC Timing series delay
/ ޸:none
/ :none
/ :none
/ ʹ˵:none
******************************************************************************/
 void I2C_delay(void)
{
        
   u8 i=I2C_DELAY_VAL; //Żٶ   ͵5д
   while(i) 
   { 
     i--; 
   }  
}




/*******************************************************************************
* Function Name  : I2C_Start
* Description    : Master Start Simulation IIC Communication
* Input          : None
* Output         : None
* Return         : Wheather  Start
****************************************************************************** */
bool I2C_Start(void)
{
    SDA_H;
    SCL_H;
    I2C_delay();
    if(!SDA_read)return false;  //SDAΪ͵ƽæ,˳
    SDA_L;
    I2C_delay();
    if(SDA_read) return false;  //SDAΪߵƽ߳,˳
    SDA_L;
    I2C_delay();
    return true;
}
/*******************************************************************************
* Function Name  : I2C_Stop
* Description    : Master Stop Simulation IIC Communication
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
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
/*******************************************************************************
* Function Name  : I2C_Ack
* Description    : Master Send Acknowledge Single
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
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
/*******************************************************************************
* Function Name  : I2C_NoAck
* Description    : Master Send No Acknowledge Single
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
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
/*******************************************************************************
* Function Name  : I2C_WaitAck
* Description    : Master Reserive Slave Acknowledge Single
* Input          : None
* Output         : None
* Return         : Wheather  Reserive Slave Acknowledge Single
****************************************************************************** */
bool I2C_WaitAck(void)   //Ϊ:=1ACK,=0ACK
{
    SCL_L;
    I2C_delay();
    SDA_H;          
    I2C_delay();
    SCL_H;
    I2C_delay();
    if(SDA_read)
    {
      SCL_L;
      I2C_delay();
      return false;
    }
    SCL_L;
    I2C_delay();
    return true;
}
/*******************************************************************************
* Function Name  : I2C_SendByte
* Description    : Master Send a Byte to Slave
* Input          : Will Send Date
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_SendByte(u8 SendByte) //ݴӸλλ//
{
    u8 i=8;
    while(i--)
    {
        SCL_L;
        I2C_delay();
      if(SendByte&0x80)
        SDA_H;  
      else 
        SDA_L;   
        SendByte<<=1;
        I2C_delay();
        SCL_H;
        I2C_delay();
    }
    SCL_L;
}  
/*******************************************************************************
* Function Name  : I2C_RadeByte
* Description    : Master Reserive a Byte From Slave
* Input          : None
* Output         : None
* Return         : Date From Slave 
****************************************************************************** */
uint8_t I2C_RadeByte(void)  //ݴӸλλ//
{ 
    u8 i=8;
    u8 ReceiveByte=0;

    SDA_H;              
    while(i--)
    {
        ReceiveByte<<=1;      
        SCL_L;
        I2C_delay();
        SCL_H;
        I2C_delay();  
        if(SDA_read)
        {
          ReceiveByte|=0x01;
        }
    }
    SCL_L;
    return ReceiveByte;
} 
/******************************************************************************
/ :ֽд
/ ޸:none
/ :
/   @arg SlaveAddress   ַ
/   @arg REG_Address    Ĵַ
/   @arg REG_data       дֽ
/ : ֽ
/ ʹ˵:ʱһĵֽڶȡ
******************************************************************************/
bool Single_Write(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t REG_data)
{
    if(!I2C_Start())return false;
    I2C_SendByte(SlaveAddress);   //豸ַ+дź//I2C_SendByte(((REG_Address & 0x0700) >>7) | SlaveAddress & 0xFFFE);//øʼַ+ַ 
    if(!I2C_WaitAck()){I2C_Stop(); return false;}
    I2C_SendByte(REG_Address );   //õʼַ      
    I2C_WaitAck();  
    I2C_SendByte(REG_data);
    I2C_WaitAck();   
    I2C_Stop(); 
    //delay5ms();
    return true;
}

/******************************************************************************
/ :ֽд
/ ޸:none
/ :
/   @arg SlaveAddress   ַ
/   @arg REG_Address    Ĵַ
/   @arg REG_data       дֽ
/ : ֽ
/ ʹ˵:ʱһĵֽڶȡ
******************************************************************************/
bool Fast_Write(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t REG_data)
{
    if(!I2C_Start())return false;
    I2C_SendByte(SlaveAddress);   //豸ַ+дź//I2C_SendByte(((REG_Address & 0x0700) >>7) | SlaveAddress & 0xFFFE);//øʼַ+ַ 
    if(!I2C_WaitAck()){I2C_Stop(); return false;}
    I2C_SendByte(REG_Address );   //õʼַ      
    I2C_WaitAck();  
    I2C_SendByte(REG_data);
    I2C_WaitAck();   
    I2C_Stop(); 
    return true;
}



/******************************************************************************
/ :ֽд
/ ޸:none
/ :
/   @arg SlaveAddress   ַ
/   @arg REG_Address    Ĵַ
/ : ֽ
/ ʹ˵:ʱһĵֽڶȡ
******************************************************************************/
uint8_t Single_Read(uint8_t SlaveAddress,uint8_t REG_Address)
{   
    uint8_t REG_data;       
    if(!I2C_Start())return false;
    I2C_SendByte(SlaveAddress); //I2C_SendByte(((REG_Address & 0x0700) >>7) | REG_Address & 0xFFFE);//øʼַ+ַ 
    if(!I2C_WaitAck()){I2C_Stop();return false;}
    I2C_SendByte((u8) REG_Address);   //õʼַ      
    I2C_WaitAck();
    I2C_Start();
    I2C_SendByte(SlaveAddress+1);
    I2C_WaitAck();

    REG_data= I2C_RadeByte();
    I2C_NoAck();
    I2C_Stop();
    //return true;
    return REG_data;
}

/******************************************************************************
/ :ֽڶ
/ ޸:none
/ :
/   @arg SlaveAddress   ַ
/   @arg REG_Address    Ĵַ
/   @arg ptChar         �
/   @arg size           ݸ,size=1
/ : ɹʧܱ
/ ʹ˵:none
******************************************************************************/
bool Mult_Read(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t * ptChar,uint8_t size)
{
    uint8_t i;
    
    if(size < 1)return false;
    if(!I2C_Start())return false;
    I2C_SendByte(SlaveAddress);
    if(!I2C_WaitAck()){I2C_Stop();return false;}
    I2C_SendByte(REG_Address);    
    I2C_WaitAck();
    
    I2C_Start();
    I2C_SendByte(SlaveAddress+1);
    I2C_WaitAck();
    
    //ax,ay,az
    for(i=1;i<size; i++)
    {
        *ptChar++ = I2C_RadeByte();
        I2C_Ack();
    }
    *ptChar++ = I2C_RadeByte();
    I2C_NoAck();
    I2C_Stop();
    return true;    
}




