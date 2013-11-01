#ifndef __COMMUNICATE_H__
#define __COMMUNICATE_H__
#pragma pack(push,1) //表示已1字节对齐  


#include "framework.h"

#ifndef DBG
#define DBG
#endif

#ifndef DBG_MEM
#ifdef __cplusplus
extern "C" 
#endif
void shp_dbg_mem(char *data, int len);
#define DBG_MEM	shp_dbg_mem
#endif

#define MAX_RECV_BUFFER_SIZE    32
typedef struct RECV_DATA_T{
  U8 recv_buffer[MAX_RECV_BUFFER_SIZE];
  U8 recved_size;
}RECV_DATA_T;
typedef U8 (*RECV_HANDLER_T)(RECV_DATA_T* r_data);

#define MAGIC1	'Q'
#define MAGIC2	'B'
typedef struct HeadForSend
{
  U8            magic1;
  U16           pkg_len;
  U8            magic2;  
  U8			content_type;
  U8			reserve;
}HeadForSend,HeadForRecv;

#ifdef WIN32
typedef struct System{
  HeadForSend   head;
  S16           current_temp;//当前温度
  DEV_STATE     lamb1_state:1;
  DEV_STATE     lamb2_state:1;
  DEV_STATE     cdplayer_state:1;
}System;
#else
typedef struct System{
  HeadForSend   head;
  S16           current_temp;//当前温度
  DEV_STATE     lamb1_state;
  DEV_STATE     lamb2_state;
  DEV_STATE     cdplayer_state;
}System;

extern System sys;
#endif//WIN32



typedef struct CMD_PKG{
  U8			op_type;
  U16			dev_id;
  U8			op_data;
}CMD_PKG;


/******************************************************************************
 * FUNCTION NAME:	SendPkgData
 * DESCRIPTION:	    发送数据包
 * --------------------
 *    2013/08/22, Q.Bryan create this function
 ******************************************************************************/
void SendPkgData(S8 type,U8* data,U16 len);


#pragma pack(pop) 
#endif//__COMMUNICATE_H__
