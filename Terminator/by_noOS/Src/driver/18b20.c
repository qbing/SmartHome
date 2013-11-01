/*-----------------------------------------------
  名称：18B20温度传感器
  编写：Zigzag
  日期：2013.7
  内容：18B20单总线结构
------------------------------------------------*/
#include"18b20.h"
/*------------------------------------------------
                    18b20初始化
------------------------------------------------*/
_Bool  Init_DS18B20(void)
{
_Bool  dat=0;
 WD_PIN = 1;    //WD_PIN复位
DelayUS(5);   //稍做延时
 WD_PIN = 0;         //单片机将WD_PIN拉低
DelayUS(200); //精确延时 大于 480us 小于960us
DelayUS(200);
 WD_PIN = 1;        //拉高总线
DelayUS(50); //15~60us 后 接收60-240us的存在脉冲
 dat=WD_PIN;        //如果x=0则初始化成功, x=1则初始化失败
DelayUS(25); //稍作延时返回
 return dat;
}

/*------------------------------------------------
                    读取一个字节
------------------------------------------------*/
unsigned char ReadOneChar(void)
{
uchar i=0;
uchar dat = 0;
for (i=8;i>0;i--)
 {
  WD_PIN = 0; // 给脉冲信号
  dat>>=1;
  WD_PIN = 1; // 给脉冲信号
  if(WD_PIN)
   dat|=0x80;
 DelayUS(25);
 }
 return(dat);
}
/*------------------------------------------------
                    写入一个字节
------------------------------------------------*/
void WriteOneChar(uchar dat)
{
 uchar i=0;
 for (i=8; i>0; i--)
 {
  WD_PIN = 0;
  WD_PIN = dat&0x01;
 DelayUS(25);
  WD_PIN = 1;
  dat>>=1;
 }
DelayUS(25);
}

/*------------------------------------------------
                    读取温度
------------------------------------------------*/
uint ReadTemperature(void)
{

uchar a=0;
uint b=0;
uint t=0;
WD_OK=0;//温度值不可用

Init_DS18B20();
WriteOneChar(0xCC); // 跳过读序号列号的操作
WriteOneChar(0x44); // 启动温度转换
DelayMs(10);
Init_DS18B20();
WriteOneChar(0xCC); //跳过读序号列号的操作 
WriteOneChar(0xBE); //读取温度寄存器等（共可读9个寄存器） 前两个就是温度
a=ReadOneChar();   //低位
b=ReadOneChar();   //高位

b<<=8;
t=a+b;
WD_OK=1;//温度值可用
return(t);
}









