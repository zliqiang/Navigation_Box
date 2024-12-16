/*
 * @Author: your name
 * @Date: 2021-03-10 22:15:13
 * @LastEditTime: 2021-06-02 11:00:05
 * @LastEditors: Liqiang Zhao
 * @Description: In User Settings Edit
 * @FilePath: \USERc:\Users\zliqiang\Desktop\ʵ��42 FATFSӲ�����������˰�������\ʵ��42 FATFSʵ��\HARDWARE\TIMER\timer.h
 */
#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F7������
//��ʱ����������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/11/27
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
extern TIM_HandleTypeDef TIM3_Handler;      //��ʱ��3PWM��� 
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

void TIM3_Init(u16 arr,u16 psc);    //��ʱ����ʼ��
void TIM2_Init(u16 arr,u16 psc);    //��ʱ����ʼ��
#endif

