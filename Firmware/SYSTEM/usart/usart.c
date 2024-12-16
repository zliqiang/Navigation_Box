#include "usart.h"
#include "imu.h"
////////////////////////////////////////////////////////////////////////////////// 	 

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F7������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2015/6/23
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.0�޸�˵�� 
////////////////////////////////////////////////////////////////////////////////// 	  
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)	
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                   
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->ISR&0X40)==0);//ѭ������,ֱ���������  
	USART1->TDR=(u8)ch;      
	return ch;
}
#endif 




u8 te;

float pressure = 0;
float height = 0;


u8 s1_rx_buf[S1_RX_SIZE];
int s1_tail=0;
u8 s1_recv_over;

u8 s6_rx_buf[S6_RX_SIZE];
int s6_tail=0;
u8 s6_recv_over;
int s6_recv_length;

u8 s5_rx_buf[S5_RX_SIZE];
int s5_tail=0;
u8 s5_recv_over;
int s5_recv_length;

u8 s4_rx_buf[S4_RX_SIZE];
int s4_tail=0;
u8 s4_recv_over;

u8 error_handle = 0;

//USART1	IMU
//USART2	����
//USART3	4Gģ��
//UART5		GPS
//USART6	��ѹ������
//UART7		����ģ��

//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   
u8 USART1_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u8 USART2_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u8 USART3_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u8 UART4_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u8 UART5_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u8 USART6_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u8 UART7_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART1_RX_STA=0;       //����״̬���	
u16 USART2_RX_STA=0;       //����״̬���	
u16 USART3_RX_STA=0;      //����״̬���	
u16 UART4_RX_STA=0;       //����״̬���	
u16 UART5_RX_STA=0;       //����״̬���		
u16 USART6_RX_STA=0;       //����״̬���	
u16 UART7_RX_STA=0;       //����״̬���		


u8 aRxBuffer[RXBUFFERSIZE];//HAL��ʹ�õĴ��ڽ��ջ���
UART_HandleTypeDef UART1_Handler; //UART���
UART_HandleTypeDef UART2_Handler; //UART���
UART_HandleTypeDef UART3_Handler; //UART���
UART_HandleTypeDef UART4_Handler; //UART���
UART_HandleTypeDef UART5_Handler; //UART���
UART_HandleTypeDef UART6_Handler; //UART���
UART_HandleTypeDef UART7_Handler;

