/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   串口中断接收测试
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-指南者 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
 
 
#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#define SOFT_DELAY Delay(5000000);

void Delay(__IO uint32_t nCount); 

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{	
  /*初始化USART 配置模式为 115200 8-N-1，中断接收*/
  USART_Config();
	/* LED 端口初始化 */
	LED_GPIO_Config();	 
	
  while(1)
	{	
		// 蓝灯闪烁表示系统正常运行
		LED3_OFF;
		SOFT_DELAY;
		LED3_ON;
		SOFT_DELAY;
	}
}

void Delay(__IO uint32_t nCount)	 //简单的延时函数
{ for(; nCount != 0; nCount--);}
/*********************************************END OF FILE**********************/
