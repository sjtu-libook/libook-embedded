#ifndef  __MATRIXKEY_H__
#define  __MATRIXKEY_H__
#include "stm32f10x.h"


//第一组  行
//PC  6
#define KEY1_1_GPIO_PORT    	GPIOA		              
#define KEY1_1_GPIO_CLK 	    RCC_APB2Periph_GPIOA	 
#define KEY1_1_GPIO_PIN		GPIO_Pin_4

#define   X1_0    GPIO_ResetBits(KEY1_1_GPIO_PORT, KEY1_1_GPIO_PIN);
#define   X1_1    GPIO_SetBits(KEY1_1_GPIO_PORT, KEY1_1_GPIO_PIN);


//PC  8
#define KEY1_2_GPIO_PORT    	GPIOA		            
#define KEY1_2_GPIO_CLK 	    RCC_APB2Periph_GPIOA	 
#define KEY1_2_GPIO_PIN		GPIO_Pin_5			 

#define   X2_0    GPIO_ResetBits(KEY1_2_GPIO_PORT, KEY1_2_GPIO_PIN);
#define   X2_1    GPIO_SetBits(KEY1_2_GPIO_PORT, KEY1_2_GPIO_PIN);


//PA  8
#define KEY1_3_GPIO_PORT    	GPIOA			              
#define KEY1_3_GPIO_CLK 	    RCC_APB2Periph_GPIOA
#define KEY1_3_GPIO_PIN		GPIO_Pin_6	

#define   X3_0    GPIO_ResetBits(KEY1_3_GPIO_PORT, KEY1_3_GPIO_PIN);
#define   X3_1    GPIO_SetBits(KEY1_3_GPIO_PORT, KEY1_3_GPIO_PIN);


//PA  10
#define KEY1_4_GPIO_PORT    	GPIOA			              
#define KEY1_4_GPIO_CLK 	    RCC_APB2Periph_GPIOA		 
#define KEY1_4_GPIO_PIN		GPIO_Pin_7

#define   X4_0    GPIO_ResetBits(KEY1_4_GPIO_PORT, KEY1_4_GPIO_PIN);
#define   X4_1    GPIO_SetBits(KEY1_4_GPIO_PORT, KEY1_4_GPIO_PIN);



#define   X_0   X1_0  \
                X2_0  \
				X3_0  \
				X4_0

#define   X_1   X1_1  \
                X2_1  \
				X3_1  \
				X4_1

//第二组  列
//
//PA  12
#define KEY2_1_GPIO_PORT    	GPIOA	              
#define KEY2_1_GPIO_CLK 	    RCC_APB2Periph_GPIOA 
#define KEY2_1_GPIO_PIN		GPIO_Pin_2			 

#define   Y1_0    GPIO_ResetBits(KEY2_1_GPIO_PORT, KEY2_1_GPIO_PIN);
#define   Y1_1    GPIO_SetBits(KEY2_1_GPIO_PORT, KEY2_1_GPIO_PIN);


// PA  14
#define KEY2_2_GPIO_PORT    	GPIOA	            
#define KEY2_2_GPIO_CLK 	    RCC_APB2Periph_GPIOA	 
#define KEY2_2_GPIO_PIN		    GPIO_Pin_3		

#define   Y2_0    GPIO_ResetBits(KEY2_2_GPIO_PORT, KEY2_2_GPIO_PIN);
#define   Y2_1    GPIO_SetBits(KEY2_2_GPIO_PORT, KEY2_2_GPIO_PIN);

//PC  10
#define KEY2_3_GPIO_PORT    	GPIOA			              
#define KEY2_3_GPIO_CLK 	    RCC_APB2Periph_GPIOA		 
#define KEY2_3_GPIO_PIN		GPIO_Pin_11

#define   Y3_0    GPIO_ResetBits(KEY2_3_GPIO_PORT, KEY2_3_GPIO_PIN);
#define   Y3_1    GPIO_SetBits(KEY2_3_GPIO_PORT, KEY2_3_GPIO_PIN);

//PC  12
#define KEY2_4_GPIO_PORT    	GPIOA              
#define KEY2_4_GPIO_CLK 	    RCC_APB2Periph_GPIOA		 
#define KEY2_4_GPIO_PIN		     GPIO_Pin_12			        

#define   Y4_0    GPIO_ResetBits(KEY2_4_GPIO_PORT, KEY2_4_GPIO_PIN);
#define   Y4_1    GPIO_SetBits(KEY2_4_GPIO_PORT, KEY2_4_GPIO_PIN);




#define   Y_0   Y1_0 \
                Y2_0;\
				Y3_0\
				Y4_0

#define   Y_1   Y1_1 \
                Y2_1;\
				Y3_1\
				Y4_1

/*初始化函数，*/
void key_GPIO_Config(int a);
uint8_t  key_seek(void);

#endif