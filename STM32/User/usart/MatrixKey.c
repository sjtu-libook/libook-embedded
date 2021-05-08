#include "MatrixKey.h"
#include "stdio.h"
#include "delay.h"

void key_GPIO_Config(int a)
{		
		/*????GPIO_InitTypeDef??????*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*??LED???GPIO????*/
	RCC_APB2PeriphClockCmd( KEY1_1_GPIO_CLK| KEY1_2_GPIO_CLK | KEY1_3_GPIO_CLK| KEY1_4_GPIO_CLK |KEY2_1_GPIO_CLK| KEY2_2_GPIO_CLK | KEY2_3_GPIO_CLK| KEY2_4_GPIO_CLK, ENABLE);

	
//KEY1  ????
		GPIO_InitStructure.GPIO_Pin = KEY1_1_GPIO_PIN;
if(a)	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
else
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(KEY1_1_GPIO_PORT, &GPIO_InitStructure);	

		GPIO_InitStructure.GPIO_Pin = KEY1_2_GPIO_PIN;
		GPIO_Init(KEY1_2_GPIO_PORT, &GPIO_InitStructure);
	
			GPIO_InitStructure.GPIO_Pin = KEY1_3_GPIO_PIN;
		GPIO_Init(KEY1_3_GPIO_PORT, &GPIO_InitStructure);
	
			GPIO_InitStructure.GPIO_Pin = KEY1_4_GPIO_PIN;
		GPIO_Init(KEY1_4_GPIO_PORT, &GPIO_InitStructure);
	

//KEY2  ????
if(a)	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
else
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

		GPIO_InitStructure.GPIO_Pin = KEY2_1_GPIO_PIN;	
		GPIO_Init(KEY2_1_GPIO_PORT, &GPIO_InitStructure);	

		GPIO_InitStructure.GPIO_Pin = KEY2_2_GPIO_PIN;
		GPIO_Init(KEY2_2_GPIO_PORT, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = KEY2_3_GPIO_PIN;
		GPIO_Init(KEY2_3_GPIO_PORT, &GPIO_InitStructure);
	
	  GPIO_InitStructure.GPIO_Pin = KEY2_4_GPIO_PIN;
		GPIO_Init(KEY2_4_GPIO_PORT, &GPIO_InitStructure);

//    Y_0;//?Y??IO???0
//		X_0;//?X??IO???0


		
}

/***************************************************************************
**??? :READ_Date
****************************************************************************/

uint8_t READ_Date(int a)
{
    if(a) {
			return   0x0f&((GPIO_ReadInputDataBit(KEY2_1_GPIO_PORT, KEY2_1_GPIO_PIN))<<3 | GPIO_ReadInputDataBit(KEY2_2_GPIO_PORT, KEY2_2_GPIO_PIN)<<2   \
				      | GPIO_ReadInputDataBit(KEY2_3_GPIO_PORT, KEY2_3_GPIO_PIN)<<1| GPIO_ReadInputDataBit(KEY2_4_GPIO_PORT, KEY2_4_GPIO_PIN));     }
	
     else  {
			 return  0x0f&((GPIO_ReadInputDataBit(KEY1_1_GPIO_PORT, KEY1_1_GPIO_PIN))<<3 | GPIO_ReadInputDataBit(KEY1_2_GPIO_PORT, KEY1_2_GPIO_PIN)<<2             \
               | GPIO_ReadInputDataBit(KEY1_3_GPIO_PORT, KEY1_3_GPIO_PIN)<<1| GPIO_ReadInputDataBit(KEY1_4_GPIO_PORT, KEY1_4_GPIO_PIN)); }
										  
}
//????
uint8_t  key_seek(void)
{	
   int mm,key_date=0;
	  key_GPIO_Config(0);
  	  X_0; Y_1;
	 delay_us(500);
	 mm=READ_Date(0);  //???0 ,??????key1????X???
	 if(mm!=0x00){
	     delay_us(500);
		   mm=READ_Date(0);
		   if(mm!=0x00){
				  switch(mm)
					{
						case(0x08): key_date=0;
							          break;
						case(0x04): key_date=4;
							          break;
						case(0x02): key_date=8;
							          break;
						case(0x01): key_date=12;
							          break;
					}
					key_GPIO_Config(1);
				   	X_1;Y_0;
			       delay_us(500);
	               mm=READ_Date(1);
				 	switch(mm)
					{
						case(0x08): key_date=key_date+1;
							          break;
						case(0x04): key_date=key_date+2;
							          break;
						case(0x02): key_date=key_date+3;
							          break;
						case(0x01): key_date=key_date+4;
							          break;
					}
				  while(0x00!=READ_Date(1));    //??????				
					switch(key_date)
					{
						case(8): key_date=3;
										 break;
						case(12): key_date=2;
										 break;
						case(16): key_date=1;
										 break;
						case(7): key_date=6;
										 break;
						case(11): key_date=5;
										 break;
						case(15): key_date=4;
										 break;
						case(6): key_date=9;
										 break;
						case(10): key_date=8;
										 break;
						case(14): key_date=7;
										 break;
						case(9): key_date = 0;
								break;
						default: key_date=10;
										 break;
					}
				  return  (uint8_t)key_date;	  
			 }
 }
return 0;

}
