/*
 * @Author: your name
 * @Date: 2021-03-08 12:14:24
 * @LastEditTime: 2021-06-02 14:43:35
 * @LastEditors: Liqiang Zhao
 * @Description: In User Settings Edit
 * @FilePath: \USERc:\Users\zliqiang\Desktop\ʵ��42 FATFSӲ�����������˰�������\ʵ��42 FATFSʵ��\HARDWARE\GPIO\gpios.c
 */
#include "gpios.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "gnss.h"
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


u8 ext_test_value = 0;
u8 read_imu_flag=0;
u8 read_gnss_flag=0;


void GPIOS_Init()
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();           //����GPIOAʱ��
    __HAL_RCC_GPIOB_CLK_ENABLE();           //����GPIOAʱ��
    __HAL_RCC_GPIOC_CLK_ENABLE();           //����GPIOCʱ��
    __HAL_RCC_GPIOD_CLK_ENABLE();           //����GPIODʱ��
    __HAL_RCC_GPIOF_CLK_ENABLE();           //����GPIOHʱ��
    __HAL_RCC_GPIOH_CLK_ENABLE();           //����GPIOHʱ��
    
    
    // GPIO_Initure.Pin=GPIO_PIN_0;            //PA0
    // GPIO_Initure.Mode=GPIO_MODE_INPUT;      //����
    // GPIO_Initure.Pull=GPIO_PULLDOWN;        //����
    // GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    // HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    
    // GPIO_Initure.Pin=GPIO_PIN_13;           //PC13
    // GPIO_Initure.Mode=GPIO_MODE_INPUT;      //����
    // GPIO_Initure.Pull=GPIO_PULLUP;          //����
    // GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    // HAL_GPIO_Init(GPIOC,&GPIO_Initure);
    
    // GPIO_Initure.Pin=GPIO_PIN_2|GPIO_PIN_3; //PH2,3
    // HAL_GPIO_Init(GPIOH,&GPIO_Initure);

    GPIO_Initure.Pin=GPIO_PIN_1;             //BLE_RST
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);

    GPIO_Initure.Pin=GPIO_PIN_0;             //BLE_EN
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);	//BLE_RST��1
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);	//BLE_EN��1

    GPIO_Initure.Pin=GPIO_PIN_13;             //4G_NRST
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);

    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_SET);	//4G_NRST��1

    GPIO_Initure.Pin=GPIO_PIN_4;             //IMU_RST
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);

    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_SET);	//IMU_RST��1

    GPIO_Initure.Pin=GPIO_PIN_8;             //GNSS_RST
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);

    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_8,GPIO_PIN_SET);	//GNSS_RST��1


    GPIO_Initure.Pin=GPIO_PIN_15;             //BLE_EN
    GPIO_Initure.Mode=GPIO_MODE_IT_RISING;  //�������
    GPIO_Initure.Pull=GPIO_PULLDOWN;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);

    
    HAL_NVIC_SetPriority(EXTI15_10_IRQn,2,1);      
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);             

}

void EXTI15_10_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);   
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    static u8 led0sta=1,led1sta=1;
    switch(GPIO_Pin)
    {
        case GPIO_PIN_15:
            {
                read_imu_flag = 1;
                read_gnss_flag =1;
                gnss_time = time_write;
								s5_tail = 0;
               
            break;}
    }
}
