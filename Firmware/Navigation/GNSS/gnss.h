/*
 * @Author: your name
 * @Date: 2021-04-12 20:40:27
 * @LastEditTime: 2021-05-27 15:22:05
 * @LastEditors: Liqiang Zhao
 * @Description: In User Settings Edit
 * @FilePath: \USERc:\Users\zliqiang\Desktop\实验42 FATFS实验\Navigation\GNSS\gnss.h
 */
#ifndef GNSS_H
#define GNSS_H
#include "delay.h"
#include "usart.h"

extern double longitude;
extern double latitude ;
extern float gnss_time;

typedef union
{
    unsigned char recv_class1;
    unsigned char class1;
}GNSS_CLASS;

typedef union
{
    unsigned char recv_id;
    unsigned char id;
}GNSS_ID;

typedef union
{
    unsigned char recv_length[2];
    unsigned short length;
}GNSS_LENGTH;

typedef union
{
    unsigned char recv_Tow[8];
    double Tow;
}GNSS_TOW;

typedef union
{
    unsigned char recv_week[2];
    unsigned short week;
}GNSS_WEEK;

typedef union
{
    unsigned char recv_leapS[1];
    char leapS;
}GNSS_LEAPS;

typedef union
{
    unsigned char recv_numMeas;
    unsigned char numMeas;
}GNSS_NUMMEAS;

typedef union
{
    unsigned char recv_recStat;
    unsigned char recStat;
}GNSS_RECSTAT;

typedef union
{
    unsigned char recv_version;
    unsigned char version;
}GNSS_VERSION;

typedef union
{
    unsigned char recv_reserved1[2];
    unsigned char reserved1[2];
}GNSS_RESERVED1;

typedef union
{
    unsigned char recv_prMes[8];
    double prMes;
}GNSS_PRMES;

typedef union
{
    unsigned char recv_cpMes[8];
    double cpMes;
}GNSS_CPMES;

typedef union
{
    unsigned char recv_doMes[4];
    float doMes;
}GNSS_DOMES;

typedef union
{
    unsigned char recv_gnssId;
    unsigned char gnssId;
}GNSS_GNSSID;

typedef union
{
    unsigned char recv_svId;
    unsigned char svId;
}GNSS_SVID;

typedef union
{
    unsigned char recv_sigId;
    unsigned char sigId;
}GNSS_SIGID;



typedef union
{
    unsigned char recv_freqId;
    unsigned char freqId;
}GNSS_FREQID;

typedef union
{
    unsigned char recv_locktime[2];
    unsigned short locktime;
}GNSS_LOCKTIME;

typedef union
{
    unsigned char recv_cno;
    unsigned char cno;
}GNSS_CNO;

typedef union
{
    unsigned char recv_prStdev;
    unsigned char prStdev;
}GNSS_PRSTDEV;

typedef union
{
    unsigned char recv_cpStdev;
    unsigned char cpStdev;
}GNSS_CPSTDEV;

typedef union
{
    unsigned char recv_doStdev;
    unsigned char doStdev;
}GNSS_DOSTDEV;

typedef union
{
    unsigned char recv_trkStat;
    unsigned char trkStat;
}GNSS_TRKSTAT;

typedef union
{
    unsigned char recv_reserved2;
    unsigned char reserved2;
}GNSS_RESERBED2;


//PVT数据共用体↓


typedef union
{
    unsigned char recv_length[2];
    unsigned short length;
}GNSS_PVT_LENGTH;

typedef union
{
    unsigned char recv_iTow[4];
    unsigned long iTow;
}GNSS_PVT_ITOW;

typedef union
{
    unsigned char recv_year[2];
    unsigned short year;
}GNSS_PVT_YEAR;

typedef union
{
    unsigned char recv_month;
    unsigned char month;
}GNSS_PVT_MONTH;

typedef union
{
    unsigned char recv_day;
    unsigned char day;
}GNSS_PVT_DAY;

typedef union
{
    unsigned char recv_hour;
    unsigned char hour;
}GNSS_PVT_HOUR;

typedef union
{
    unsigned char recv_min;
    unsigned char min;
}GNSS_PVT_MIN;

typedef union
{
    unsigned char recv_sec;
    unsigned char sec;
}GNSS_PVT_SEC;

typedef union
{
    unsigned char recv_valid;
    unsigned char valid;
}GNSS_PVT_VALID;

typedef union
{
    unsigned char recv_tAcc[4];
    unsigned long tAcc;
}GNSS_PVT_TACC;

typedef union
{
    unsigned char recv_nano[4];
    long nano;
}GNSS_PVT_NANO;

typedef union
{
    unsigned char recv_fixType;
    unsigned char fixType;
}GNSS_PVT_FIXTYPE;

typedef union
{
    unsigned char recv_flags;
    unsigned char flags;
}GNSS_PVT_FLAGS;

typedef union
{
    unsigned char recv_flags2;
    unsigned char flags2;
}GNSS_PVT_FLAGS2;

typedef union
{
    unsigned char recv_numSV;
    unsigned char numSV;
}GNSS_PVT_NUMSV;

typedef union
{
    unsigned char recv_longitude[4];
    long longitude;
}GNSS_PVT_LONGITUDE;

typedef union
{
    unsigned char recv_latitude[4];
    long latitude;
}GNSS_PVT_LATITUDE;

typedef union
{
    unsigned char recv_height[4];
    long height;
}GNSS_PVT_HEIGHT;

