#include "stm32f10x.h"
#include "CONFIG.h"
#ifdef QB_UCOSII
#include "misc.h"
#else
#include "stm32f10x_nvic.h"
#endif
/**************
*clock config
*******************/
void RCC_Configuration(void)
{
	SystemInit();
	/*
	RCC_HSEConfig(RCC_HSE_ON);
	HSEStartUpStatus=RCC_WaitForHSEStartUp();
	if(HSEStartUpStatus==SUCCESS){
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		PCC_PCLK2Config(RCC_HCLK_Div1);
		PCC_PCLK1Config(RCC_HCLK_Div4);//18M
	}
	*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3|RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|
													RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
}


/***************
*irq config

*
************************/
void NVIC_Config(void) 
{ 
	#if 1//qbing
    NVIC_InitTypeDef NVIC_InitStructure; 				 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 	 //设置优先级分组：先占优先级和从优先级
    //NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQChannel;          //通道 使能或者失能指定的IRQ 通道
    NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn   ;          //通道 使能或者失能指定的IRQ 通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;// 该参数设置了成员NVIC_IRQChannel  中的先占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0; //   该参数设置了成员NVIC_IRQChannel  中的从优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE; 		 //通道开启
    NVIC_Init(&NVIC_InitStructure); 
	#endif
} 

#if 0
/* 
* 函数名：RTC_Configuration  * 描述  ：配置RTC  * 输入  ：无  * 输出  ：无 
* 调用  ：外部调用  */ 
void RTC_Configuration(void) { 
	/* 使能PWR和BKP时钟 */  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR|RCC_APB1Periph_BKP, ENABLE); 
	/*取消后备区域的写保护，因为后备寄存器中放的是重要的数据，默认是不允许往里面写入值的 */ 
	PWR_BackupAccessCmd(ENABLE); 
	/* 将后背寄存器的寄存器值设为默认值 */  
	BKP_DeInit(); 

	/* 打开外部低速晶振，RTC可以选择的时钟源是外部和内部低速
	晶振及外部高速晶振，这里我们选择外部低速晶振32768HZ */ 
	RCC_LSEConfig(RCC_LSE_ON); 
	/*等待外部低速晶振准备就序*/ 
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)  {} 
	/*选择外部低速晶振为RTC的时钟源*/  
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);  
	/* Enable RTC Clock */  
	RCC_RTCCLKCmd(ENABLE); 
	/* 等待RTC寄存器与RTC的APB时钟同步 */
	RTC_WaitForSynchro(); 
	/* 等待上次对RTC寄存器配置完成 */ 
	RTC_WaitForLastTask();  /* 使能RTC中断 */ 
	RTC_ITConfig(RTC_IT_SEC, ENABLE);  
	/* 等待上次对RTC寄存器配置完成 */ 
	RTC_WaitForLastTask(); 

	/* 设置RTC的预分频值，因为外部低速晶振是32768，所以选
	择 RTC计数器计数频率= RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */ 
	RTC_SetPrescaler(32767);  

	/* 等待上次对RTC寄存器配置完成 */ 
	RTC_WaitForLastTask();  
}  

/* 
* 函数名：RTC_IRQHandler(void)  
* 描述  ：RTC中断服务函数  
* 输入  ：无  
* 输出  ：无  
* 调用  ：无  
*/ 
void RTC_IRQHandler(void) { 
	/*判断中断标志位是否被置位*/  
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)  {
		/* 清除中断标志位 */ 
		RTC_ClearITPendingBit(RTC_IT_SEC); 


		/* TimeDisplay是一个标志位，只有等于1时才让串口发送时
		间数据，即让串口一秒发一次时间值  */ 
		//TimeDisplay = 1; 

		/* Wait until last write operation on RTC registers has finished */  
        RTC_WaitForLastTask(); 


		/* 当时间走到23:59:59秒时RTC计数器中的值清零，
		0x00015180=23*3600+56*60+59*/   
		if (RTC_GetCounter() == 0x00015180)   
        {    
            RTC_SetCounter(0x0); 
    		/* Wait until last write operation on RTC registers has finished */    
    		RTC_WaitForLastTask(); 
		} 
	} 
}
#endif
