/*
 * @Author: zliqiang
 * @Date: 2021-03-08 12:14:24
 * @LastEditTime: 2021-06-04 17:27:28
 * @LastEditors: Liqiang Zhao
 * @Description: In User Settings Edit
 * @FilePath: \USER\main.c
 */
 
#include "main.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "gpios.h"
#include "mpu.h"
#include "timer.h"
#include "lcd.h"
#include "ltdc.h"
#include "sdram.h"
#include "touch.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "malloc.h"
#include "sdmmc_sdcard.h"
#include "ftl.h"
#include "exfuns.h"
#include "nand.h"
#include "w25qxx.h"
#include "gnss.h"
#include "math.h"
#include "imu.h"
#include "calculate.h"
#include "main_win.h"

//注:对于littleVGL库,最好不要采用-O的优化,保持为-O0不优化就可以了

uint32_t data_file_num_init();
void read_station();
void ble_start();
float sd_time_temp = 0;
float sd_time=0;


#define TEST_NUM		2   //1,2,3·?±e??ó|3??ày3ì


int main(void)
{
	u8 res=0;	
	char imu_data_temp[80];
	char rx_buff_4g[100];
	u8 debug_buff[20];
	char imu_data_write1[120];//160
	char imu_data_write2[60];//160
	char imu_data_num_str[20];
	uint32_t imu_data_num;
	UINT bw;
	Cache_Enable();                 //打开L1-Cache		/Open L1-Catch
	MPU_Memory_Protection();		//保护某些存储区域	 /Secure certain storage areas
	HAL_Init();				        //初始化HAL库		/Initialize the HAL Lib
	Stm32_Clock_Init(432,25,2,9);   //设置时钟,216Mhz	/Set the clock,216 MHz
	delay_init(216);                //延时初始化		/delay initialization
	GPIOS_Init();                   //初始化GPIO		/Initialize the GPIO
	uart_init(115200);		        //串口初始化	     /Initialize the USART
	SDRAM_Init();                   //初始化SDRAM		/Initialize the SDRAM
	TIM3_Init(1000-1,107);				//定时器初始化(1ms中断),用于给lvgl提供1ms的心跳节拍
	//TIM2_Init(999,107);
	imu_reset();
	LCD_Init();                     //LCD初始化	
	LCD_Display_Dir(1);				//LCD设置为横屏显示
	tp_dev.init();				    //触摸屏初始化 
	W25QXX_Init();  
 	my_mem_init(SRAMIN);            //初始化内部内存池
  	my_mem_init(SRAMEX);            //初始化外部SDRAM内存池
	my_mem_init(SRAMDTCM); 			//初始化CCM内存池 
	sd_state = SD_Init();				//初始话SD卡
	FTL_Init();
	exfuns_init();					//为fatfs相关变量申请内存				 
  	f_mount(fs[0],"0:",1); 			//挂载SD卡 
 	res=f_mount(fs[1],"1:",1); 		//挂载FLASH.
	
	imu_data_num = data_file_num_init();
	read_station();
	
	lv_init();						//lvgl系统初始化
	lv_port_disp_init();			//lvgl显示接口初始化,放在lv_init()的后面
	lv_port_indev_init();			//lvgl输入接口初始化,放在lv_init()的后面
	//gui_app_start();				//开始界面显示
	
	int write1_size=0;
	ble_start();

	main_win_create();
	u8 test_val;
	
	sprintf(imu_data_num_str,"0:/DATA/data%d.txt",imu_data_num);
	f_open(file,imu_data_num_str,FA_OPEN_ALWAYS|FA_READ|FA_WRITE);//建立txt文件
	
	while(1)
	{
		tp_dev.scan(0);//触摸扫描
		lv_task_handler();//lvgl的事务处理
		
		read_pressure();
		
		if(read_gnss_flag == 1)
		{
			pos_cal_flag = IMU_GNSS;
			if(gnss_start_cal == 1)
			{
				gnss_start_cal=0;
				time_gnss_cal=0;
				read_gnss_flag = 0;
				//s5_tail = 0;
				read_gnss();
				sprintf(rx_buff_4g,"{\"latitude\":%f,\"longitude\":%f}",latitude,longitude);
				HAL_UART_Transmit(&UART3_Handler,rx_buff_4g,100,1000);
				//HAL_UART_Transmit(&UART2_Handler,rx_buff_4g,100,1000);
			}
		}	

		if((write_flag > 0))
		{
			write_flag --;
			time_10ms_flag = 0;
			
			for(int i=0;i<120;i++)
			{
				imu_data_write1[i] = 0;
			}
			//  sprintf(imu_data_write1,"%f\t%f\t%f\t%f\t%f\t%f\t%f\t%lf\t%lf\t%f\t%f\n%f\t%f\t%f\t%f\t%f\t%f\t%f\t%lf\t%lf\t%f\t%f\n%f\t%f\t%f\t%f\t%f\t%f\t%f\t%lf\t%lf\t%f\t%f",imu_time_write[0],ax_write[0],ay_write[0],az_write[0],gx_write[0],gy_write[0],gz_write[0],latitude,longitude,pressure,height,imu_time_write[1],ax_write[1],ay_write[1],az_write[1],gx_write[1],gy_write[1],gz_write[1],latitude,longitude,pressure,height,imu_time_write[2],ax_write[2],ay_write[2],az_write[2],gx_write[2],gy_write[2],gz_write[2],latitude,longitude,pressure,height);
			sprintf(imu_data_write1,"%8.2f\t%f\t%f\t%f\t%f\t%f\t%f\t%lf\t%lf\t%f\t%f\t%d",imu_time_write[imu_whead],ax_write[imu_whead],ay_write[imu_whead],az_write[imu_whead],gx_write[imu_whead],gy_write[imu_whead],gz_write[imu_whead],latitude,longitude,pressure,height,write_flag);
			imu_whead++;
			if(imu_whead == BUFF_SIZE)
			{
				imu_whead=0;
			}
			imu_data_write1[119] = '\n';
			f_lseek(file,f_size(file));
			f_write(file,imu_data_write1,120,&bw);
		}
		
		if(s4_recv_over == 1)
		{
			s4_recv_over = 0;
			for(int i=0;i<S4_RX_SIZE;i++)
			{
				if(!memcmp(&s4_rx_buf[i],"MAC   :",strlen("MAC   :")))
				{
					if(!memcmp(&s4_rx_buf[i+7],"DD:3C:1E:5E:37:01",strlen("DD:3C:1E:5E:37:01")))
						{
							
						}
				}
			}
		}	
	}
}


