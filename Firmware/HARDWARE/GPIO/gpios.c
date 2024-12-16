/*
 * @Author: your name
 * @Date: 2021-03-08 12:14:24
 * @LastEditTime: 2021-06-02 14:43:35
 * @LastEditors: Liqiang Zhao
 * @Description: In User Settings Edit
 * @FilePath: \USERc:\Users\zliqiang\Desktop\实验42 FATFS硬件基本可以了啊啊啊啊\实验42 FATFS实验\HARDWARE\GPIO\gpios.c
 */
#include "gpios.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "gnss.h"
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


u8 ext_test_value = 0;
u8 read_imu_flag=0;
u8 read_gnss_flag=0;


void GPIOS_Init()
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();           //开启GPIOA时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();           //开启GPIOA时钟
    __HAL_RCC_GPIOC_CLK_ENABLE();           //开启GPIOC时钟
    __HAL_RCC_GPIOD_CLK_ENABLE();           //开启GPIOD时钟
    __HAL_RCC_GPIOF_CLK_ENABLE();           //开启GPIOH时钟
    __HAL_RCC_GPIOH_CLK_ENABLE();           //开启GPIOH时钟
    
    
    // GPIO_Initure.Pin=GPIO_PIN_0;            //PA0
    // GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    // GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    // GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    // HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    
    // GPIO_Initure.Pin=GPIO_PIN_13;           //PC13
    // GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    // GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    // GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    // HAL_GPIO_Init(GPIOC,&GPIO_Initure);
    
    // GPIO_Initure.Pin=GPIO_PIN_2|GPIO_PIN_3; //PH2,3
    // HAL_GPIO_Init(GPIOH,&GPIO_Initure);

    GPIO_Initure.Pin=GPIO_PIN_1;             //BLE_RST
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);

    GPIO_Initure.Pin=GPIO_PIN_0;             //BLE_EN
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);	//BLE_RST置1
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);	//BLE_EN置1

    GPIO_Initure.Pin=GPIO_PIN_13;             //4G_NRST
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);

    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_SET);	//4G_NRST置1

    GPIO_Initure.Pin=GPIO_PIN_4;             //IMU_RST
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);

    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_SET);	//IMU_RST置1

    GPIO_Initure.Pin=GPIO_PIN_8;             //GNSS_RST
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);

    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_8,GPIO_PIN_SET);	//GNSS_RST置1


    GPIO_Initure.Pin=GPIO_PIN_15;             //BLE_EN
    GPIO_Initure.Mode=GPIO_MODE_IT_RISING;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLDOWN;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
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