//��ʼ��IO ����1 
//bound:������
void uart_init(u32 bound)
{	
	//UART ?????
	UART1_Handler.Instance=USART1;					    //USART1
	UART1_Handler.Init.BaudRate=bound;				    //������
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&UART1_Handler);					    //HAL_UART_Init()��ʹ��UART1

	UART2_Handler.Instance=USART2;					    
	UART2_Handler.Init.BaudRate=bound;				    
	UART2_Handler.Init.WordLength=UART_WORDLENGTH_8B;   
	UART2_Handler.Init.StopBits=UART_STOPBITS_1;	    
	UART2_Handler.Init.Parity=UART_PARITY_NONE;		    
	UART2_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   
	UART2_Handler.Init.Mode=UART_MODE_TX_RX;		    
	HAL_UART_Init(&UART2_Handler);					    

	UART3_Handler.Instance=USART3;					    
	UART3_Handler.Init.BaudRate=bound;				    
	UART3_Handler.Init.WordLength=UART_WORDLENGTH_8B;  
	UART3_Handler.Init.StopBits=UART_STOPBITS_1;	    
	UART3_Handler.Init.Parity=UART_PARITY_NONE;		    
	UART3_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   
	UART3_Handler.Init.Mode=UART_MODE_TX_RX;		    
	HAL_UART_Init(&UART3_Handler);					   

	UART4_Handler.Instance=UART4;					    
	UART4_Handler.Init.BaudRate=bound;				    
	UART4_Handler.Init.WordLength=UART_WORDLENGTH_8B;   
	UART4_Handler.Init.StopBits=UART_STOPBITS_1;	    
	UART4_Handler.Init.Parity=UART_PARITY_NONE;		    
	UART4_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   
	UART4_Handler.Init.Mode=UART_MODE_TX_RX;		    
	HAL_UART_Init(&UART4_Handler);					   

	UART5_Handler.Instance=UART5;					    
	UART5_Handler.Init.BaudRate=bound;				   
	UART5_Handler.Init.WordLength=UART_WORDLENGTH_8B;  
	UART5_Handler.Init.StopBits=UART_STOPBITS_1;	    
	UART5_Handler.Init.Parity=UART_PARITY_NONE;		    
	UART5_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;  
	UART5_Handler.Init.Mode=UART_MODE_TX_RX;		    
	HAL_UART_Init(&UART5_Handler);					    

	UART6_Handler.Instance=USART6;					    
	UART6_Handler.Init.BaudRate=bound;				    
	UART6_Handler.Init.WordLength=UART_WORDLENGTH_8B;   
	UART6_Handler.Init.StopBits=UART_STOPBITS_1;	    
	UART6_Handler.Init.Parity=UART_PARITY_NONE;		    
	UART6_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   
	UART6_Handler.Init.Mode=UART_MODE_TX_RX;		    
	HAL_UART_Init(&UART6_Handler);					    

	UART7_Handler.Instance=UART7;					    
	UART7_Handler.Init.BaudRate=bound;				    
	UART7_Handler.Init.WordLength=UART_WORDLENGTH_8B;   
	UART7_Handler.Init.StopBits=UART_STOPBITS_1;	    
	UART7_Handler.Init.Parity=UART_PARITY_NONE;		   
	UART7_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   
	UART7_Handler.Init.Mode=UART_MODE_TX_RX;		    
	HAL_UART_Init(&UART7_Handler);					   
	
	//HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);
  
}

//UART�ײ��ʼ����ʱ��ʹ�ܣ��������ã��ж�����
//�˺����ᱻHAL_UART_Init()����
//huart:���ھ��
//USART1	IMU
//USART2	����
//USART3	4Gģ��
//UART4		����ģ��
//UART5		GPS
//USART6	��ѹ������


void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO????
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USART1)//����Ǵ���1�����д���1 MSP��ʼ��  
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			//ʹ��GPIOAʱ��
		__HAL_RCC_USART1_CLK_ENABLE();			//ʹ��USART1ʱ��
	
		GPIO_Initure.Pin=GPIO_PIN_9;			//PA9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			//����
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//����
		GPIO_Initure.Alternate=GPIO_AF7_USART1;	//����ΪUSART1
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA9

		GPIO_Initure.Pin=GPIO_PIN_10;			//PA10
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA10
		
#if EN_USART1_RX
		__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);		//���������ж�
		HAL_NVIC_EnableIRQ(USART1_IRQn);				//ʹ��USART1�ж�ͨ��
		HAL_NVIC_SetPriority(USART1_IRQn,0,0);			//��ռ���ȼ�3�������ȼ�3
#endif	
	}

	if(huart->Instance==USART2)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			
		__HAL_RCC_USART2_CLK_ENABLE();			
	
		GPIO_Initure.Pin=GPIO_PIN_2;			
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		
		GPIO_Initure.Pull=GPIO_PULLUP;			
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		
		GPIO_Initure.Alternate=GPIO_AF7_USART2;	
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	

		GPIO_Initure.Pin=GPIO_PIN_3;			
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	
		
#if EN_USART2_RX
		__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);		
		HAL_NVIC_EnableIRQ(USART2_IRQn);				
		HAL_NVIC_SetPriority(USART2_IRQn,3,7);			
#endif	
	}

	if(huart->Instance==USART3)
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();			
		__HAL_RCC_USART3_CLK_ENABLE();			
	
		GPIO_Initure.Pin=GPIO_PIN_10;			
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;	
		GPIO_Initure.Pull=GPIO_PULLUP;			
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		
		GPIO_Initure.Alternate=GPIO_AF7_USART3;	
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	

		GPIO_Initure.Pin=GPIO_PIN_11;			
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	
		
