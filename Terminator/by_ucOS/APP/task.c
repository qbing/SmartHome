/******************************************************************************
 *  Copyright Statement:
 *    Copyright (c)  Q.Bryan <qbing2010@163.com>
 *    All rights reserved.
 *-----------------------------------------------------------------------------
 * Filename:      task.c
 * Description:   task file
 * Author:        Q.Bryan	2013/08/22 16:22:03
 *=============================================================================
 * HISTORY
 *------Below this line--------------------------------------------------------
 *3.your_name datetime->add_your_change_for_this_file.
 *2.your_name datetime->add_your_change_for_this_file.
 *1.Q.Bryan 2013/08/22 16:22:03->create this file.
 *------Upper this line--------------------------------------------------------
 *=============================================================================
******************************************************************************/

#include "task.h"
#include "../kernel/ucos_ii.h"
#include "../Driver/driver.h"
#include "../init/Delay.h"


void task1(void *pdata){
		u8 temp[11];
		RelayNum Relay;
		int success;
		Relay = Relay1;
		Relay_init(Relay);
		usart_init();
		DS18B20_Init();
    /***********SHP*******begin*******/{
		extern void  shp_example_init_called_by_system(void);
    shp_example_init_called_by_system();
		/***********SHP*******end*********/}
	
		while(1){
            #if 0
				success=DS18B20_Get_Temp(temp,sizeof(temp));
				if(success){
					usart_send(temp);
				}				
				Relay_Disconnect(Relay);//open led
				DelayMs(1000);
				Relay_Connect(Relay);
				DelayMs(1000);
				Relay = Relay2;
				Relay_Disconnect(Relay);
				DelayMs(1000);
				Relay_Connect(Relay);
				DelayMs(1000);
				Relay = Relay1;
            #else
    /***********SHP*******begin*******/{
		extern S16  shp_system_tick(void);
    shp_system_tick();
		tty_printf("sys_tick\r\n");
		/***********SHP*******end*********/}
                DelayMs(1000);
            #endif
		}
}

//for delay handle
void save_char_to_buf_or_handle(char c,char handle_or_not);

//LED1ÈÎÎñ
void led1_task(void *pdata)
{	  
		RelayNum Relay;
		Relay = Relay4;
		Relay_init(Relay);
		while(1){
            /*
				Relay_Disconnect(Relay);
				DelayMs(500);
				Relay_Connect(Relay);
				DelayMs(500);
				Relay = Relay3;
				Relay_Disconnect(Relay);
				DelayMs(500);
				Relay_Connect(Relay);
				DelayMs(500);
				Relay = Relay4;
		    */
				DelayMs(100);
				save_char_to_buf_or_handle(0,1);
		}
}


