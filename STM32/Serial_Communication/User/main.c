/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   �����жϽ��ղ���
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� F103-ָ���� STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
 
 
#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#define SOFT_DELAY Delay(5000000);

void Delay(__IO uint32_t nCount); 

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{	
  /*��ʼ��USART ����ģʽΪ 115200 8-N-1���жϽ���*/
  USART_Config();
	/* LED �˿ڳ�ʼ�� */
	LED_GPIO_Config();	 
	
  while(1)
	{	
		// ������˸��ʾϵͳ��������
		LED3_OFF;
		SOFT_DELAY;
		LED3_ON;
		SOFT_DELAY;
	}
}

void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{ for(; nCount != 0; nCount--);}
/*********************************************END OF FILE**********************/
