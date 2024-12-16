/*
 * @Description: 
 * @Author: Liqiang Zhao
 * @E-Mail: zliqiang@nuaa.edu.cn
 * @Github: https://github.com/zliqiang
 * @Date: 2021-04-29 16:28:39
 * @LastEditors: Liqiang Zhao
 * @LastEditTime: 2021-04-29 16:41:47
 * @FilePath: \HK_METRO1.0\HARDWARE\CALCULATE\calculate.h
 */
#ifndef CALCULATE_H
#define CALCULATE_H

#define IMU_ONLY  0
#define IMU_GNSS  1
#define IMU_PRESS 2  

#include "sys.h"
#include "gnss.h"
#include "imu.h"

extern u8 pos_cal_flag;
void gnss_imu_cal();

#endif