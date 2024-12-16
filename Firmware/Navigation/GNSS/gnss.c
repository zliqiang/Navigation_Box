/*
 * @Author: your name
 * @Date: 2021-04-12 20:40:27
 * @LastEditTime: 2021-05-27 15:21:50
 * @LastEditors: Liqiang Zhao
 * @Description: In User Settings Edit
 * @FilePath: \USERc:\Users\zliqiang\Desktop\实验42 FATFS实验\Navigation\GNSS\gnss.c
 */
#include "gnss.h"
#include "math.h"

double longitude = 0,latitude = 0;
float gnss_time = 0;

GNSS_LENGTH gnss_length;
GNSS_TOW gnss_Tow;
GNSS_WEEK gnss_week;
GNSS_LEAPS gnss_leapS;
GNSS_NUMMEAS gnss_numMeas;
GNSS_RECSTAT gnss_recStat;
GNSS_VERSION gnss_version;
GNSS_RESERVED1 gnss_reserved1;
GNSS_PRMES gnss_prMes[20];
GNSS_CPMES gnss_cpMes[20];
GNSS_DOMES gnss_doMes[20];
GNSS_GNSSID gnss_gnssId[20];
GNSS_SVID gnss_svId[20];
GNSS_SIGID gnss_sigId[20];
GNSS_FREQID gnss_freqId[20];
GNSS_LOCKTIME gnss_locktime[20];
GNSS_CNO gnss_cno[20];
GNSS_PRSTDEV gnss_prStdev[20];
GNSS_CPSTDEV gnss_cpStdev[20];
GNSS_DOSTDEV gnss_doStdev[20];
GNSS_TRKSTAT gnss_trkStat[20];
GNSS_RESERBED2 gnss_reseved2[20];


//PVT数据共用体定义

GNSS_PVT_LENGTH gnss_pvt_length;
GNSS_PVT_ITOW   gnss_pvt_iTow;
GNSS_PVT_YEAR   gnss_pvt_year;
GNSS_PVT_MONTH  gnss_pvt_month;
GNSS_PVT_DAY    gnss_pvt_day;
GNSS_PVT_HOUR   gnss_pvt_hour;
GNSS_PVT_MIN    gnss_pvt_min;
GNSS_PVT_SEC    gnss_pvt_sec;
GNSS_PVT_VALID  gnss_pvt_valid;
GNSS_PVT_TACC   gnss_pvt_tAcc;
GNSS_PVT_NANO   gnss_pvt_nano;
GNSS_PVT_FIXTYPE gnss_pvt_fixType;
GNSS_PVT_FLAGS  gnss_pvt_flags;
GNSS_PVT_FLAGS2 gnss_pvt_flags2;
GNSS_PVT_NUMSV  gnss_pvt_numSV;
GNSS_PVT_LONGITUDE gnss_pvt_longitude;
GNSS_PVT_LATITUDE   gnss_pvt_latitude;
GNSS_PVT_HEIGHT     gnss_pvt_height;
GNSS_PVT_HMSL       gnss_pvt_hMSL;
GNSS_PVT_HACC       gnss_pvt_hAcc;
GNSS_PVT_VACC       gnss_pvt_vAcc;
GNSS_PVT_VELN       gnss_pvt_velN;
GNSS_PVT_VELE       gnss_pvt_velE;
GNSS_PVT_VELD       gnss_pvt_velD;
GNSS_PVT_GSPEED     gnss_pvt_gSpeed;
GNSS_PVT_HEADMOT    gnss_pvt_headMot;
GNSS_PVT_SACC       gnss_pvt_sAcc;
GNSS_PVT_HEADACC    gnss_pvt_headAcc;
GNSS_PVT_PDOP       gnss_pvt_pDOP;
GNSS_PVT_FLAGS3     gnss_pvt_flags3;
GNSS_PVT_HEADVEH    gnss_pvt_headVeh;
GNSS_PVT_MAGDEC     gnss_pvt_magDec;
GNSS_PVT_MAGACC     gnss_pvt_magAcc;

