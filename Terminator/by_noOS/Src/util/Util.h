/******************************************************************************
 *  Copyright Statement:
 *    Copyright (c)  Q.Bryan <qbing2010@163.com>
 *    All rights reserved.
 *-----------------------------------------------------------------------------
 * Filename:      Util.h
 * Description:   工具头文件
 * Author:        Q.Bryan	2013/06/30 16:36:37
 *=============================================================================
 * HISTORY
 *------Below this line--------------------------------------------------------
 *3.your_name datetime->add_your_change_for_this_file.
 *2.your_name datetime->add_your_change_for_this_file.
 *1.Q.Bryan 2013/06/30 16:36:37->create this file.
 *------Upper this line--------------------------------------------------------
 *=============================================================================
******************************************************************************/

#ifndef __QBING_UTIL_20130630_H_
#define __QBING_UTIL_20130630_H_

extern void delay(U16 clk);
extern void putc(U8 byte);
extern void puts(U8 *string);
extern S16  myprintf(char *fmt, ...);
extern void sendData(U8* data,U8 size);

extern U8* mystrcat(char* str1,char* str2);
extern U8 myitoa(char* dest,long d,U8 r);

#define ASSERT

#if 1//def DEBUG
#define printf
#define dbg
#define per500ms_dbg
#define per100ms_dbg
#define per1000ms_dbg
#define per5000ms_dbg
#else
#define QB_DEBUG
#define QB_DEBUG_RECORD 1
#define CLOSE_TEMPERATER_TEST_AND_OFEN_UART
#define printf myprintf
#define dbg myprintf
#define per5000ms_dbg	if(system.time.ms%5000==0)myprintf
#define per1000ms_dbg	if(system.time.ms%1000==0)myprintf
#define per500ms_dbg	if(system.time.ms%500==0)myprintf
#define per100ms_dbg	if(system.time.ms%100==0)myprintf
#endif


#endif//__QBING_UTIL_20130630_H_