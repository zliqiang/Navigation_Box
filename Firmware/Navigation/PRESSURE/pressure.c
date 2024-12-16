/*
 * @Description: 
 * @Author: Liqiang Zhao
 * @Github: https://github.com/zliqiang
 * @Date: 2021-04-23 16:23:04
 * @LastEditors: Liqiang Zhao
 * @LastEditTime: 2021-11-06 15:54:29
 * @FilePath: \USERd:\项目文件\香港地铁\HK_Metro新旧硬件通用版本\实验42 FATFS实验\Navigation\PRESSURE\pressure.c
 */

#include "pressure.h"


void read_pressure(void)
{
	int dot_num[2];
	int dot_count = 0;
	int pressure_space_num ,height_space_num;
	int height_f_flag=0;
	//u8 dot_exit = 0;
	if(s6_rx_buf[0] != 80)
		{
			s6_tail = 0;
		}
	if(s6_recv_over == 1)
	{
		pressure = 0;
		height = 0;
		for(int i=0;i<s6_recv_length;i++)
		{
			if(s6_rx_buf[i] == '.')
			{
				dot_num[dot_count++] = i;
				if(dot_count >= 2)
				{dot_count = 0;}
			}
		}
		for(int i = dot_num[0];i>0;i--)
		{
			if(s6_rx_buf[i] == ' ')
			{
				pressure_space_num = i;
				break;
			}
			
		}
		for(int i = dot_num[1];i>0;i--)
		{
			if(s6_rx_buf[i] == ' ')
			{
				height_space_num = i;
				break;
			}
		}
		
		for(int i=dot_num[0]-1;i>=pressure_space_num+1;i--)
		{
			pressure += (s6_rx_buf[i]-48)*pow(10,dot_num[0]-i-1);
		}
		pressure += (s6_rx_buf[dot_num[0]+1]-48)*0.1;
		
		if(s6_rx_buf[height_space_num+1] == '-')
		{

			for(int i=dot_num[1]-1;i>=height_space_num+2;i--)
			{
				height += (s6_rx_buf[i]-48)*pow(10,dot_num[1]-i-1);
			}
			height += (s6_rx_buf[dot_num[1]+1]-48)*0.1;
			height = height*(-1);
		}
		else
		{
			for(int i=dot_num[1]-1;i>=height_space_num+1;i--)
			{
				height += (s6_rx_buf[i]-48)*pow(10,dot_num[1]-i-1);
			}
			height += (s6_rx_buf[dot_num[1]+1]-48)*0.1;
		}
		if((pressure < 10000000)&&(pressure > 1000))
		{
			pressure = pressure;
		}
		
		s6_recv_over=0;
		char rx_buff_4g0[100];
		sprintf(rx_buff_4g0,"{\"pressure\":%f,\"height\":%f}",pressure,height);
		HAL_UART_Transmit(&UART3_Handler,rx_buff_4g0,100,1000);
	}
}