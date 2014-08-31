#ifndef ___SendData_H
#define ___SendData_H
#include "SYS.h"
#include "DMA.h"
//DMA在发送过程中不断查询完成标志位，直至发送完成 DMA在main函数中，不影响定时中断与输入捕获中断
void UART1_ReportIMU(s16 yaw,s16 pitch,s16 roll,s16 alt,s16 tempr,s16 press,s16 IMUpersec);
void UART1_ReportMotion(s16 ax,s16 ay,s16 az,s16 gx,s16 gy,s16 gz,s16 hx,s16 hy,s16 hz);
void DMA_UART1_ReportIMU(s16 yaw,s16 pitch,s16 roll,s16 alt,s16 tempr,s16 press,s16 IMUpersec);
void DMA_UART1_ReportMotion(s16 ax,s16 ay,s16 az,s16 gx,s16 gy,s16 gz,s16 hx,s16 hy,s16 hz);
void SCI_DIS(u32 ch0, u32 ch1,u32 ch2);


#endif
