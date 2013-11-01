#include "SHP_example.h"



/*****************************************************************************************
|--head--|-----ID----------|--device type--|--device name--|---p version--|--surported p--|
|-6Bytes-|----48bits MAC---|-----2Bytes----|---32Bytes-----|---1Bytes-----|---3Bytes------|
*****************************************************************************************/
const static struct
{
	SHP_HeadForSend head;
	U16				pid;
	U8  mac[6];
	U16 dev_type;
	S8  dev_name[32];
	U8  version;
	U8  surpoted_protocl[3];
}
SHP_EXAMPLE_MAC_PKG = {/*Please view the document!!!!!!!!!!!*/
	{SHP_MAGIC1,sizeof(SHP_EXAMPLE_MAC_PKG),SHP_MAGIC2,'C',0},	/*head*/
	0x0001,				/*PID*/
	"INVMAC",			/*MAX : invalid mac address*/
	0,					/*device type : Demo*/
	"Modis",			/*device name : Modis*/
	0x01,				/*version : 0.1*/
	"B--"
};

#if 0
#define DEVICE_INFO \
"{\
    \"NAME\": \"SmartHeater\",\
    \"MAC\": \"ZJU003\",\
    \"TYPE\": \"DEV\",\
    \"CHILD\": [\
        {\
            \"NAME\": \"Light\",\
            \"TYPE\": \"SET\",\
            \"CHILD\": [\
                {\
                    \"NAME\": \"ReadLight\",\
                    \"TYPE\": \"TERM\",\
                    \"DEVTYPE\": \"SWITCH\"\
                },\
                {\
                    \"NAME\": \"DecorateLight\",\
                    \"TYPE\": \"TERM\",\
                    \"DEVTYPE\": \"SWITCH\"\
                }\
            ]\
        },\
        {\
            \"NAME\": \"CDPlayer\",\
            \"TYPE\": \"TERM\",\
            \"DEVTYPE\": \"SWITCH\"\
        }\
    ]\
}"
#elif 1//defined QB_UCOSII
#define DEVICE_INFO "{\"NAME\":\"SmartHeater\",\"MAC\":\""SHP_MAC_TEMP"\",\"TYPE\":\"DEV\",\"CHILD\":[{\"NAME\":\"Light\",\"TYPE\":\"SET\",\"CHILD\":[{\"NAME\":\"ReadLight\",\"TYPE\":\"TERM\",\"DEV_ID\":0,\"DEVTYPE\":\"SWITCH\"},{\"NAME\":\"DecorateLight\",\"TYPE\":\"TERM\",\"DEV_ID\":1,\"DEVTYPE\":\"SWITCH\"}]},{\"NAME\":\"CDPlayer\",\"TYPE\":\"TERM\",\"DEV_ID\":2,\"DEVTYPE\":\"SWITCH\"},{\"NAME\":\"Temperature\",\"TYPE\":\"TERM\",\"DEV_ID\":3,\"DEVTYPE\":\"VARIABLE\"}]}"
#endif
/*****************************************************************************************
|--head--|----JSON----|
|-6Bytes-|----XXXX----|
*****************************************************************************************/
const static struct
{
	SHP_HeadForSend head;
	U16				pid;
	U8  topology[sizeof(DEVICE_INFO)];
}
SHP_EXAMPLE_TOPOLOGY_PKG = {/*Please view the document!!!!!!!!!!!*/
	{SHP_MAGIC1,8+sizeof(DEVICE_INFO),SHP_MAGIC2,'C',0},	/*head*/
	0x0003,
	DEVICE_INFO
};




static S16 shp_example_send_data(U8* bin,U16 len)
{
	extern void sendData(U8* bin,U16 size);
	sendData(bin,len);
}

static S16 shp_example_send_system_mac(void)
{
	shp_example_send_data((U8*)&SHP_EXAMPLE_MAC_PKG,sizeof(SHP_EXAMPLE_MAC_PKG));
}

static S16 shp_example_send_system_topology(void)
{
	shp_example_send_data((U8*)&SHP_EXAMPLE_TOPOLOGY_PKG,sizeof(SHP_EXAMPLE_TOPOLOGY_PKG));
}

U8 lamb1_state = 0;
SHP_RESULT_T shp_example_lamp1_operator(DEV_ID dev_id,OP_TYPE op_type,void *bin)
{
	extern void operate(char dev_id,char op_type,void* op_data);
	operate(dev_id,op_type,0);
	lamb1_state = op_type;
	return SHP_OK;
}

