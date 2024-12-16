/*
 * @Author: your name
 * @Date: 2021-03-10 22:15:13
 * @LastEditTime: 2021-06-02 11:00:05
 * @LastEditors: Liqiang Zhao
 * @Description: In User Settings Edit
 * @FilePath: \USERc:\Users\zliqiang\Desktop\实验42 FATFS硬件基本可以了啊啊啊啊\实验42 FATFS实验\HARDWARE\TIMER\timer.h
 */
#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F7开发板
//定时器驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/11/27
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
extern TIM_HandleTypeDef TIM3_Handler;      //定时器3PWM句柄 
extern int time_count1;

extern float time_write;
extern u8 time_1s_flag;
extern u8 gnss_strat_recv;
extern u8 time_5s_flag;
extern u8 time_10ms_flag;

extern int time_gnss_cal;
extern u8 gnss_start_cal;
extern u8 time_check_over;
extern float time_err;
extern float time_write_imu;

void TIM3_Init(u16 arr,u16 psc);    //定时器初始化
void TIM2_Init(u16 arr,u16 psc);    //定时器初始化
#endif