#if EN_USART3_RX
		__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);		
		HAL_NVIC_EnableIRQ(USART3_IRQn);				
		HAL_NVIC_SetPriority(USART3_IRQn,3,5);			
#endif	
	}

	if(huart->Instance==UART4)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			
		__HAL_RCC_UART4_CLK_ENABLE();			
	
		GPIO_Initure.Pin=GPIO_PIN_12;			
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		
		GPIO_Initure.Pull=GPIO_PULLUP;			
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		
		GPIO_Initure.Alternate=GPIO_AF6_UART4;	
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	

		GPIO_Initure.Pin=GPIO_PIN_11;			
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	
		
#if EN_UART4_RX
		__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);		
		HAL_NVIC_EnableIRQ(UART4_IRQn);				
		HAL_NVIC_SetPriority(UART4_IRQn,3,5);			
#endif	
	}

	if(huart->Instance==UART5)
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();			
		__HAL_RCC_UART5_CLK_ENABLE();			
	
		GPIO_Initure.Pin=GPIO_PIN_13;			
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		
		GPIO_Initure.Pull=GPIO_PULLUP;			
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		
		GPIO_Initure.Alternate=GPIO_AF8_UART5;	
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	

		GPIO_Initure.Pin=GPIO_PIN_12;			
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	
		
#if EN_UART5_RX
		__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);		
		HAL_NVIC_EnableIRQ(UART5_IRQn);				
		HAL_NVIC_SetPriority(UART5_IRQn,3,4);			
#endif	
	}

	if(huart->Instance==USART6)
	{
		__HAL_RCC_GPIOC_CLK_ENABLE();			
		__HAL_RCC_USART6_CLK_ENABLE();			
	
		GPIO_Initure.Pin=GPIO_PIN_6;			
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		
		GPIO_Initure.Pull=GPIO_PULLUP;			
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		
		GPIO_Initure.Alternate=GPIO_AF8_USART6;	
		HAL_GPIO_Init(GPIOC,&GPIO_Initure);	   	

		GPIO_Initure.Pin=GPIO_PIN_7;			
		HAL_GPIO_Init(GPIOC,&GPIO_Initure);	   	
		
#if EN_USART6_RX
		__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);		
		HAL_NVIC_EnableIRQ(USART6_IRQn);				
		HAL_NVIC_SetPriority(USART6_IRQn,3,6);			
#endif	
	}

	if(huart->Instance==UART7)
	{
		__HAL_RCC_GPIOF_CLK_ENABLE();			
		__HAL_RCC_UART7_CLK_ENABLE();			
	
		GPIO_Initure.Pin=GPIO_PIN_6;			
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		
		GPIO_Initure.Pull=GPIO_PULLUP;			
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		
		GPIO_Initure.Alternate=GPIO_AF8_UART7;	
		HAL_GPIO_Init(GPIOF,&GPIO_Initure);	   	

		GPIO_Initure.Pin=GPIO_PIN_7;			
		HAL_GPIO_Init(GPIOF,&GPIO_Initure);	   
		
#if EN_UART7_RX
		__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);		
		HAL_NVIC_EnableIRQ(UART7_IRQn);				
		HAL_NVIC_SetPriority(UART7_IRQn,3,5);			
#endif	
	}

	

}
int test_val00;


//����1�жϷ������
void USART1_IRQHandler(void)                	
{ 
	u8 Res;
	

	if((__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_RXNE)!=RESET))  
	{
        HAL_UART_Receive(&UART1_Handler,&Res,1,1000); 
		 	
		s1_rx_buf[s1_tail++] = Res;
		if(s1_rx_buf[0] != 0xAA)
			{
				s1_tail = 0;
				//test_val00++;
			}
		if(s1_tail >= S1_RX_SIZE)
		{
			
			s1_tail = 0;
			s1_recv_over = 1;
			
			 read_imu_old();	
				

		}	
		
		
	}

	
	HAL_UART_IRQHandler(&UART1_Handler);	

} 