SHP_RESULT_T shp_example_lamp1_get_state(DEV_ID dev_id,void* out_buf,U16* max_size_AND_out_size)
{
	if(*max_size_AND_out_size>=1){
		*((char*)out_buf) = lamb1_state;
		*max_size_AND_out_size = 1;
		return SHP_OK;
	}else{
		return SHP_ERROR;
	}
}


U8 lamb2_state = 0;
U8 lamb3_state = 0;
SHP_RESULT_T shp_example_lamps_operator(DEV_ID dev_id,OP_TYPE op_type,void *bin)
{
	extern void operate(char dev_id,char op_type,void* op_data);
	switch(dev_id)
	{
	case 1:
		operate(dev_id,op_type,0);
		lamb2_state = op_type;
		break;
	case 2:
		operate(dev_id,op_type,0);
		lamb3_state = op_type;
		break;
	}
	return SHP_OK;
}

SHP_RESULT_T shp_example_lamps_get_state(DEV_ID dev_id,void* out_buf,U16* max_size_AND_out_size)
{
	if(*max_size_AND_out_size>=1){
		*max_size_AND_out_size = 1;
		switch(dev_id)
		{
		case 1:
			*((char*)out_buf) = lamb2_state;
			break;
		case 2:
			*((char*)out_buf) = lamb3_state;
			break;
		}
		return SHP_OK;
	}else{
		return SHP_ERROR;
	}
}



U32 temperature = 25;
SHP_RESULT_T shp_example_temperature_operator(DEV_ID dev_id,OP_TYPE op_type,void *bin)
{
	return SHP_OK;
}
SHP_RESULT_T shp_example_temperature_get_state(DEV_ID dev_id,void* out_buf,U16* max_size_AND_out_size)
{
	if(*max_size_AND_out_size>=4)
	{
		#ifdef WIN32
		extern void updata_temperature(void);
		updata_temperature();
		#endif
		*max_size_AND_out_size = 4;
		*((U32*)out_buf) = temperature;
		return SHP_OK;
	}
	return SHP_ERROR;
}

void shp_example_init_called_by_system(void)
{
	U16 dev_ids[] = {1,2};

	shp_system_init(shp_example_send_data,shp_example_send_system_mac,shp_example_send_system_topology);
	shp_register_execution(0,SHP_SWITCH_PID,shp_example_lamp1_operator,shp_example_lamp1_get_state);
	shp_register_groud_dev_execution(dev_ids,sizeof(dev_ids)/sizeof(dev_ids[0]),shp_example_lamps_operator,shp_example_lamps_get_state);
	shp_register_execution(3,SHP_VARIABLE_PID,shp_example_temperature_operator,shp_example_temperature_get_state);
}

void shp_example_tick_called_by_system(void)
{
	shp_system_tick();
}

void shp_example_data_arrived_called_by_system(U8* bin,U16 len)
{
	shp_handle_recviced_data(bin,len);
}

#ifdef WIN32
#undef DBG
#include <Windows.h>
__int64 CompareFileTimeXXX ( FILETIME time1, FILETIME time2 )
{
       __int64 a = time1.dwHighDateTime << 32 | time1.dwLowDateTime ;
       __int64 b = time2.dwHighDateTime << 32 | time2.dwLowDateTime ;

       return   (b - a);
}
extern void disp_temperature(int temp);
void updata_temperature(void){
/******get cpu performance*******begin***/{
	BOOL res ;
 
	static FILETIME preidleTime;
	static FILETIME prekernelTime;
	static FILETIME preuserTime;

	FILETIME idleTime;
	FILETIME kernelTime;
	FILETIME userTime;


	res = GetSystemTimes( &idleTime, &kernelTime, &userTime );
	{
	__int64 idle = CompareFileTimeXXX(preidleTime,idleTime);
	__int64 kernel = CompareFileTimeXXX( prekernelTime, kernelTime);
	__int64 user = CompareFileTimeXXX(preuserTime, userTime);

	if((kernel+user)>0){
		__int64 cpu = (kernel +user - idle) *100/(kernel+user);
		temperature = cpu;
		disp_temperature(temperature);
	}
	}
/******get cpu performance*******end*****/}
}
#endif//WIN32
