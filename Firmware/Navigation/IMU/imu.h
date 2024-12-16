/*
 * @Description: 
 * @Author: Liqiang Zhao
 * @E-Mail: zliqiang@nuaa.edu.cn
 * @Github: https://github.com/zliqiang
 * @Date: 2021-04-23 16:18:31
 * @LastEditors: Liqiang Zhao
 * @LastEditTime: 2021-06-03 10:11:51
 * @FilePath: \USERc:\Users\zliqiang\Desktop\实验42 FATFS硬件基本可以了啊啊啊啊\实验42 FATFS实验\Navigation\IMU\imu.h
 */
/*
 *                        _oo0oo_
 *                       o8888888o
 *                       88" . "88
 *                       (| -_- |)
 *                       0\  =  /0
 *                     ___/`---'\___
 *                   .' \\|     |// '.
 *                  / \\|||  :  |||// \
 *                 / _||||| -:- |||||- \
 *                |   | \\\  - /// |   |
 *                | \_|  ''\---/''  |_/ |
 *                \  .-\__  '-'  ___/-. /
 *              ___'. .'  /--.--\  `. .'___
 *           ."" '<  `.___\_<|>_/___.' >' "".
 *          | | :  `- \`.;`\ _ /`;.`/ - ` : | |
 *          \  \ `_.   \_ __\ /__ _/   .-` /  /
 *      =====`-.____`.___ \_____/___.-`___.-'=====
 *                        `=---='
 * 
 * 
 *      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 *            佛祖保佑       永不宕机     永无BUG
 */



#ifndef IMU_H
#define IMU_H
#include "sys.h"
#include "usart.h"

extern float imu_time_temp ,imu_time_test;

struct STATION
{
	char *name;
	double lon;
	double lat;
};

struct READ_STATION_INDEX
{
	u8 station_name_start;
	u8 station_name_end;
	u8 station_lon_start;
	u8 station_lon_end;
	u8 station_lat_start;
	u8 station_lat_end;
};

extern struct STATION station[20];
extern struct READ_STATION_INDEX read_station_index[10];
extern u8 station_num;
extern float imu_time_s;
extern  void read_imu_old(void); //读取IMU数据到已定义好的共用体中 / Read IMU data into the defined union
extern unsigned int write_flag;

typedef union 
{
    u8 recv_check[2];
    u16 check;
}IMU_CHEACK;

typedef union 
{
	/* data */
	u8 recv_id[2];
	u16 id;
}IMU_ID;


typedef union 
{
	u8 recv_length[2];
	u16 length;
}IMU_LENGTH;


typedef union  
{
	/* data */
	u8 recv_time[4];
	uint32_t time;
}IMU_TIME;

typedef union  
{
	/* data */
	u8 recv_pitch[4];
	float pitch;
}IMU_PITCH;

typedef union  
{
	/* data */
	u8 recv_roll[4];
	float roll;
}IMU_ROLL;

typedef union  
{
	/* data */
	u8 recv_yaw[4];
	float yaw;
}IMU_YAW;

typedef union  
{
	/* data */
	u8 recv_ax[4];
	float ax;
}IMU_AX;

typedef union  
{
	/* data */
	u8 recv_ay[4];
	float ay;
}IMU_AY;

typedef union  
{
	/* data */
	u8 recv_az[4];
	float az;
}IMU_AZ;

typedef union  
{
	/* data */
	u8 recv_gx[4];
	float gx;
}IMU_GX;

typedef union  
{
	/* data */
	u8 recv_gy[4];
	float gy;
}IMU_GY;

typedef union  
{
	/* data */
	u8 recv_gz[4];
	float gz;
}IMU_GZ;

typedef union 
{
	u8 recv_temp[4];
	float temp;
}IMU_TEMP;

 
typedef union 
{
    /* data */
    u8 recv_crc[4];
    uint32_t crc;
}IMU_CRC;

extern IMU_CHEACK imu_check;
extern IMU_ID imu_id;
extern IMU_LENGTH imu_length;
extern IMU_TIME imu_time;
extern IMU_PITCH imu_pitch;
extern IMU_ROLL imu_roll;
extern IMU_YAW imu_yaw;
extern IMU_AX imu_ax;
extern IMU_AY imu_ay;
extern IMU_AZ imu_az;
extern IMU_GX imu_gx;
extern IMU_GY imu_gy;
extern IMU_GZ imu_gz;
extern IMU_TEMP imu_temp;
extern IMU_CRC imu_crc;
#define BUFF_SIZE 50

extern float ax_write[BUFF_SIZE],ay_write[BUFF_SIZE],az_write[BUFF_SIZE],gx_write[BUFF_SIZE],gy_write[BUFF_SIZE],gz_write[BUFF_SIZE],imu_time_write[BUFF_SIZE];
extern volatile int imu_rhead,imu_rtail,imu_whead,imu_wtail;
extern int imu_count;
extern u8 s1_buff_debug ;
extern u8 imu_recv_buf[S1_RX_SIZE+2];
void read_imu(void);
void imu_reset();
#endif

