/**********
*author:Zachary
*Zachary's os init file
***********/
#include "../kernel/CONFIG/includes.h"
#include "../APP/task.h"
#include "stm32f10x_rcc.h"
#include "CONFIG.h"
#include "Delay.h"
#include "framework.h"
#include "tty.h"
/////////////////////////UCOSII任务设置///////////////////////////////////
//START 任务
//设置任务优先级
#define START_TASK_PRIO      			10 //开始任务的优先级设置为最低
//设置任务堆栈大小
#define START_STK_SIZE  				64
//任务堆栈	
OS_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *pdata);	 		

//任务1
//设置任务优先级
#define LED0_TASK_PRIO       			7 
//设置任务堆栈大小
#define LED0_STK_SIZE  		    		64
//任务堆栈	
OS_STK LED0_TASK_STK[LED0_STK_SIZE];
//任务函数


//LED1任务
//设置任务优先级
#define LED1_TASK_PRIO       			6 
//设置任务堆栈大小
#define LED1_STK_SIZE  					64
//任务堆栈
OS_STK LED1_TASK_STK[LED1_STK_SIZE];
//任务函数

int main(void)
{		 
	RCC_Configuration();
	NVIC_Config(); 	
	DelayMs_init(72);
	tty_init();
	OSInit();
 	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//创建起始任务
	OSStart();
	return 1; //never come here
}


//开始任务
void start_task(void *pdata)
{
    OS_CPU_SR cpu_sr=0;
    pdata = pdata; 
    OS_ENTER_CRITICAL();			//进入临界区(无法被中断打断)    
    OSTaskCreate(task1,(void *)0,(OS_STK*)&LED0_TASK_STK[LED0_STK_SIZE-1],LED0_TASK_PRIO);						   
    OSTaskCreate(led1_task,(void *)0,(OS_STK*)&LED1_TASK_STK[LED1_STK_SIZE-1],LED1_TASK_PRIO);	 				   
    OSTaskSuspend(START_TASK_PRIO);	//挂起起始任务.
    OS_EXIT_CRITICAL();				//退出临界区(可以被中断打断)
}


void driver_init(void)
{

}
void drive_device(void)
{

}

