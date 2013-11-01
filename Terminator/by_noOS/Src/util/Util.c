/******************************************************************************
 *  Copyright Statement:
 *    Copyright (c)  Q.Bryan <qbing2010@163.com>
 *    All rights reserved.
 *-----------------------------------------------------------------------------
 * Filename:      Util.c
 * Description:   工具源文件
 * Author:        Q.Bryan	2013/06/30 16:36:15
 *=============================================================================
 * HISTORY
 *------Below this line--------------------------------------------------------
 *3.your_name datetime->add_your_change_for_this_file.
 *2.your_name datetime->add_your_change_for_this_file.
 *1.Q.Bryan 2013/06/30 16:36:15->create this file.
 *------Upper this line--------------------------------------------------------
 *=============================================================================
******************************************************************************/

#include <stdarg.h>
#include <driver.h>
#include "Util.h"

void putc(U8 byte)
{
    sendData(&byte,1);
}
void puts(U8 *string)
{
    int i=0;
    while(string[i])i++;
    sendData(string,i);
}

S8 mystrcmp(const char* str1,const char* str2,S8 len)
{
  if(len<=0){len = 128;}
  
  while(len>0  && *str1 == *str2 && *str1!='\0')
  {
    len--  ;
    str1++ ;
    str2++ ;
  }
  return *str1-*str2;
}

//return the end of str1+str2
U8* mystrcat(char* str1,char* str2)
{
  while(*str1)str1++;
  while(*str2)*str1++=*str2++;
  *str1='\0';
  return str1;
}

U8 myitoa(char* dest,long d,U8 r)
{
    char dest_temp[16];
    int i = 0;
    int j = -1;
    char digtal=0;
    if(d==0)
    {
        dest[i++]='0';
    }else if(d<0){
        dest[i++]='-';
        d=-d;
    }
    while(d){
        digtal = (d%r);
        dest_temp[++j] = (digtal<10)?digtal+'0':digtal+'A';
        d/=r;
    }
    while(j>=0)
    {
        dest[i++]=dest_temp[j--];
    }
    dest[i]='\0';
    return i;
}

S16 myprintf(char *fmt, ...){ 
    const char *str = fmt;  
    va_list args; 
    va_start(args, fmt);  
  
    while(*str != '\0') {                       //循环遍历格式字符串  
        if(*str != '%') {                       //遇到非'%'字符  
            putc(*str);  
            ++str;  
            continue;  
        } else {                                //遇到'%'  
            ++str;  
            switch(*str) {  
                case 'l': {                   //%ld  
                    if(*(str+1)=='d'){
                        break;
                    }else{
                        long val = va_arg(args, long);  
                        char res[16] = {0};  
                        myitoa(res,val, 10);  
                        puts(res); 
                    }										
                    break;
                }
                case 'd': {                     //%d  
                    int val = va_arg(args, int);  
                    char res[16] = {0};  
                    myitoa(res,val, 10);  
                    puts(res);  
                    break;  
                }  
                case 'x': {                     //%x  
                    int val = va_arg(args, int);  
                    char res[16] = {0};  
                    myitoa(res,val, 16);  
                    puts(res);  
                    break;  
                }  
                case 'c': {                     //%c  
                    char ch = va_arg(args, char);  
                    putc(ch);  
                    break;  
                }  
                case 's': {                     //%s  
                    char *res = va_arg(args, char*);  
                    puts(res);  
                    break;  
                }  
            }  
            ++str;  
        }  
    }  
    va_end(args);  
    return 0;  
}  

void delay(U16 y)
{
	U16 x;
	for(x=0;x<y;x++);
}


