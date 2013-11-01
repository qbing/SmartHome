/******************************************************************************
 *  Copyright Statement:
 *    Copyright (c)  Q.Bryan <qbing2010@163.com>
 *    All rights reserved.
 *-----------------------------------------------------------------------------
 * Filename:      framework.h
 * Description:   智能家居框架头文件
 * Author:        Q.Bryan	2013/06/30 15:32:40
 *=============================================================================
 * HISTORY
 *------Below this line--------------------------------------------------------
 *3.your_name datetime->add_your_change_for_this_file.
 *2.your_name datetime->add_your_change_for_this_file.
 *1.Q.Bryan 2013/06/30 15:32:40->create this file.
 *------Upper this line--------------------------------------------------------
 *=============================================================================
******************************************************************************/

#ifndef __FRAMEWORK_20130628_H_
#define __FRAMEWORK_20130628_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ASSERT
#define ASSERT
#endif
	
#pragma pack(push,1) //表示已1字节对齐  

typedef enum PRG_STATE
{
    PRG_STATE_POWERON,          //启动
    PRG_STATE_CONSULTATION,     //协商
    PRG_STATE_RUNNING,          //正在工作
    PRG_STATE_CLOSING,          //关机中
    PRG_STATE_CLOSED            //已关机
}PRG_STATE;

#if 0
typedef enum DEV_STATE{
  IS_CLOSED,
  IS_OPEN
}DEV_STATE;
#elif defined(WIN32)
typedef char DEV_STATE;
typedef  unsigned char 		U8;
typedef  signed char 		S8;
typedef  unsigned short		U16;
typedef  signed short 		S16;
typedef  unsigned int		U32;
typedef  signed int			S32;

typedef U8 DEVICE_ID;
#define   ID_LAMB1		= 0;
#define   ID_LAMB2		= 1;
#define   ID_CDPLAYER	= 2;
#define   ID_HEATER		= 3;
#define   ID_INDICATOR	= 4;//连接指示灯

#if 0
typedef U8 OPERTATING_TYPE;
typedef U8 OP_TYPE;
#define   OP_OPEN		= 0;
#define   OP_CLOSE		= 1;
#define   OP_NOT		= 2;
#define   OP_SET		= 3;
#endif

#include "..\..\Terminator\by_noOS\Src\util\communicate.h"

#elif QB_UCOSII
//typedef char DEV_STATE;
typedef  unsigned char 		U8;
typedef  signed char 		S8;
typedef  unsigned short		U16;
typedef  signed short 		S16;
typedef  unsigned long		U32;
typedef  signed long			S32;

typedef enum DEVICE_ID{
  ID_LAMB1,
  ID_LAMB2,
  ID_CDPLAYER,
  ID_HEATER,
  ID_INDICATOR//连接指示灯
}DEVICE_ID;

typedef enum OPERTATING_TYPE{
  OP_OPEN,
  OP_CLOSE,
  OP_NOT,
  OP_SET
}OP_TYPE;

typedef _Bool DEV_STATE;
//#include "driver.h"
//#include "util.h"
#include "communicate.h"

#else

typedef enum DEVICE_ID{
  ID_LAMB1,
  ID_LAMB2,
  ID_CDPLAYER,
  ID_HEATER,
  ID_INDICATOR//连接指示灯
}DEVICE_ID;

typedef enum OPERTATING_TYPE{
  OP_OPEN,
  OP_CLOSE,
  OP_NOT,
  OP_SET
}OP_TYPE;

typedef _Bool DEV_STATE;
#include "driver.h"
#include "util.h"
#include "communicate.h"
#endif


/*******************************************************************************
 * FUNCTION NAME:	SetProgrameStae
 * DESCRIPTION:	    设置系统状态
 * --------------------
 *    2013/08/22, Q.Bryan create this function
 ******************************************************************************/
extern void SetProgrameStae(PRG_STATE state);

/*******************************************************************************
 * FUNCTION NAME:	SetProgrameStae
 * DESCRIPTION:	    设置系统状态
 * --------------------
 *    2013/08/22, Q.Bryan create this function
 ******************************************************************************/
extern PRG_STATE GetProgrameStae(void);

extern void init(void);
extern U8 recived_data_handler(RECV_DATA_T* r_data);


#pragma pack(pop) 
#ifdef __cplusplus
}
#endif
#endif//__FRAMEWORK_20130628_H_
