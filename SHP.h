#ifndef _ZJU_SHP_H__
#define _ZJU_SHP_H__
#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push,1) //表示已字节对齐

#include "SHP_CONFIG.h"


#ifndef DBG
#define DBG
#endif

#define DBG_BREIF

#ifndef ASSERT
#define ASSERT(exp) do {if(!(exp)){DBG("\r\nASSERT:"#exp"\r\n");while(1);}}while(0)
#endif

#ifndef DBG_MEM
#define DBG_MEM
#endif

#ifndef NULL
#define NULL    0
#endif

#if 0//def COMMON_TYPE

#elif defined WIN32
typedef  unsigned char 		U8;
typedef  signed char 		S8;
typedef  unsigned short		U16;
typedef  signed short 		S16;
typedef  unsigned int		U32;
typedef  signed int			S32;

#elif defined __GNUC__
typedef  unsigned char 		U8;
typedef  signed char 		S8;
typedef  unsigned short		U16;
typedef  signed short 		S16;
typedef  unsigned int		U32;
typedef  signed int			S32;

#elif defined __C51__
typedef  unsigned char 		U8;
typedef  signed char 		S8;
typedef  unsigned short		U16;
typedef  signed short 		S16;
typedef  unsigned long		U32;
typedef  signed long		S32;
#else
typedef  unsigned char 		U8;
typedef  signed char 		S8;
typedef  unsigned short		U16;
typedef  signed short 		S16;
typedef  unsigned long		U32;
typedef  signed long		S32;
#endif

typedef enum SHP_STATUS_T
{
    SHP_STATE_POWERON,          //启动
    SHP_STATE_CONSULTATION_S0,  //协商
    SHP_STATE_CONSULTATION_S1,	//协商
    SHP_STATE_RUNNING,          //正在工作
    SHP_STATE_CLOSING,          //关机中
    SHP_STATE_CLOSED,           //已关机
    SHP_STATE_NOT_SURPORT       //已关机
}SHP_STATUS_T;


typedef enum SHP_SVR_STATUS_T
{
    SHP_SVR_STATE_POWERON,          //启动
    SHP_SVR_STATE_CONSULTATION_S0,  //协商
    SHP_SVR_STATE_CONSULTATION_S1,	//协商
    SHP_SVR_STATE_RUNNING,          //正在工作
    SHP_SVR_STATE_CLOSING,				//关机中
    SHP_SVR_STATE_CLOSED,				//已关机
}SHP_SVR_STATUS_T;

typedef enum SHP_PID
{
	SHP_CONSULT_S0_PID      = 0xfff0,
	SHP_CONSULT_S1_PID      = 0x0001,
	SHP_CONSULT_S2_PID      = 0x0002,
	SHP_CONSULT_S3_PID      = 0x0003,
	SHP_CONSULT_S4_PID      = 0x0004,
	SHP_SWITCH_PID          = 1001,
	SHP_T_SENSOR_PID        = 1002,
	SHP_G_SENSOR_PID        = 1003,
	SHP_VARIABLE_PID        = 1004,
	SHP_KEYBOARD_PID        = 1005,
	SHP_CAMERA_PID          = 1006,
	SHP_CUSTOM_PID          = 8000,
	SHP_DEV_TICK_PID        = 9999
}SHP_PID;


typedef enum SHP_RESULT_T
{
	SHP_OK,
	SHP_ERROR,
}SHP_RESULT_T;

#pragma pack(push,1)

#define SHP_MAGIC1	'Q'
#define SHP_MAGIC2	'B'
typedef struct SHP_HeadForSend
{
  U8            magic1;
  U16           pkg_len;
  U8            magic2;  
  U8			content_type;
  U8			reserve;
}SHP_HeadForSend,SHP_HeadForRecv;

typedef struct SHP_BIN_HEAD_T
{
	U16 pid;
	U16 dev_id;
	U8  op_type;
	U16 path_size;
}SHP_BIN_HEAD_T;


