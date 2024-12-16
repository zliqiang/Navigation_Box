/*
 * @Description: 
 * @Author: Liqiang Zhao
 * @E-Mail: zliqiang@nuaa.edu.cn
 * @Github: https://github.com/zliqiang
 * @Date: 2021-05-25 23:36:00
 * @LastEditors: Liqiang Zhao
 * @LastEditTime: 2021-05-26 21:58:30
 * @FilePath: \USERc:\Users\zliqiang\Desktop\实验42 FATFS实验\GUI_APP\main_win.h
 */
/**
 * @file demo.h
 *
 */

#ifndef MAIN_WIN_H
#define MAIN_WIN_H

#include "sys.h"

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#ifdef LV_CONF_INCLUDE_SIMPLE
#include "lvgl.h"
#include "lv_ex_conf.h"
#else
#include "lvgl.h"
#include "lv_ex_conf.h"
#endif

extern char *list_buff;
extern u8 sd_state;
extern u8 ble_init_success;
#define LV_USE_DEMO 1

#if LV_USE_DEMO

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create a demo application
 */
void main_win_create(void);

/**********************
 *      MACROS
 **********************/

#endif /*LV_USE_DEMO*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*DEMO_H*/
