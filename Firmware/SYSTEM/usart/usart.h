/*
 * @Author: zliqiang
 * @Date: 2021-03-08 12:14:24
 * @LastEditTime: 2021-06-02 17:27:24
 * @LastEditors: Liqiang Zhao
 * @Description: In User Settings Edit
 * @FilePath: \USERc:\Users\zliqiang\Desktop\实验42 FATFS硬件基本可以了啊啊啊啊\实验42 FATFS实验\SYSTEM\usart\usart.h
 */
#ifndef _USART_H
#define _USART_H
#include "sys.h"
#include "stdio.h"	
#include "timer.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F7开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.csom
//修改日期:2015/6/23
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
//********************************************************************************
//V1.0修改说明 
////////////////////////////////////////////////////////////////////////////////// 	
#define USART_REC_LEN  			1500  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
#define EN_USART2_RX 			1		//使能（1）/禁止（0）串口1接收
#define EN_USART3_RX 			1		//使能（1）/禁止（0）串口1接收
#define EN_UART4_RX 			1		//使能（1）/禁止（0）串口1接收
#define EN_UART5_RX 			1		//使能（1）/禁止（0）串口1接收
#define EN_USART6_RX 			1		//使能（1）/禁止（0）串口1接收
#define EN_UART7_RX 			1		//使能（1）/禁止（0）串口1接收
//#define EN_UART7_RX 			1		//使能（1）/禁止（0）串口1接收




extern int test_val00;


//#define S1_RX_SIZE 54
#define S1_RX_SIZE 54		//108
#define S4_RX_SIZE 500			//16
#define S6_RX_SIZE 44			//44
#define S5_RX_SIZE 5000		//3000

extern uint32_t imu_time_global;
extern float imu_data_global[9];
extern float imu_temp_global;
extern float pressure;
extern float height;

extern u8 s1_rx_buf[S1_RX_SIZE];
extern int s1_tail;
extern u8 s1_recv_over;

extern u8 s6_rx_buf[S6_RX_SIZE];
extern int s6_tail;
extern u8 s6_recv_over;
extern int s6_recv_length;

extern u8 s5_rx_buf[S5_RX_SIZE];
extern int s5_tail;
extern u8 s5_recv_over;
extern int s5_recv_length;

extern u8 s4_rx_buf[S4_RX_SIZE];
extern int s4_tail;
extern u8 s4_recv_over;







	  	
extern u8  USART1_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u8  USART2_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u8  USART3_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u8  UART4_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u8  UART5_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u8  USART6_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u8  UART7_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 

extern u16 USART1_RX_STA;         		//接收状态标记		
extern u16 USART2_RX_STA;         		//接收状态标记	
extern u16 USART3_RX_STA;         		//接收状态标记	
extern u16 UART4_RX_STA;         		//接收状态标记	
extern u16 UART5_RX_STA;         		//接收状态标记	
extern u16 USART6_RX_STA;         		//接收状态标记	
extern u16 UART7_RX_STA;         		//接收状态标记	

extern UART_HandleTypeDef UART1_Handler; //UART句柄
extern UART_HandleTypeDef UART2_Handler; //UART句柄
extern UART_HandleTypeDef UART3_Handler; //UART句柄
extern UART_HandleTypeDef UART4_Handler; //UART句柄
extern UART_HandleTypeDef UART5_Handler; //UART句柄
extern UART_HandleTypeDef UART6_Handler; //UART句柄
extern UART_HandleTypeDef UART7_Handler; //UART句柄

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

#define RXBUFFERSIZE   16 //缓存大小
extern u8 aRxBuffer[RXBUFFERSIZE];//HAL库USART接收Buffer

extern u8 gps_debug_start;
extern u8 gps_value;
extern u8 ble_test_val;


//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);
void MX_USART2_UART_Init(void);
void MX_USART3_UART_Init(void);
void read_imu1();
void read_imu();
void imu_error_handle();

#endif