//PVT数据共用体定义



void read_gnss()
{
	
	u8 gnss_get_head = 0;
	u8 gnss_get_rawx = 0;
	u8 gnss_get_pvt = 0;
	
	unsigned char numMeas = 0;
	double lat1,lon1;
	u8 test1;
	u8 get_rawx_over = 0,get_pvt_over = 0;
	
	
		s5_recv_over = 0;
		
		//HAL_UART_Transmit(&UART2_Handler,gnss_recv_buff,S5_RX_SIZE,1000);
		
		for(int i = 0;i < S5_RX_SIZE; i ++)
		{
			if((s5_rx_buf[i] == 0xB5)&&(s5_rx_buf[i+1] == 0x62))
			{
				gnss_get_head = 1;
				if((s5_rx_buf[i+2] == 0x02)&&(s5_rx_buf[i+3] == 0x15))
				{
					gnss_get_rawx = 1;
				}
				if((s5_rx_buf[i+2] == 0x01)&&(s5_rx_buf[i+3] == 0x07))
				{
					gnss_get_pvt = 1;
				}
			}
			if(gnss_get_rawx == 1)
			{
				for(int j = 0;j < 2;j++)
            	{
								if(i+j+4 < S5_RX_SIZE)
               		 gnss_length.recv_length[j] = s5_rx_buf[i+j+4];
								else 
									gnss_length.recv_length[j] = s5_rx_buf[i+j+4-S5_RX_SIZE];
            	}
				numMeas = (gnss_length.length - 16)/32;
				for(int j = 0;j < 8;j++)
            	{
								if(i+j+6 < S5_RX_SIZE)
               	 	gnss_Tow.recv_Tow[j] = s5_rx_buf[i+j+6];
								else
					gnss_Tow.recv_Tow[j] = s5_rx_buf[i+j+6-S5_RX_SIZE];
            	}
            	for(int j = 0;j < 2;j++)
            	{
								if(i+j+8+6 < S5_RX_SIZE)
                	gnss_week.recv_week[j] = s5_rx_buf[i+j+8+6];
								else
					gnss_week.recv_week[j] = s5_rx_buf[i+j+8+6-S5_RX_SIZE];
            	}
							if(i+10+6 < S5_RX_SIZE)
            	gnss_leapS.recv_leapS[0] = s5_rx_buf[i+10+6];
							else
					gnss_leapS.recv_leapS[0] = s5_rx_buf[i+10+6-S5_RX_SIZE];
							if(i+11+6 < S5_RX_SIZE)
            	gnss_numMeas.recv_numMeas = s5_rx_buf[i+11+6];
							else
					gnss_numMeas.recv_numMeas = s5_rx_buf[i+11+6-S5_RX_SIZE];
				if(gnss_numMeas.numMeas == numMeas)
            	{
                	
            	}
							if(i+12+6 < S5_RX_SIZE)
				gnss_recStat.recv_recStat = s5_rx_buf[i+12+6];
							else
					gnss_recStat.recv_recStat = s5_rx_buf[i+12+6-S5_RX_SIZE];
							if(i+13+6 < S5_RX_SIZE)
            	gnss_version.recv_version = s5_rx_buf[i+13+6];
							else
					gnss_version.recv_version = s5_rx_buf[i+13+6-S5_RX_SIZE];
				for(int j = 0;j <2;j++)
				{
					if(i+j+14+6 < S5_RX_SIZE)
					gnss_reserved1.recv_reserved1[j] = s5_rx_buf[i+j+14+6];
					else
					gnss_reserved1.recv_reserved1[j] = s5_rx_buf[i+j+14+6-S5_RX_SIZE];
				}

				for(int k = 0;k< gnss_numMeas.numMeas ;k++)
				{
					for(int j=0;j<8;j++)
					{
						if(i+j+16+32*k+6 < S5_RX_SIZE)
						gnss_prMes[k].recv_prMes[j] = s5_rx_buf[i+j+16+32*k+6];
						else
					gnss_prMes[k].recv_prMes[j] = s5_rx_buf[i+j+16+32*k+6-S5_RX_SIZE];
					}
					for(int j=0;j<8;j++)
					{
						if(i+j+24+32*k+6 < S5_RX_SIZE)
						gnss_cpMes[k].recv_cpMes[j] = s5_rx_buf[i+j+24+32*k+6];
						else
					gnss_cpMes[k].recv_cpMes[j] = s5_rx_buf[i+j+24+32*k+6-S5_RX_SIZE];
					}
					for(int j=0;j<4;j++)
					{
						if(i+j+32+32*k+6 < S5_RX_SIZE)
						gnss_doMes[k].recv_doMes[j] = s5_rx_buf[i+j+32+32*k+6];
						else
					gnss_doMes[k].recv_doMes[j] = s5_rx_buf[i+j+32+32*k+6-S5_RX_SIZE];
					}
					if(i+36+32*k+6 < S5_RX_SIZE)
					gnss_gnssId[k].recv_gnssId = s5_rx_buf[i+36+32*k+6];
					else
					gnss_gnssId[k].recv_gnssId = s5_rx_buf[i+36+32*k+6-S5_RX_SIZE];
					if(i+37+32*k+6 < S5_RX_SIZE)
					gnss_svId[k].recv_svId = s5_rx_buf[i+37+32*k+6];
					else
					gnss_svId[k].recv_svId = s5_rx_buf[i+37+32*k+6-S5_RX_SIZE];
					if(i+38+32*k+6 < S5_RX_SIZE)
					gnss_sigId[k].recv_sigId = s5_rx_buf[i+38+32*k+6];
					else
					gnss_sigId[k].recv_sigId = s5_rx_buf[i+38+32*k+6-S5_RX_SIZE];
					if(i+39+32*k+6 < S5_RX_SIZE)
					gnss_freqId[k].recv_freqId = s5_rx_buf[i+39+32*k+6];
					else
					gnss_freqId[k].recv_freqId = s5_rx_buf[i+39+32*k+6-S5_RX_SIZE];
					for(int j = 0;j<2;j++)
					{
						if(i+j+40+32*k+6 < S5_RX_SIZE)
						gnss_locktime[k].recv_locktime[j] = s5_rx_buf[i+j+40+32*k+6];
						else
					gnss_locktime[k].recv_locktime[j] = s5_rx_buf[i+j+40+32*k+6-S5_RX_SIZE];
					}
					if(i+42+32*k+6 < S5_RX_SIZE)
					gnss_cno[k].recv_cno = s5_rx_buf[i+42+32*k+6];
					else
					gnss_cno[k].recv_cno = s5_rx_buf[i+42+32*k+6-S5_RX_SIZE];
					if(i+43+32*k+6 < S5_RX_SIZE)
					gnss_prStdev[k].recv_prStdev = s5_rx_buf[i+43+32*k+6];
					else
					gnss_prStdev[k].recv_prStdev = s5_rx_buf[i+43+32*k+6-S5_RX_SIZE];
					if(i+44+32*k+6 < S5_RX_SIZE)
					gnss_cpStdev[k].recv_cpStdev = s5_rx_buf[i+44+32*k+6];
					else
					gnss_cpStdev[k].recv_cpStdev = s5_rx_buf[i+44+32*k+6-S5_RX_SIZE];
					if(i+45+32*k+6 < S5_RX_SIZE)
					gnss_doStdev[k].recv_doStdev = s5_rx_buf[i+45+32*k+6];
					else
					gnss_doStdev[k].recv_doStdev = s5_rx_buf[i+45+32*k+6-S5_RX_SIZE];
					if(i+46+32*k+6 < S5_RX_SIZE)
					gnss_trkStat[k].recv_trkStat = s5_rx_buf[i+46+32*k+6];
					else
					gnss_trkStat[k].recv_trkStat = s5_rx_buf[i+46+32*k+6-S5_RX_SIZE];
					if(i+47+32*k+6 < S5_RX_SIZE)
					gnss_reseved2[k].recv_reserved2 = s5_rx_buf[i+47+32*k+6];
					else
					gnss_reseved2[k].recv_reserved2 = s5_rx_buf[i+47+32*k+6-S5_RX_SIZE];
				}
				// sprintf(tx_buf2,"carrier:%lf",gnss_cpMes[0].cpMes);
				// HAL_UART_Transmit(&UART2_Handler,tx_buf2,25,1000);
				i = i+40;
				gnss_get_rawx = 0;
				get_rawx_over = 1;
				//break;
        	}
			if(gnss_get_pvt == 1) // 先接收到
			{
				for(int j=0;j<2;j++)
				{
					if(i+j+4 < S5_RX_SIZE)
					gnss_pvt_length.recv_length[j] = s5_rx_buf[i+j+4];
					else
					gnss_pvt_length.recv_length[j] = s5_rx_buf[i+j+4-S5_RX_SIZE];
				}
				for(int j = 0;j<4;j++)
				{
					if(i+j+0+6 < S5_RX_SIZE)
					gnss_pvt_iTow.recv_iTow[j] = s5_rx_buf[i+j+0+6];
					else
					gnss_pvt_iTow.recv_iTow[j] = s5_rx_buf[i+j+0+6-S5_RX_SIZE];
				}
				for(int j = 0;j<2;j++)
				{
					if(i+j+4+6 < S5_RX_SIZE)
					gnss_pvt_year.recv_year[j] = s5_rx_buf[i+j+4+6];
					else
					gnss_pvt_year.recv_year[j] = s5_rx_buf[i+j+4+6-S5_RX_SIZE];
				}
				if(i+6+6 < S5_RX_SIZE)
				gnss_pvt_month.recv_month = s5_rx_buf[i+6+6];
				else
					gnss_pvt_month.recv_month = s5_rx_buf[i+6+6-S5_RX_SIZE];
				if(i+7+6 < S5_RX_SIZE)
				gnss_pvt_day.recv_day = s5_rx_buf[i+7+6];
				else
					gnss_pvt_day.recv_day = s5_rx_buf[i+7+6-S5_RX_SIZE];
				if(i+8+6 < S5_RX_SIZE)
				gnss_pvt_hour.recv_hour = s5_rx_buf[i+8+6];
				else
					gnss_pvt_hour.recv_hour = s5_rx_buf[i+8+6-S5_RX_SIZE];
				if(i+9+6 < S5_RX_SIZE)
				gnss_pvt_min.recv_min = s5_rx_buf[i+9+6];
				else
					gnss_pvt_min.recv_min = s5_rx_buf[i+9+6-S5_RX_SIZE];
				if(i+10+6 < S5_RX_SIZE)
				gnss_pvt_sec.recv_sec = s5_rx_buf[i+10+6];
				else
					gnss_pvt_sec.recv_sec = s5_rx_buf[i+10+6-S5_RX_SIZE];
				
				for(int j = 0;j<4;j++)
				{
					if(i+j+12+6 < S5_RX_SIZE)
					gnss_pvt_tAcc.recv_tAcc[j] = s5_rx_buf[i+j+12+6];
					else
					gnss_pvt_tAcc.recv_tAcc[j] = s5_rx_buf[i+j+12+6-S5_RX_SIZE];
				}
				for(int j = 0;j<4;j++)
				{
					if(i+j+16+6 < S5_RX_SIZE)
					gnss_pvt_nano.recv_nano[j] = s5_rx_buf[i+j+16+6];
					else
					gnss_pvt_nano.recv_nano[j] = s5_rx_buf[i+j+16+6-S5_RX_SIZE];
				}
				if(i+20+6 < S5_RX_SIZE)
				gnss_pvt_fixType.recv_fixType = s5_rx_buf[i+20+6];
				else
					gnss_pvt_fixType.recv_fixType = s5_rx_buf[i+20+6-S5_RX_SIZE];
				if(i+21+6 < S5_RX_SIZE)
				gnss_pvt_flags.recv_flags = s5_rx_buf[i+21+6];
				else
					gnss_pvt_flags.recv_flags = s5_rx_buf[i+21+6-S5_RX_SIZE];
				if(i+22+6 < S5_RX_SIZE)
				gnss_pvt_flags2.recv_flags2 = s5_rx_buf[i+22+6];
				else
					gnss_pvt_flags2.recv_flags2 = s5_rx_buf[i+22+6-S5_RX_SIZE];
				if(i+23+6 < S5_RX_SIZE)
				gnss_pvt_numSV.recv_numSV = s5_rx_buf[i+23+6];
				else
					gnss_pvt_numSV.recv_numSV = s5_rx_buf[i+23+6-S5_RX_SIZE];
				for(int j = 0;j<4;j++)
				{
					if(i+j+24+6 < S5_RX_SIZE)
					gnss_pvt_longitude.recv_longitude[j] = s5_rx_buf[i+j+24+6];
					else
					gnss_pvt_longitude.recv_longitude[j] = s5_rx_buf[i+j+24+6-S5_RX_SIZE];
				}
				for(int j = 0;j<4;j++)
				{
					if(i+j+28+6 < S5_RX_SIZE)
					gnss_pvt_latitude.recv_latitude[j] = s5_rx_buf[i+j+28+6];
					else
					gnss_pvt_latitude.recv_latitude[j] = s5_rx_buf[i+j+28+6-S5_RX_SIZE];
				}
				for(int j = 0;j<4;j++)
				{
					if(i+j+32+6 < S5_RX_SIZE)
					gnss_pvt_height.recv_height[j] = s5_rx_buf[i+j+32+6];
					else
					gnss_pvt_height.recv_height[j] = s5_rx_buf[i+j+32+6-S5_RX_SIZE];
				}
				for(int j = 0;j<4;j++)
				{
					if(i+j+36+6 < S5_RX_SIZE)
					gnss_pvt_hMSL.recv_hMSL[j] = s5_rx_buf[i+j+36+6];
					else
					gnss_pvt_hMSL.recv_hMSL[j] = s5_rx_buf[i+j+36+6-S5_RX_SIZE];
				}
				for(int j = 0;j<4;j++)
				{
					if(i+j+40+6 < S5_RX_SIZE)
					gnss_pvt_hAcc.recv_hAcc[j] = s5_rx_buf[i+j+40+6];
					else
					gnss_pvt_hAcc.recv_hAcc[j] = s5_rx_buf[i+j+40+6-S5_RX_SIZE];
				}
				for(int j = 0;j<4;j++)
				{
					if(i+j+44+6 < S5_RX_SIZE)
					gnss_pvt_vAcc.recv_vAcc[j] = s5_rx_buf[i+j+44+6];
					else
					gnss_pvt_vAcc.recv_vAcc[j] = s5_rx_buf[i+j+44+6-S5_RX_SIZE];
				}
				for(int j = 0;j<4;j++)
				{
					if(i+j+48+6 < S5_RX_SIZE)
					gnss_pvt_velN.recv_velN[j] = s5_rx_buf[i+j+48+6];
					else
					gnss_pvt_velN.recv_velN[j] = s5_rx_buf[i+j+48+6-S5_RX_SIZE];
				}
				for(int j = 0;j<4;j++)
				{
					if(i+j+52+6 < S5_RX_SIZE)
					gnss_pvt_velE.recv_velE[j] = s5_rx_buf[i+j+52+6];
					else
					gnss_pvt_velE.recv_velE[j] = s5_rx_buf[i+j+52+6-S5_RX_SIZE];
				}
				for(int j = 0;j<4;j++)
				{
					if(i+j+56+6 < S5_RX_SIZE)
					gnss_pvt_velD.recv_velD[j] = s5_rx_buf[i+j+56+6];
					else
					gnss_pvt_velD.recv_velD[j] = s5_rx_buf[i+j+56+6-S5_RX_SIZE];
				}
				for(int j = 0;j<4;j++)
				{
					if(i+j+60+6 < S5_RX_SIZE)
					gnss_pvt_gSpeed.recv_gSpeed[j] = s5_rx_buf[i+j+60+6];
					else
					gnss_pvt_gSpeed.recv_gSpeed[j] = s5_rx_buf[i+j+60+6-S5_RX_SIZE];
				}
				for(int j = 0;j<4;j++)
				{
					if(i+j+64+6 < S5_RX_SIZE)
					gnss_pvt_headMot.recv_headMot[j] = s5_rx_buf[i+j+64+6];
					else
					gnss_pvt_headMot.recv_headMot[j] = s5_rx_buf[i+j+64+6-S5_RX_SIZE];
				}
				for(int j = 0;j<4;j++)
				{
					if(i+j+68+6 < S5_RX_SIZE)
					gnss_pvt_sAcc.recv_sAcc[j] = s5_rx_buf[i+j+68+6];
					else
					gnss_pvt_sAcc.recv_sAcc[j] = s5_rx_buf[i+j+68+6-S5_RX_SIZE];
				}
				for(int j = 0;j<4;j++)
				{
					if(i+j+72+6 < S5_RX_SIZE)
					gnss_pvt_headAcc.recv_headAcc[j] = s5_rx_buf[i+j+72+6];
					else
					gnss_pvt_headAcc.recv_headAcc[j] = s5_rx_buf[i+j+72+6-S5_RX_SIZE];
				}
				for(int j = 0;j<2;j++)
				{
					if(i+j+76+6 < S5_RX_SIZE)
					gnss_pvt_pDOP.recv_pDOP[j] = s5_rx_buf[i+j+76+6];
					else
					gnss_pvt_pDOP.recv_pDOP[j] = s5_rx_buf[i+j+76+6-S5_RX_SIZE];
				}
				if(i+78+6 < S5_RX_SIZE)
				gnss_pvt_flags3.recv_flags3 = s5_rx_buf[i+78+6];
				else
					gnss_pvt_flags3.recv_flags3 = s5_rx_buf[i+78+6-S5_RX_SIZE];
				for(int j = 0;j<4;j++)
				{
					if(i+j+84+6 < S5_RX_SIZE)
					gnss_pvt_headVeh.recv_headVeh[j] = s5_rx_buf[i+j+84+6];
					else
					gnss_pvt_headVeh.recv_headVeh[j] = s5_rx_buf[i+j+84+6-S5_RX_SIZE];
				}
				for(int j = 0;j<2;j++)
				{
					if(i+j+88+6 < S5_RX_SIZE)
					gnss_pvt_magDec.recv_magDec[j] = s5_rx_buf[i+j+88+6];
					else
					gnss_pvt_magDec.recv_magDec[j] = s5_rx_buf[i+j+88+6-S5_RX_SIZE];
				}
				for(int j = 0;j<2;j++)
				{
					if(i+j+90+6 < S5_RX_SIZE)
					gnss_pvt_magAcc.recv_magAcc[j] = s5_rx_buf[i+j+90+6];
					else
					gnss_pvt_magAcc.recv_magAcc[j] = s5_rx_buf[i+j+90+6-S5_RX_SIZE];
				}
				i = i+90;
				longitude = gnss_pvt_longitude.longitude*pow(10,-7);
				latitude  = gnss_pvt_latitude.latitude*pow(10,-7);
				// sprintf(tx_buf,"longitude:%d\n",gnss_pvt_longitude.longitude);
				// HAL_UART_Transmit(&UART2_Handler,tx_buf,20,1000);
				get_pvt_over = 1;
				gnss_get_pvt = 0;
				
			}
			if((get_pvt_over == 1)&&(get_rawx_over == 1))
			{
				get_rawx_over = 0;
				get_pvt_over = 0;
				break;
			}
			

    	}

		// for(int i = 0;i < S5_RX_SIZE; i++)
		// {
		// 	gnss_recv_buff[i] = 0;
		// 	s5_rx_buf[i] = 0;
		// }
	
}