typedef union
{
    unsigned char recv_hMSL[4];
    long hMSL;
}GNSS_PVT_HMSL;

typedef union
{
    unsigned char recv_hAcc[4];
    unsigned long hAcc;
}GNSS_PVT_HACC;

typedef union
{
    unsigned char recv_vAcc[4];
    unsigned long vAcc;
}GNSS_PVT_VACC;

typedef union
{
    unsigned char recv_velN[4];
    long velN;
}GNSS_PVT_VELN;

typedef union
{
    unsigned char recv_velE[4];
    long velE;
}GNSS_PVT_VELE;

typedef union
{
    unsigned char recv_velD[4];
    long velD;
}GNSS_PVT_VELD;

typedef union
{
    unsigned char recv_gSpeed[4];
    long gSpeed;
}GNSS_PVT_GSPEED;

typedef union
{
    unsigned char recv_headMot[4];
    long headMot;
}GNSS_PVT_HEADMOT;

typedef union
{
    unsigned char recv_sAcc[4];
    unsigned long sAcc;
}GNSS_PVT_SACC;

typedef union
{
    unsigned char recv_headAcc[4];
    unsigned long headAcc;
}GNSS_PVT_HEADACC;

typedef union
{
    unsigned char recv_pDOP[2];
    unsigned short pDOP;
}GNSS_PVT_PDOP;

typedef union
{
    unsigned char recv_flags3;
    unsigned char flags3;
}GNSS_PVT_FLAGS3;

typedef union
{
    unsigned char recv_headVeh[4];
    long headVeh;
}GNSS_PVT_HEADVEH;

typedef union
{
    unsigned char recv_magDec[2];
    short magDec;
}GNSS_PVT_MAGDEC;

typedef union
{
    unsigned char recv_magAcc[2];
    unsigned short magAcc;
}GNSS_PVT_MAGACC;



//PVT数据共用体↑

extern GNSS_TOW gnss_Tow;
extern GNSS_WEEK gnss_week;
extern GNSS_LEAPS gnss_leapS;
extern GNSS_NUMMEAS gnss_numMeas;
extern GNSS_RECSTAT gnss_recStat;
extern GNSS_VERSION gnss_version;
extern GNSS_RESERVED1 gnss_reserved1;
extern GNSS_PRMES gnss_prMes[20];
extern GNSS_CPMES gnss_cpMes[20];
extern GNSS_DOMES gnss_doMes[20];
extern GNSS_GNSSID gnss_gnssId[20];
extern GNSS_SVID gnss_svId[20];
extern GNSS_SIGID gnss_sigId[20];
extern GNSS_FREQID gnss_freqId[20];
extern GNSS_LOCKTIME gnss_locktime[20];
extern GNSS_CNO gnss_cno[20];
extern GNSS_PRSTDEV gnss_prStdev[20];
extern GNSS_CPSTDEV gnss_cpStdev[20];
extern GNSS_DOSTDEV gnss_doStdev[20];
extern GNSS_TRKSTAT gnss_trkStat[20];
extern GNSS_RESERBED2 gnss_reseved2[20];

extern GNSS_PVT_LENGTH gnss_pvt_length;
extern GNSS_PVT_ITOW   gnss_pvt_iTow;
extern GNSS_PVT_YEAR   gnss_pvt_year;
extern GNSS_PVT_MONTH  gnss_pvt_month;
extern GNSS_PVT_DAY    gnss_pvt_day;
extern GNSS_PVT_HOUR   gnss_pvt_hour;
extern GNSS_PVT_MIN    gnss_pvt_min;
extern GNSS_PVT_SEC    gnss_pvt_sec;
extern GNSS_PVT_VALID  gnss_pvt_valid;
extern GNSS_PVT_TACC   gnss_pvt_tAcc;
extern GNSS_PVT_NANO   gnss_pvt_nano;
extern GNSS_PVT_FIXTYPE gnss_pvt_fixType;
extern GNSS_PVT_FLAGS  gnss_pvt_flags;
extern GNSS_PVT_FLAGS2 gnss_pvt_flags2;
extern GNSS_PVT_NUMSV  gnss_pvt_numSV;
extern GNSS_PVT_LONGITUDE	gnss_pvt_longitude;
extern GNSS_PVT_LATITUDE   gnss_pvt_latitude;
extern GNSS_PVT_HEIGHT     gnss_pvt_height;
extern GNSS_PVT_HMSL       gnss_pvt_hMSL;
extern GNSS_PVT_HACC       gnss_pvt_hAcc;
extern GNSS_PVT_VACC       gnss_pvt_vAcc;
extern GNSS_PVT_VELN       gnss_pvt_velN;
extern GNSS_PVT_VELE       gnss_pvt_velE;
extern GNSS_PVT_VELD       gnss_pvt_velD;
extern GNSS_PVT_GSPEED     gnss_pvt_gSpeed;
extern GNSS_PVT_HEADMOT    gnss_pvt_headMot;
extern GNSS_PVT_SACC       gnss_pvt_sAcc;
extern GNSS_PVT_HEADACC    gnss_pvt_headAcc;
extern GNSS_PVT_PDOP       gnss_pvt_pDOP;
extern GNSS_PVT_FLAGS3     gnss_pvt_flags3;
extern GNSS_PVT_HEADVEH    gnss_pvt_headVeh;
extern GNSS_PVT_MAGDEC     gnss_pvt_magDec;
extern GNSS_PVT_MAGACC     gnss_pvt_magAcc;

void read_gnss();

#endif