void USART2_IRQHandler(void)                	
{ 
	
	u32 timeout=0;
    u32 maxDelay=0x1FFFF;
#if SYSTEM_SUPPORT_OS	 	
	OSIntEnter();    
#endif
	
	HAL_UART_IRQHandler(&UART2_Handler);	
	
	timeout=0;
    while (HAL_UART_GetState(&UART2_Handler)!=HAL_UART_STATE_READY)
	{
        timeout++;
        if(timeout>maxDelay) break;		
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UART2_Handler,(u8 *)aRxBuffer, RXBUFFERSIZE)!=HAL_OK)
	{
        timeout++; 
        if(timeout>maxDelay) break;	
	}
#if SYSTEM_SUPPORT_OS	 	
	OSIntExit();  											 
#endif

} 

void USART3_IRQHandler(void)                	
{ 
	u8 Res;

	if((__HAL_UART_GET_FLAG(&UART3_Handler,UART_FLAG_RXNE)!=RESET))  
	{
        HAL_UART_Receive(&UART3_Handler,&Res,1,1000); 
		   		 
	}
	HAL_UART_IRQHandler(&UART3_Handler);	

} 
u8 ble_test_val = 0;
void UART4_IRQHandler(void)    
{
	u8 Res;

	if((__HAL_UART_GET_FLAG(&UART4_Handler,UART_FLAG_RXNE)!=RESET))  
	{
        HAL_UART_Receive(&UART4_Handler,&Res,1,1000); 
		s4_rx_buf[s4_tail++] = Res;
		if(s4_tail >=S4_RX_SIZE)
		{
			s4_tail = 0;
			s4_recv_over = 1;
		}
		
		// if(s7_tail == S7_RX_SIZE)
		// {
		// 	s7_tail = 0;
		// 	s7_recv_over = 1;
		// }   
		// ble_test_val = s7_rx_buf[0];		 
	}
	HAL_UART_IRQHandler(&UART4_Handler);	

}

u8 gps_debug_start = 0;
u8 gps_value;
void UART5_IRQHandler(void)    //GPS
{
	u8 Res;

	if((__HAL_UART_GET_FLAG(&UART5_Handler,UART_FLAG_RXNE)!=RESET)) 
	{
        HAL_UART_Receive(&UART5_Handler,&Res,1,1000); 
			s5_rx_buf[s5_tail++] = Res;
			if(s5_tail >= S5_RX_SIZE)
				s5_tail = 0;

		// if(s5_tail == S5_RX_SIZE)
		// {
		// 	s5_recv_over =1;
		// 	s5_tail = 0;
		// }
		
		//HAL_UART_Transmit(&UART2_Handler,&Res,40,1000);
		   		 
	}
	HAL_UART_IRQHandler(&UART5_Handler);	

}

void USART6_IRQHandler(void)                	
{ 
	u8 Res;

	if((__HAL_UART_GET_FLAG(&UART6_Handler,UART_FLAG_RXNE)!=RESET))  
	{
        HAL_UART_Receive(&UART6_Handler,&Res,1,1000); 
		
		s6_rx_buf[s6_tail++] = Res;
		if(s6_rx_buf[0] != 80)
		{
			s6_tail = 0;
		}
		
		if(s6_rx_buf[s6_tail-1] == '\n')
		{
			s6_recv_length = s6_tail; 
			s6_tail = 0;
			s6_recv_over = 1;
		}   
		  		 
	}
	HAL_UART_IRQHandler(&UART6_Handler);	

} 

	
	
    

void UART7_IRQHandler(void)    
{
	u8 Res;

	if((__HAL_UART_GET_FLAG(&UART7_Handler,UART_FLAG_RXNE)!=RESET))  
	{
        HAL_UART_Receive(&UART7_Handler,&Res,1,1000); 
		
		
		
		
		// if(s7_tail == S7_RX_SIZE)
		// {
		// 	s7_tail = 0;
		// 	s7_recv_over = 1;
		// }   
		// ble_test_val = s7_rx_buf[0];		 
	}
	HAL_UART_IRQHandler(&UART7_Handler);	

}




 









void imu_error_handle()
{

}



