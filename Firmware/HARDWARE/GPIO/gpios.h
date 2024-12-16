/*
 * @Author: zliqiang
 * @Date: 2021-03-08 12:14:24
 * @LastEditTime: 2021-04-29 16:34:23
 * @LastEditors: Liqiang Zhao
 * @Description: In User Settings Edit
 * @FilePath: \HK_METRO1.0\HARDWARE\GPIO\gpios.h
 */
#ifndef _GPIOS_H
#define _GPIOS_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F7开发板
//KEY驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/11/27
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
#define KEY0        HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_3)  //KEY0按键PH3
#define KEY1        HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_2)  //KEY1按键PH2
#define KEY2        HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13) //KEY2按键PC13
#define WK_UP       HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)  //WKUP按键PA0

#define KEY0_PRES 	1
#define KEY1_PRES	2
#define KEY2_PRES	3
#define WKUP_PRES   4

extern u8 ext_test_value;
extern u8 read_imu_flag;
extern u8 read_gnss_flag;

void GPIOS_Init(void);


#endif
