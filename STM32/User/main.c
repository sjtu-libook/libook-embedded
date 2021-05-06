
#include "stm32f10x.h"
#include "stm32f10x_it.h"
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
	tx_stack_init();
	rx_stack_init();
	
  while(1)
	{	
		// 中断处理程序接收请求，接收到的数据存在rx_stack接收栈中
		// 接收格式为"!req[01][01]~"，其中'!'放入开始位，'~'放入结束位，中间值放在rx_stack.data中
		// rx_stack.lock_flag标识中断处理程序接收请求完成，进入锁状态
		if(rx_stack.lock_flag == 1) {
			if (rx_stack.data[0] == 'r' && rx_stack.data[1] == 'e' && rx_stack.data[2] == 'q') { 
				if (rx_stack.data[3] == '0' && rx_stack.data[4] == '0') {
					// get token
					// 返回message格式： 1. "True{tocken}" 首次落座   2. "False" 非首次落座  3. "Null" 未落座
					// 通过get_token函数来获取token，成功失败对应上面的消息格式返回，都是字符串形式返回
					printf("True%d", 123456); // 这里为了测试固定返回值（偷懒）
				}
				else if (rx_stack.data[3] == '0' && rx_stack.data[4] == '1') {
					// get fingerprint id
					// 返回message格式： 1. "Suc{id}" 返回ID  2. "Null" ID尚未准备
					printf("Suc%d", 1);
				}
				else if (rx_stack.data[3] == '1' && rx_stack.data[4] == '0') {
					// get temperature
					// 返回message格式: "Temp{t}"
					// 这部分已经完成了，已完成获取温度的api，这里直接调用，按照格式返回
					Temperature = SMBus_ReadTemp();
		      printf("Temp%f", Temperature);
				}
				else if (rx_stack.data[3] == '1' && rx_stack.data[4] == '1') {
					// is_leave
					// message格式: "True"离开 "False"未离开
					// 只需要有一个变量保存状态即可，需要时返回
					printf("False");
				}
			}
			rx_stack.lock_flag = 0;
		}
		// 通信顺畅要求
		delay_ms(100);	
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
