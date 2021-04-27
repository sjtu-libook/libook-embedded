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
#include "delay.h"
#include "sys.h"
#include "mlx90614.h"

#define SOFT_DELAY Delay(5000000);
//温度的单位 ℃
unsigned char TempCompany[][16]=
{
	0x06,0x09,0x09,0xE6,0xF8,0x0C,0x04,0x02,0x02,0x02,0x02,0x02,0x04,0x1E,0x00,0x00,
	0x00,0x00,0x00,0x07,0x1F,0x30,0x20,0x40,0x40,0x40,0x40,0x40,0x20,0x10,0x00,0x00,/*"¡æ",0*/	

};
void Delay(__IO uint32_t nCount); 

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{	
	float Temperature = 0; //温度变量
	char TempValue[80]={0}; //温度值 字符串
  /*初始化USART 配置模式为 115200 8-N-1，中断接收*/
  USART_Config();
	/* LED 端口初始化 */
	LED_GPIO_Config();	 
	delay_init();
	SMBus_Init();
	
	
  while(1)
	{	
		Temperature = SMBus_ReadTemp();
		printf("Temperature = %f\r\n",Temperature);	//在串口调试助手上打印结果
		delay_ms(100);//不加延时在电脑串口助手上看不到效果
		sprintf(TempValue,"%.1f", Temperature); //浮点转换为字符串	
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
