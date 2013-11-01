#include "../kernel/CONFIG/includes.h"
#include "usart.h"
#include "../init/stm32f10x_it.h"
//#include "stm32f10x.h"

#define  usart_pin_tx	GPIO_Pin_2
#define  usart_pin_rx GPIO_Pin_3
#define Buffer_Size 20
u8 Rev_Buffer[Buffer_Size];

#define Binary
//#define Json
//#define Xml



void usart_init(void){
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    //config pin
    GPIO_InitStructure.GPIO_Pin = usart_pin_tx;	         //tx
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOA, &GPIO_InitStructure); 

    GPIO_InitStructure.GPIO_Pin = usart_pin_rx;	         //rx
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
    USART_Init(USART2,& USART_InitStructure);
    //irq fun
    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
    //open usart2
    USART_Cmd(USART2, ENABLE);
}

#include "../../../SHP.h"


//for delay handle
void save_char_to_buf_or_handle(char c,char handle_or_not)
{
		/***********SHP*******begin*******/{
		S16 shp_handle_recviced_data(U8* bin,U16 len);
		static char buf[128];
		static int recved_size = 0;
		if(handle_or_not){
			if(recved_size>0){
				shp_handle_recviced_data((U8*)buf,recved_size);
				recved_size = 0;
			}
		}else{
			buf[recved_size++]=c;
			if(recved_size>=sizeof(buf)){
				shp_handle_recviced_data((U8*)buf,recved_size);
				recved_size = 0;
			}
		}
		/***********SHP*******end*********/}
}

void USART2_IRQHandler(void){
#if 0
	static u8 i=0;
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET){
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		Rev_Buffer[i]=USART_ReceiveData(USART2);
	}
	if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET){
		USART_ClearFlag(USART2,USART_FLAG_ORE);
		USART_ReceiveData(USART2);
	}
	if(Rev_Buffer[i]=='a'){
		usart_send("nihao");
	}
	i++;
    
#else

    unsigned char c;
    if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET){
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		c=USART_ReceiveData(USART2);
		}else{
	    //not recive data
	    return ;
    }
		
		
		/***********SHP*******begin*******/{
		save_char_to_buf_or_handle(c,0);
		/***********SHP*******end*********/}
		
 
#endif

#if 0
    len = tcpSoc->read(temp_recv,sizeof(temp_recv));
		if(len>0)
		{
			qDebug("recived %d byte data :",len);
			qDebug(mem2Hex(temp_recv,len));
			//memcpy(temp_recv,recv_buffer+recved_len,len);
			min_len = sizeof(rd.recv_buffer) - rd.recved_size;
			if(min_len>len){
				min_len=len;
			}

			memcpy(rd.recv_buffer+rd.recved_size,temp_recv,min_len);
			rd.recved_size+=min_len;

			if(recived_data_handler){
				int used_len = 0;
				while(rd.recved_size){
					used_len = recived_data_handler(&rd);
					if(used_len>0 && used_len<=rd.recved_size){
						rd.recved_size -= used_len;
						memcpy(rd.recv_buffer+used_len,rd.recv_buffer,rd.recved_size);
					}else
					{
						rd.recved_size = 0;
					}
				}
			}else
			{
				rd.recved_size = 0;
			}
		}else{
			qDebug("len = %d break",len);
			break;
		}
#endif
}



void usart_send(u8 *data){
	while(*data){
		USART_SendData(USART2,*data++);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
	}
}

void sendData(U8* data,U16 size)
{
	
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	
    #if OS_CRITICAL_METHOD == 3
    OS_CPU_SR cpu_sr;
    #endif
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
    OS_ENTER_CRITICAL();
	while(size--){
		USART_SendData(USART2,*data++);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
	}
    OS_EXIT_CRITICAL();
}