void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

uint32_t data_file_num_init()
{
	u8 write_str[17];
	u8 write_num_str[4];
	u8 read_file_num[17];
	uint32_t write_num = 0;
	uint16_t byteRead;
	f_open(file,"0:/DATA/data_num.txt",FA_OPEN_ALWAYS|FA_READ|FA_WRITE);//建立txt文件
	f_read(file,read_file_num,sizeof(read_file_num),(void *)&byteRead);
	f_close(file);
	if(byteRead > 0)
	{
		for(int i=0;i<sizeof(read_file_num);i++)
		{
			if(read_file_num[i] == ':')
			{
				for(int j=1;j<sizeof(read_file_num)-i;j++)
				{
					write_num_str[j-1] = read_file_num[i+j];
				}
			}
		}
		for(int i = 0;i<4;i++)
		{
			if(write_num_str[i] == 0)
			{
				switch(i)
				{
					case 0:{write_num = 0;break;}
					case 1:{write_num = write_num_str[0] - 48;break;}
					case 2:{write_num = (write_num_str[0]-48)*10+(write_num_str[2]-48);break;}
					case 3:{write_num = (write_num_str[0]-48)*100+(write_num_str[1]-48)*10+(write_num_str[2]-48);break;}
					default:{write_num = 0;break;}
				}
				break;
			}
		}
		write_num = write_num+1;
		f_unlink("0:/DATA/data_num.txt");
		f_open(file,"0:/DATA/data_num.txt",FA_OPEN_ALWAYS|FA_READ|FA_WRITE);//建立txt文件
		sprintf(write_str,"imu_data_num:%d",write_num);
		f_write(file,write_str,17,&bw);
		f_close(file);
	}
	else 
	{
		f_open(file,"0:/DATA/data_num.txt",FA_OPEN_ALWAYS|FA_READ|FA_WRITE);//建立txt文件
		sprintf(write_str,"imu_data_num:%d",0);
		f_write(file,write_str,17,&bw);
		f_close(file);
	}
	return write_num;
}



