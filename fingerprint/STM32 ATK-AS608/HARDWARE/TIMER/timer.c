#include "timer.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK Mini STM32������
//ͨ�ö�ʱ�� ��������			   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/3/07
//�汾��V1.2
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2009-2019
//All rights reserved	
//********************************************************************************
//V1.1 20140306 
//����TIM1_CH1��PWM�������������� 
//V1.2 20140307
//����TIM2���벶���ʼ������TIM2_Cap_Init�����жϴ���
////////////////////////////////////////////////////////////////////////////////// 	  
extern vu16 USART2_RX_STA; 
//��ʱ��4�жϷ������	 
void TIM4_IRQHandler(void)
{ 		    		  			    
	if(TIM4->SR&0X0001)//����ж�
	{
		USART2_RX_STA|=1<<15;	//��ǽ������	
		TIM4->CR1&=~(1<<0);		//�رն�ʱ��4
	}				   
	TIM4->SR&=~(1<<0);//����жϱ�־λ 	    
}
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��4!
void TIM4_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<2;	//TIM4ʱ��ʹ��    
 	TIM4->ARR=arr;  	//�趨�������Զ���װֵ 
	TIM4->PSC=psc;  	//Ԥ��Ƶ������
	TIM4->DIER|=1<<0;   //���������ж�				
	TIM4->CR1|=0x01;    //ʹ�ܶ�ʱ��4
  MY_NVIC_Init(0,2,TIM4_IRQn,2);//��ռ1�������ȼ�3����2									 
}
//TIM1_CH1 PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM1_PWM_Init(u16 arr,u16 psc)
{		 					 
	//�˲������ֶ��޸�IO������
	RCC->APB2ENR|=1<<11; 	//TIM1ʱ��ʹ��    
	GPIOA->CRH&=0XFFFFFFF0;	//PA8���֮ǰ������
	GPIOA->CRH|=0X0000000B;	//���ù������ 
	
	TIM1->ARR=arr;			//�趨�������Զ���װֵ 
	TIM1->PSC=psc;			//Ԥ��Ƶ������
  
	TIM1->CCMR1|=7<<4;  	//CH1 PWM2ģʽ		 
	TIM1->CCMR1|=1<<3; 		//CH1Ԥװ��ʹ��	 
 	TIM1->CCER|=1<<0;   	//OC1 ���ʹ��	   
	TIM1->BDTR|=1<<15;   	//MOE �����ʹ��	   

	TIM1->CR1=0x0080;   	//ARPEʹ�� 
	TIM1->CR1|=0x01;    	//ʹ�ܶ�ʱ��1 										  
}  