typedef struct SHP_CONSULT_PKG
{/*Please view the document!!!!!!!!!!!*/
	U16 pid;
	U8  mac[6];
	U8  surport_or_not[2];
	U16 reserved;
/*Please view the document!!!!!!!!!!!*/
}SHP_CONSULT_S0_PKG,SHP_CONSULT_PKG;


//SHP_SWITCH_PID
typedef struct SHP_SWITCH_CONTROL_T
{
    SHP_BIN_HEAD_T  bin_head;
    U16             reserved;
}SHP_SWITCH_CONTROL_T;


#define SHP_MAX_RECV_BUFFER_SIZE    96
typedef struct SHP_RECV_DATA_T{
    char    data[SHP_MAX_RECV_BUFFER_SIZE];
    int     total_len;
    int     start;
    int     end;
}SHP_RECV_DATA_T;


#if 0
typedef U16 DEV_ID;
typedef U8  DEV_TYPE;
#else
typedef U16		DEV_ID;
typedef SHP_PID	DEV_TYPE;
#endif

//打开	1	OPEN	OPEN
//关闭	0	CLOSE	CLOSE
//读取	2	READ	READ
typedef enum SHP_OP_TYPE
{
    SHP_OP_CLOSE,
    SHP_OP_OPEN,
    SHP_OP_READ,
	SHP_OP_NOT,
	SHP_OP_SET,
}SHP_OP_TYPE,OP_TYPE;


SHP_RESULT_T shp_set_device_infr(const U8* system_topology);



S16 shp_send_package(S8 type/*'C' or 'B' or 'J' or 'X'*/,void* bin,S16 len);

typedef enum SHP_USED_SIZE
{
    SHP_USED_SIZE_ALL       = -1,
    SHP_USED_SIZE_ERROR     = -2
}SHP_USED_SIZE;



typedef SHP_RESULT_T (*SHP_EXE_T)(DEV_ID dev_id,OP_TYPE op_type,void *data);
typedef SHP_RESULT_T (*SHP_GET_T)(DEV_ID dev_id,void *out_buf,U16* max_size_AND_out_size);

typedef struct SHP_SYSTEM
{
	//////////////////SHP System Value//////////////////////begin////////////////////////////////
	SHP_STATUS_T		state;
	SHP_RECV_DATA_T		buf;
	//////////////////SHP System Value//////////////////////end//////////////////////////////////

	//////////////The function which should be implimente at other place/////////begin///////////
	S16 (*send_data)(U8* bin,U16 len);
	S16 (*send_system_mac)(void);
	S16 (*send_system_topology)(void);
	//////////////The function which should be implimente at other place/////////begin///////////
}SHP_SYSTEM;

//////////////The function which should be called at other place/////////begin///////////
//called before use any SHP function
S16 shp_system_init(S16 (*send_data)(U8* bin,U16 len),S16 (*send_system_mac)(void),	S16 (*send_system_topology)(void));
//Register your device operation after called init func
SHP_RESULT_T shp_register_execution(DEV_ID dev_id,DEV_TYPE dev_type,SHP_EXE_T exe,SHP_GET_T get);
SHP_RESULT_T shp_register_groud_dev_execution(DEV_ID* dev_ids,U16 dev_nums,SHP_EXE_T exe,SHP_GET_T get);
//typicly be called per second or 100ms
S16 shp_system_tick(void);
//You MUST call this fuction when data arrived!!!!!!!
S16 shp_handle_recviced_data(U8* bin,U16 len);
//////////////The function which should be called at other place/////////begin///////////



/////////////JUST for TEST////////////////begin///////////
typedef struct CDeviceTest_TTTTTT
{
	char* s0_bin;
	int   s0_len;
	char* s1_bin;
	int   s1_len;
}CDeviceTest_TTTTTT;
CDeviceTest_TTTTTT* getTest(int index);
/////////////JUST for TEST////////////////end/////////////

#ifdef __cplusplus
}
#endif

#endif