void read_station()
{
	u8 read_file_station[700];
	uint16_t byteRead;
	f_open(file,"0:/Setting/station.csv",FA_OPEN_ALWAYS|FA_READ|FA_WRITE);//建立txt文件
	f_read(file,read_file_station,sizeof(read_file_station),(void *)&byteRead);
	f_close(file);
	
	u8 read_state = 0;//过程状态标志位  0 没有读到 1 读到了名字  2读到了经度lon 3读到了纬度lat
	int m;
	
	for(int i = 0;i < byteRead;i++)
	{
		if((read_file_station[i] == ',')&&(read_state == 0))
		{
			read_station_index[station_num].station_name_end = i-1;
			read_station_index[station_num].station_lon_start = i+1;
			read_state = 1;
		}
		else if((read_file_station[i] == ',')&&(read_state == 1))
		{
			read_station_index[station_num].station_lon_end = i-1;
			read_station_index[station_num].station_lat_start = i+1;
			read_state = 2;
		}
		else if((read_file_station[i] == '\r')&&(read_file_station[i+1] == '\n')&&(read_state == 2))
		{
			
			read_station_index[station_num].station_lat_end = i-1;
			read_station_index[station_num+1].station_name_start = i+2;
			station_num ++;
			read_state = 0;
		}
	}
	for(int i = 0;i < station_num;i ++)
	{
		m =0;
		for(int j=read_station_index[i].station_name_start;j<=read_station_index[i].station_name_end;j++)
		{
			station[i].name[m++] = read_file_station[j];
		}
		m =0;
		for(int j=read_station_index[i].station_lon_start;j<=read_station_index[i].station_lon_end;j++)
		{
			if(read_file_station[j] == '.')
			{
				for(int n = j-1;n >= read_station_index[i].station_lon_start;n--)
				{
					station[i].lon += (read_file_station[n]-48)*pow(10,j-1-n);
				}
				for(int n = j+1;n <= read_station_index[i].station_lon_end;n++)
				{
					station[i].lon += (read_file_station[n]-48)*pow(10,j-n);
				}
			}
		}
		m =0;
		for(int j=read_station_index[i].station_lat_start;j<=read_station_index[i].station_lat_end;j++)
		{
			if(read_file_station[j] == '.')
			{
				for(int n = j-1;n >= read_station_index[i].station_lat_start;n--)
				{
					station[i].lat += (read_file_station[n]-48)*pow(10,j-1-n);
				}
				for(int n = j+1;n <= read_station_index[i].station_lat_end;n++)
				{
					station[i].lat += (read_file_station[n]-48)*pow(10,j-n);
				}
			}
		}
	}

	unsigned int size=0;
	for(int i = 0;i< station_num;i++)
	{
		size += read_station_index[i].station_name_end - read_station_index[i].station_name_start+1;
	}

	// char *buff = (char *)malloc(size);
	list_buff = (char *)malloc(size);
	int index=0;
	for(int i = 0;i<station_num ; i++)
	{
		for(int j = 0;j<read_station_index[i].station_name_end - read_station_index[i].station_name_start+1;j++)
		{
			list_buff[index++] = station[i].name[j];
		}
		list_buff[index++] = '\n';
	}

}

void ble_start()
{


	u8 observer_start[15] = "AT+OBSERVER=1\r\n";
	u8 AT_mode[3] = "+++";
	u8 AT_EXIT[9] = "AT+EXIT\r\n";

	while(memcmp(s4_rx_buf,"DEVICE START\r\n",strlen("DEVICE START\r\n")));
	s4_tail = 0;
	for(int i=0;i<30;i++)
	{
		s4_rx_buf[i] = 0;
	}
	HAL_UART_Transmit(&UART4_Handler,AT_mode,3,1000);
	while(memcmp(s4_rx_buf,"OK\r\n",strlen("OK\r\n")));
	s4_tail = 0;
	for(int i=0;i<30;i++)
	{
		s4_rx_buf[i] = 0;
	}
	HAL_UART_Transmit(&UART4_Handler,observer_start,15,1000);
	while(memcmp(s4_rx_buf,"OK\r\n",strlen("OK\r\n")));
	s4_tail = 0;
	for(int i=0;i<30;i++)
	{
		s4_rx_buf[i] = 0;
	}
	HAL_UART_Transmit(&UART4_Handler,AT_EXIT,9,1000);
	ble_init_success = 1;
}








