#include "tty.h"
#include "../init/stm32f10x_it.h"

#define  tty_pin_tx	GPIO_Pin_9
#define  tty_pin_rx GPIO_Pin_10

void tty_init(void){
		USART_InitTypeDef USART_InitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;

		//config pin
	  GPIO_InitStructure.GPIO_Pin = tty_pin_tx;	         //tx
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(GPIOA, &GPIO_InitStructure);		   

		GPIO_InitStructure.GPIO_Pin = tty_pin_rx;	         //rx
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		//usart configure
	  USART_StructInit(&USART_InitStructure);
	  USART_InitStructure.USART_BaudRate =9600;
	  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	  USART_InitStructure.USART_StopBits = USART_StopBits_1;
	  USART_InitStructure.USART_Parity = USART_Parity_No;
	  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	 
	  USART_Init(USART1,& USART_InitStructure);			  	  
	  USART_Cmd(USART1, ENABLE);
}

void USART1_IRQHandler(void){
	u16 rev;
	rev=USART_ReceiveData(USART1);	
}


int tty_command(u8 *cmd){
		return 1;
}

int tty_printf(u8 *str){
	while(*str){
		USART_SendData(USART1,*str++);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	}
	return 1;
}
