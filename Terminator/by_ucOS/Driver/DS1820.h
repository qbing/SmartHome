#ifndef __DS1820_H__
#define __DS1820_H__

#include "stm32f10x.h"
#include "../init/Delay.h"
#define DS1820_GPIO  GPIOB


#define DS1820_DQ GPIO_Pin_10
#define DS1820_READ_PIN GPIO_ReadInputDataBit(DS1820_GPIO, DS1820_DQ)
#define DS1820_DQ_H     GPIO_WriteBit(DS1820_GPIO, DS1820_DQ, Bit_SET);
#define DS1820_DQ_L     GPIO_WriteBit(DS1820_GPIO, DS1820_DQ, Bit_RESET);

uint8_t DS18B20_Rst(void);
uint8_t DS18B20_Check(void);
uint8_t DS18B20_Read_Byte(void);
void DS18B20_Write_Byte(u8 dat);
void DS18B20_Start(void);
uint8_t DS18B20_Init(void);
int DS18B20_Get_Temp(u8 *temp,u8 len);


extern void DelayUs(u32 m);
extern void DelayMs(u16 m);

#endif

