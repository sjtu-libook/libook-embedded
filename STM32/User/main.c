
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "delay.h"
#include "sys.h"
#include "mlx90614.h"
#include "MatrixKey.h"
#include "stdio.h"


#define SOFT_DELAY Delay(5000000);
//温度的单位 ℃
unsigned char TempCompany[][16]=
{
	0x06,0x09,0x09,0xE6,0xF8,0x0C,0x04,0x02,0x02,0x02,0x02,0x02,0x04,0x1E,0x00,0x00,
	0x00,0x00,0x00,0x07,0x1F,0x30,0x20,0x40,0x40,0x40,0x40,0x40,0x20,0x10,0x00,0x00,/*"¡æ",0*/	

};


void LCD_COUT(char* ch, int line) {
	char dispBuff[200];
	/********显示变量示例*******/
	LCD_SetTextColor(GREEN);

	/*转换成字符串*/
	sprintf(dispBuff, "%s ", ch);
	/*清除对应行 */
	LCD_ClearLine(LINE(line));	/* Çå³ýµ¥ÐÐÎÄ×Ö */

	/*显示该字符*/
	ILI9341_DispStringLine_EN_CH(LINE(line), dispBuff);
}
unsigned char charater_matrix[] =
{ 
0x01,0x00,0x21,0x08,0x11,0x08,0x09,0x10,0x09,0x20,0x01,0x00,0x7F,0xF8,0x00,	0x08,
0x00,0x08,0x00,0x08,0x3F,0xF8,0x00,0x08,0x00,0x08,0x00,0x08,0x7F,0xF8,0x00,0x08,

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
	uint8_t input_num=0; //输入的数字
  /*初始化USART 配置模式为 115200 8-N-1，中断接收*/
	USART_Config();
	/* LED 端口初始化 */
	LED_GPIO_Config();	 
	delay_init();
	SMBus_Init();
	key_GPIO_Config(1);
	tx_stack_init();
	rx_stack_init();

	while (1)
	{
		// 中断处理程序接收请求，接收到的数据存在rx_stack接收栈中
		// 接收格式为"!req[01][01]~"，其中'!'放入开始位，'~'放入结束位，中间值放在rx_stack.data中
		// rx_stack.lock_flag标识中断处理程序接收请求完成，进入锁状态
		if (rx_stack.lock_flag == 1) {
			if (rx_stack.data[0] == 'r' && rx_stack.data[1] == 'e' && rx_stack.data[2] == 'q') {
				if (rx_stack.data[3] == '0' && rx_stack.data[4] == '0') {
					// get token
					LCD_OUT("请输入验证码",1);
					char str[10];
					index = 0;
					while (1) {
						input_num = key_seek(); //一次使能获取一个0-9的数字,如果没有输入则返回10
						if (input_num != 10) break; //一直循环，直至获取到数字
						LCD_OUT(to_String(input_num),2);
						str[index++] = input_num;
					}
					// 返回message格式： 1. "True{tocken}" 首次落座   2. "False" 非首次落座  3. "Null" 未落座
					// 通过get_token函数来获取token，成功失败对应上面的消息格式返回，都是字符串形式返回
					if (verify(str))
						printf("True%s", str);
						LCD_OUT("验证成功",3);
					else 
						printf("False");
						LCD_OUT("验证失败",3);
				}
				else if (rx_stack.data[3] == '0' && rx_stack.data[4] == '1') {
					// get fingerprint id
					// 返回message格式： 1. "Suc{id}" 返回ID  2. "Null" ID尚未准备
					LCD_OUT("请验证指纹",1);
					int id = getFingerprintID();
					if(id == -1) {
						printf("Null");
						LCD_OUT("验证失败",2);
					}
					else {
						printf("Suc%d", id);
						LCD_OUT("验证成功",2);
					}
				}
				else if (rx_stack.data[3] == '1' && rx_stack.data[4] == '0') {
					// get temperature
					// 返回message格式: "Temp{t}"
					// 这部分已经完成了，已完成获取温度的api，这里直接调用，按照格式返回
					Temperature = SMBus_ReadTemp();
					printf("Temp%f", Temperature);
					int line = 1;
					LCD_OUT("当前温度", line);
					char str[10] = convert_toString(Temperature);
					LCD_OUT(str, line);
					line++;
				}
				else if (rx_stack.data[3] == '1' && rx_stack.data[4] == '1') {
					// is_leave
					// message格式: "True"离开 "False"未离开
					// 只需要有一个变量保存状态即可，需要时返回
					bool state = getState();
					if(stat) {
						printf("True");
					}
					else {
						printf("False");
					}
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
		// 通信顺畅要求
		delay_ms(100);	
		// 蓝灯闪烁表示系统正常运行
		LED3_OFF;
		SOFT_DELAY;
		LED3_ON;
		SOFT_DELAY;
	}

void Delay(__IO uint32_t nCount)	 //简单的延时函数
{ for(; nCount != 0; nCount--);}
/*********************************************END OF FILE**********************/
