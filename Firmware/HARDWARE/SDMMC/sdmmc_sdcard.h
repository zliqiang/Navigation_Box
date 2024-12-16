/*
 * @Author: your name
 * @Date: 2021-03-23 16:15:13
 * @LastEditTime: 2021-06-01 10:52:08
 * @LastEditors: Liqiang Zhao
 * @Description: In User Settings Edit
 * @FilePath: \USERc:\Users\zliqiang\Desktop\ʵ��42 FATFSӲ�����������˰�������\ʵ��42 FATFSʵ��\HARDWARE\SDMMC\sdmmc_sdcard.h
 */
#ifndef _SDMMC_SDCARD_H
#define _SDMMC_SDCARD_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F7������
//SD����������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/12/15
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

#define MSD_OK                        ((uint8_t)0x00)
#define MSD_ERROR                     ((uint8_t)0x01)

#define SD_TIMEOUT      ((uint32_t)100000000)       //��ʱʱ��
#define SD_DMA_MODE     1	                        //1��DMAģʽ��0����ѯģʽ   

extern SD_HandleTypeDef        SDCARD_Handler;      //SD�����
extern HAL_SD_CardInfoTypedef  SDCardInfo;          //SD����Ϣ�ṹ��

u8 SD_Init(void);
u8 SD_GetCardInfo(HAL_SD_CardInfoTypedef *cardinfo);
u8 SD_ReadDisk(u8* buf,u32 sector,u8 cnt);
u8 SD_WriteDisk(u8 *buf,u32 sector,u8 cnt);
u8 SD_ReadBlocks_DMA(uint32_t *buf,uint64_t sector,uint32_t blocksize,uint32_t cnt);
u8 SD_WriteBlocks_DMA(uint32_t *buf,uint64_t sector,uint32_t blocksize,uint32_t cnt);
#endif
