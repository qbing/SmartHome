#ifndef	 USART
#define	 USART
#include "../FWlib/inc/stm32f10x_usart.h"
#include "../FWlib/inc/stm32f10x_gpio.h"
void usart_init(void);
void usart_send(u8 *data);
#endif

