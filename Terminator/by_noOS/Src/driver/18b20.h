#ifndef __DS18B20_H__
#define __DS18B20_H__
#include<Delay.h>
#include<Define.h>


/*------------------------------------------------
                  变量定义
------------------------------------------------*/
extern	uint	 WD;					  //温度值
extern	_Bool	  WD_OK;			//温度可以用了
extern     _Bool	 	  WD_PIN;			//定义引脚



/*------------------------------------------------
                  函数声明
------------------------------------------------*/
unsigned int ReadTemperature(void);
_Bool Init_DS18B20(void);
unsigned char ReadOneChar(void);
void WriteOneChar(unsigned char dat);
extern void DelayMs(uchar t);
extern void DelayUS(uchar t);



#endif
