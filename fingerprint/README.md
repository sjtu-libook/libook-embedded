## README
### sjtu-libook libook-embedded
实验器材:
	Mini STM32F103开发板
	
实验目的:
	学习ATK-AS608指纹识别模块的使用,学会录指纹、删除指纹、设置波特率及对比等级等功能.
	
硬件资源:	
	1,GPIO（PA6）读取指纹模块触摸感应信号
	2,按键KEY0(PE4)/KEY1(PE3)/KEY2(PE2)/KEY_UP(PA0,也称之为WK_UP)
	3,串口1(波特率:115200,PA9/PA10连接在板载USB转串口芯片CH340上面)用于支持USMART调试
	4,串口2(波特率:57600,PA2/PA3)与指纹模块AS608通讯
	5,ALIENTEK 2.8/3.5/4.3/7寸TFTLCD模块(通过FSMC驱动,FSMC_NE4接LCD片选/A6接RS) 	
	6,W25Q128(SPI FLASH芯片,连接在SPI1上，用于存储字库) 
	7,24C02（用于存储LCD触摸屏一些参数）
	8,ATK-AS608指纹识别模块1个,杜邦线6根
	

	ATK-AS608模块与开发板连接方式1（通过6根杜邦线连接）：
	ATK-AS608          STM32开发板
		3.3V<----------->3.3V 
		TX<------------->PA3(RX)
		RX <------------>PA2(TX)
		GND<------------>GND 
		touch<---------->PA6
		touchVin<------>3.3V			

实验现象:	
	本实验功能简介：本实验用于测试ATK-AS608模块，总共包括三项测试：
	
	1，删指纹——与模块握手通过之后，屏幕将虚拟一个键盘，按‘del_fp’进入删除界面。在此界面可
	   以按照提示输出一个数值ID按‘Enter’删除单个指纹，也可按‘del_all’清空指纹库。如果是误
	   操作也可以按‘BACK’返回主界面。
	
	2，录指纹——按‘add_fp’进入此项。录指纹流程：按第一次手指-->按第二次手指-->自动生成模板
	   -->输入存储ID-->存储指纹成功。其中两次按指纹操作，如果超过5次无手指按下则退回主界面。
	   模块感应到手指按下0.3S（最少停留0.3S）后LCD提示指纹是否正常！
	
	3，刷指纹--与模块握手成功之后，主函数中判断指纹模块的感应引脚（touch->PA6）模块感应到手指
	   输出高电平，程序判断后进入刷指纹流程。读指纹图像-->搜索匹配指纹库-->成功或没有搜到指纹。



注意事项:
	1,4.3寸和7寸屏需要比较大电流,USB供电可能不足,请用外部电源适配器(推荐外接12V 1A电源).
	2,本例程在LCD_Init函数里面(在ILI93xx.c),用到了printf,如果不初始化串口1,将导致液晶无法显示！
	3,程序初始化时检查字库，如果提示"Font Error"则需要下载-->实验40 汉字显示实验更新字库！
	4,由于Mini开发板的UART2(PA2\PA3)连接到了Flash_CS和SD_CS，当显示汉字时会产生高低电平信号，这 信号对于AS608模块是错误的指令。错误的指令发送给AS608模块会让模块自锁，导致握手失败或有时通讯不正常。

​	5,使用2.8寸屏（电阻屏）初始化提示“Adjust the touch screen”？——是否校准触摸屏？一般选择“否”实验过程发现触摸不准则复位再进行校准！