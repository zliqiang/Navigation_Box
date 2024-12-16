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
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F7������
//KEY��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/11/27
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
#define KEY0        HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_3)  //KEY0����PH3
#define KEY1        HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_2)  //KEY1����PH2
#define KEY2        HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13) //KEY2����PC13
#define WK_UP       HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)  //WKUP����PA0

#define KEY0_PRES 	1
#define KEY1_PRES	2
#define KEY2_PRES	3
#define WKUP_PRES   4

extern u8 ext_test_value;
extern u8 read_imu_flag;
extern u8 read_gnss_flag;

void GPIOS_Init(void);


#endif
