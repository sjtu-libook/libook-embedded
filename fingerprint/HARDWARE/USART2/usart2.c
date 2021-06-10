#include "sys.h"
#include "usart2.h"	
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"
#include "timer.h" 
//////////////////////////////////////////////////////////////////////////////////	   
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK Mini STM32������
//����2��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/3/29
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved	
//********************************************************************************
//�޸�˵��
//��
////////////////////////////////////////////////////////////////////////////////// 	

//���ڽ��ջ����� 	
u8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; 				//���ջ���,���USART2_MAX_RECV_LEN���ֽ�.
u8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; 

//ͨ���жϽ�������2���ַ�֮���ʱ������10ms�������ǲ���һ������������.
//���2���ַ����ռ������10ms,����Ϊ����1����������.Ҳ���ǳ���10msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���
vu16 USART2_RX_STA=0;
void USART2_IRQHandler(void)
{
	u8 res;	      
	if(USART2->SR&(1<<5))//���յ�����
	{	 
		res=USART2->DR; 			 
		if((USART2_RX_STA&(1<<15))==0)//�������һ������,��û�б�����,���ٽ�����������
		{ 
			if(USART2_RX_STA<USART2_MAX_RECV_LEN)	//�����Խ�������
			{
				TIM4->CNT=0;         				//���������
				if(USART2_RX_STA==0) 				//ʹ�ܶ�ʱ��4���ж� 
				{
					TIM4->CR1|=1<<0;     			//ʹ�ܶ�ʱ��4
				}
				USART2_RX_BUF[USART2_RX_STA++]=res;	//��¼���յ���ֵ	 
			}else 
			{
				USART2_RX_STA|=1<<15;				//ǿ�Ʊ�ǽ������
			} 
		}
	}  				 											 
}
//��ʼ��IO ����2
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������ 
void usart2_init(u32 pclk1,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk1*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   				//ʹ��PORTA��ʱ�� 
	RCC->APB1ENR|=1<<17;  				//ʹ�ܴ���2ʱ�� 
	GPIOA->CRL&=0XFFFF00FF;				//PA2\PA3IO״̬����
	GPIOA->CRL|=0X00008B00;				//IO״̬����
	
	RCC->APB1RSTR|=1<<17;   			//��λ����2
	RCC->APB1RSTR&=~(1<<17);			//ֹͣ��λ	
	//����������
// 	USART2->BRR=(pclk1*1000000)/(bound);// ����������	
	USART2->BRR=mantissa;
	USART2->CR1|=0X200C;  				//1λֹͣ,��У��λ.
	//ʹ�ܽ����ж� 
	USART2->CR1|=1<<5;    				//���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(0,1,USART2_IRQn,2);//��2
	TIM4_Int_Init(99,7199);				//10ms�ж�
	TIM4->CR1&=~(1<<0);						//�رն�ʱ��4
	USART2_RX_STA=0;							//����
}

//����2,printf ����
//ȷ��һ�η������ݲ�����USART2_MAX_SEND_LEN�ֽ�
void u2_printf(char* fmt,...)  
{  
	u16 i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)USART2_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART2_TX_BUF);		//�˴η������ݵĳ���
	for(j=0;j<i;j++)							//ѭ����������
	{
		while((USART2->SR&0X40)==0);			//ѭ������,ֱ���������   
		USART2->DR=USART2_TX_BUF[j];  
	} 
}





