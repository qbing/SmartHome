#include "DS1820.h"
#include "stm32f10x_gpio.h"
#include "tty.h"
/******************************************************************************************************************************/
uint8_t DS18B20_Rst(void)			   //ds18b20初始化函数
{
	 u8 success;
	 success=0;
   DS1820_DQ_H;
   DelayUs(30);
   DS1820_DQ_L;
   DelayUs(500);
   DS1820_DQ_H;
   DelayUs(40);
	 if(DS1820_READ_PIN){};
	 DelayUs(500);
	 DS1820_DQ_H;
	 return success;
}
//从DS18B20读取一个字节
//返回值：读到的数据
uint8_t DS18B20_Read_Byte(void)    // read one byte
{        
    uint8_t i,dat;
    dat=0;
		DS1820_DQ_H;
		DelayUs(5);
		for (i=1;i<=8;i++) 
		{
					DS1820_DQ_L;
					DelayUs(5);
					dat>>=1;
					DS1820_DQ_H; 
					DelayUs(5);
					if(DS1820_READ_PIN){
						dat|=0x80;
					}else{
						dat&=0x7f;
					}
					DelayUs(65);
					DS1820_DQ_H; 
			}						    
			return dat;
}

//写一个字节到DS18B20
//dat：要写入的字节
void DS18B20_Write_Byte(u8 dat)     
 {             
    uint8_t j;
    uint8_t testb;
    for (j=1;j<=8;j++) 
		{
        testb=dat&0x01;        
				DS1820_DQ_L;
				DelayUs(5);
				dat=dat/2;
        if (testb) // Write 1
        {
						DS1820_DQ_H;             
        }
        else // Write 0
        {
            DS1820_DQ_L;
                   
        }
				DelayUs(65);             
				DS1820_DQ_H;
				DelayUs(2);
    }
		DelayUs(25); 
}
//初始化DS18B20的IO口 DQ 同时检测DS的存在
//返回1:不存在
//返回0:存在    	 
uint8_t DS18B20_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin=DS1820_DQ ;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;  //传说OD开漏可是stm32IO可成双向
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(DS1820_GPIO, &GPIO_InitStructure); 
		return 1;
}
/*****************************
//从ds18b20得到温度值
//精度：0.1C
//arg: buffer,it's size is 10
//返回值：温度值 （-550~1250）
*******************************/
int DS18B20_Get_Temp(u8 *temp,u8 len)
{
    uint8_t TL,TH,tf,td,k;
    if(len!=11){
        return 0;
    }
    *temp++='W';
    *temp++='D';
    *temp++=':';
    DS18B20_Rst();
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0x44);// convert	 
    DelayMs(10);
    DS18B20_Rst();
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0xbe);// read tempereture reg	 
    TL=DS18B20_Read_Byte(); // LSB
    TH=DS18B20_Read_Byte(); // MSB  

    tf=TL&0x0f;						  //tf表示小数部分
    td=TH*16+TL/16;
    //整数部分
    k=td/100;
    k=k+'0';
    *temp++=k;
    k=(td%100)/10;
    k=k+'0';
    *temp++=k;
    k=td%10;
    k=k+'0';
    *temp++=k;
    *temp++='.';
    //小数部分
    tf=tf*62.5;
    k=tf/100;
    k=k+'0';
    *temp++=k;
    k=tf%100/10;
    k=k+'0';
    *temp++=k;
    //k=tf%10;
    //k=k+'0';
    *temp++='\n';
    *temp++='\0';
    return 1;
}

#include "framework.h"
S16 GetT(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
    uint8_t TL,TH,tf,td;
	S16 t;
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
    DS18B20_Rst();
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0x44);// convert	 
    DelayMs(10);
    DS18B20_Rst();
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0xbe);// read tempereture reg	 
    TL=DS18B20_Read_Byte(); // LSB
    TH=DS18B20_Read_Byte(); // MSB  

    tf=TL&0x0f;						  //tf表示小数部分
    td=TH*16+TL/16;

    t = TH;
    t <<=8;
    t |= TL;
    return (t*10)>>4;
}

