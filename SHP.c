#include "SHP.h"

#if !defined(QB_UCOSII) && !defined(QB_ZIGBEE)

#ifdef	DBG
#undef	DBG
char _dbg_buffer___[4096];
#define DBG(fmt,...) do{\
	sprintf(_dbg_buffer___,fmt, __VA_ARGS__);\
	modis_debug(_dbg_buffer___);\
}while(0)
#endif

#elif defined(QB_UCOSII)
#ifdef	DBG
#undef	DBG
char _dbg_buffer___[128];
extern int tty_printf(U8 *str);
#define DBG(fmt,...) do{\
	sprintf(_dbg_buffer___,fmt, ##__VA_ARGS__);\
	tty_printf(_dbg_buffer___);\
}while(0)
#endif
#else
#ifdef	DBG
#undef	DBG
#define DBG
#endif
#endif

#if defined(QB_ZIGBEE)
#define memcpy osal_memcpy
#define memcmp osal_memcmp
#endif

/*****************************************************************************/
SHP_SYSTEM shp_sys;//The only instance about shp system!!!!!
SHP_SYSTEM* const THIS=&shp_sys;
/*****************************************************************************/

/*******************************************************************************
 * FUNCTION NAME:	shp_send_package
 * DESCRIPTION:	    Add head to send data ,then send it out!!!!
 * --------------------
 *    2013/09/17, Q.Bryan create this function
 ******************************************************************************/
S16 shp_send_package(S8 type,void* bin,S16 len)
{
	SHP_HeadForSend	head={SHP_MAGIC1,sizeof(SHP_HeadForSend),SHP_MAGIC2,'B',0};
	int ret = 0;
	head.pkg_len+=len;
	head.content_type = type;
	ret = shp_sys.send_data((U8*)&head,sizeof(head));
	ret = shp_sys.send_data(bin,len);
	return ret;
}

/*******************************************************************************
 * FUNCTION NAME:	shp_recv_buf_get_data
 * DESCRIPTION:	    Get valid recv buffer data pointer 
					You MUST use this POINTER immediately!!!
					When you used other shp_rcv_buf_* function,
					this POINTER may became invalid!!!!
 * --------------------
 *    2013/09/08, Q.Bryan create this function
 ******************************************************************************/
void shp_recv_buf_init(SHP_RECV_DATA_T* recv_buf)
{
	recv_buf->total_len = sizeof(recv_buf->data);
	recv_buf->start = 0;
	recv_buf->end = 0;
}

/*******************************************************************************
 * FUNCTION NAME:	shp_recv_buf_get_data
 * DESCRIPTION:	    Get valid recv buffer data pointer 
					You MUST use this POINTER immediately!!!
					When you used other shp_rcv_buf_* function,
					this POINTER may became invalid!!!!
 * --------------------
 *    2013/09/08, Q.Bryan create this function
 ******************************************************************************/
char* shp_recv_buf_get_data(SHP_RECV_DATA_T* recv_buf)
{
    return recv_buf->data+recv_buf->start;
}

/*******************************************************************************
 * FUNCTION NAME:	shp_recv_buf_put_data
 * DESCRIPTION:	    
 * --------------------
 *    2013/09/08, Q.Bryan create this function
 ******************************************************************************/
int shp_recv_buf_put_data(SHP_RECV_DATA_T* recv_buf,char* data,int size)
{   
  if(recv_buf->end+size<recv_buf->total_len){
    memcpy(recv_buf->data+recv_buf->end,data,size);
    recv_buf->end+=size;
  }else{

  }
}
/*******************************************************************************
 * FUNCTION NAME:	shp_recv_buf_get_lenght
 * DESCRIPTION:	    
 * --------------------
 *    2013/09/08, Q.Bryan create this function
 ******************************************************************************/
int shp_recv_buf_get_lenght(SHP_RECV_DATA_T* recv_buf)
{
    return recv_buf->end-recv_buf->start;
}

/*******************************************************************************
 * FUNCTION NAME:	shp_recv_buf_get_lenght
 * DESCRIPTION:	    
 * --------------------
 *    2013/09/08, Q.Bryan create this function
 ******************************************************************************/
int shp_recv_buf_set_used(SHP_RECV_DATA_T* recv_buf,int size)
{
    recv_buf->start+=size;
    if(recv_buf->start==recv_buf->end){
        recv_buf->start=0;
        recv_buf->end=0;
    }
}



/*******************************************************************************
 * FUNCTION NAME:	SetShpStae
 * DESCRIPTION:	    设置系统状态
 * --------------------
 *    2013/09/08, Q.Bryan create this function
 ******************************************************************************/
void SetShpStae(SHP_STATUS_T state)
{
    shp_sys.state = state;
}

/*******************************************************************************
 * FUNCTION NAME:	GetShpStae
 * DESCRIPTION:	    得到系统状态
 * --------------------
 *    2013/09/08, Q.Bryan create this function
 ******************************************************************************/
SHP_STATUS_T GetShpStae(void)
{
    return shp_sys.state;
}



/******************************************************************************
 * FUNCTION NAME:	shp_default_binary_data_handler
 * DESCRIPTION:	    handle the recived binary data 
 * PARAMETER:	    r_data :recived data struct
 * RETURNS:         used sized
 * modification history
 * --------------------
 *    2013/09/08, Q.Bryan create this function
 ******************************************************************************/
static S16 shp_default_consultation_handler(void* bin)
{
	SHP_CONSULT_PKG * consult = (SHP_CONSULT_PKG *)bin;
	if(consult->surport_or_not[0] != 'S'){
		DBG("Server NOT surpot this Device!!!!\r\n");
		SetShpStae(SHP_STATE_NOT_SURPORT);
	}

	if(SHP_STATE_POWERON==GetShpStae()){
		if(consult->surport_or_not[1] == '0'){
			shp_sys.send_system_topology();
			SetShpStae(SHP_STATE_CONSULTATION_S0);
		}else{
			DBG("Unkown Result!!!!\r\n");
		}
	}else if(SHP_STATE_CONSULTATION_S0==GetShpStae()){
		if(consult->surport_or_not[1] == '1'){
			SetShpStae(SHP_STATE_RUNNING);
		}else{
			DBG("Unkown Result!!!!\r\n");
		}
	}else{

	}

	return SHP_USED_SIZE_ALL;
}


typedef struct SHP_CALLBACK_ITEM
{
	DEV_ID		id;
	SHP_EXE_T	func;
	SHP_GET_T	get;
}SHP_CALLBACK_ITEM;

#define MAX_DEV_NUMS	16
typedef struct SHP_CALLBACK_LIST
{
	U16					registered_num;
	U16					max_num;
	SHP_CALLBACK_ITEM	callbacks[MAX_DEV_NUMS];
}SHP_CALLBACK_LIST;
SHP_CALLBACK_LIST callback_list = {0,MAX_DEV_NUMS};

SHP_RESULT_T shp_register_execution(DEV_ID dev_id,DEV_TYPE dev_type,SHP_EXE_T exe,SHP_GET_T get)
{
	int i=0;
	for(i=0;i<callback_list.registered_num;i++)
	{
		if(callback_list.callbacks[i].id == dev_id){
			callback_list.callbacks[i].func = exe;
			callback_list.callbacks[i].get = get;
			return SHP_OK;
		}
	}
	if(callback_list.registered_num<callback_list.max_num){
		callback_list.callbacks[callback_list.registered_num].id = dev_id;
		callback_list.callbacks[callback_list.registered_num].func = exe;
		callback_list.callbacks[callback_list.registered_num].get = get;
		callback_list.registered_num++;
		return SHP_OK;
	}else{
		return SHP_ERROR;
	}
}

SHP_RESULT_T shp_register_groud_dev_execution(DEV_ID* dev_ids,U16 dev_nums,SHP_EXE_T exe,SHP_GET_T get)
{
	int i = 0;
	if(dev_ids==NULL || dev_nums == 0 )return SHP_ERROR;

	for(i=0;i<dev_nums;i++)
	{
		shp_register_execution(dev_ids[i],0/*temp*/,exe,get);
	}
	return SHP_OK;
}

static SHP_CALLBACK_ITEM * shp_find_dev_exe(DEV_ID dev_id)
{
	int i=0;
	for(i=0;i<callback_list.registered_num;i++)
	{
		if(callback_list.callbacks[i].id == dev_id){
			return &callback_list.callbacks[i];
		}
	}
	return NULL;
}

/******************************************************************************
 * FUNCTION NAME:	shp_default_binary_data_handler
 * DESCRIPTION:	    handle the recived binary data 
 * PARAMETER:	    r_data :recived data struct
 * RETURNS:         used sized
 * modification history
 * --------------------
 *    2013/06/30, Q.Bryan create this function
 ******************************************************************************/
static S16 shp_default_binary_data_handler(void* bin)
{
	SHP_BIN_HEAD_T* bin_head = (SHP_BIN_HEAD_T*)bin;
	SHP_CALLBACK_ITEM * callback = NULL;
	switch(bin_head->pid)
	{
		case SHP_SWITCH_PID:/**/{
			SHP_SWITCH_CONTROL_T* switch_pkg = (SHP_SWITCH_CONTROL_T*)bin;
			callback = shp_find_dev_exe(switch_pkg->bin_head.dev_id);
			if(callback && callback->func){callback->func(switch_pkg->bin_head.dev_id,switch_pkg->bin_head.op_type,NULL);}
			break;
		}
		default:
			break;
	}
	return SHP_USED_SIZE_ALL;
}

/******************************************************************************
 * FUNCTION NAME:	shp_handle_buffer
 * DESCRIPTION:	    handle the recived data 
 * PARAMETER:	    r_data :recived data struct
 * RETURNS:         used sized
 * modification history
 * --------------------
 *    2013/06/30, Q.Bryan create this function
 ******************************************************************************/
static S16 shp_handle_buffer(const SHP_RECV_DATA_T* r_data)
{
	SHP_HeadForRecv* head = (SHP_HeadForRecv*)shp_recv_buf_get_data((SHP_RECV_DATA_T*)r_data);
	if(head->magic1 == SHP_MAGIC1){
		if(head->pkg_len <= shp_recv_buf_get_lenght((SHP_RECV_DATA_T*)r_data)){
			if(head->magic2 == SHP_MAGIC2)
			{
				int used = 0;
				DBG("handleing...\r\n");
				switch(head->content_type)
				{
				case 'B':
					//handle
					used = shp_default_binary_data_handler(((char*)head)+sizeof(SHP_HeadForRecv));
					break;
				case 'C':
					//Consultation
					used = shp_default_consultation_handler(((char*)head)+sizeof(SHP_HeadForRecv));
					break;
				default:
					//Unsurported content type
					DBG("Unsurported content type 0X%02X\r\n",head->content_type);
					return head->pkg_len;
				}
				DBG("handle success!!!!!!!!!!!\r\n");
				/////////////used size////////begin///////
				switch(used)
				{
				case SHP_USED_SIZE_ALL:
					return head->pkg_len;
				case SHP_USED_SIZE_ERROR:
					return SHP_USED_SIZE_ERROR;
				default:
					return used>0 ? used +sizeof(SHP_HeadForRecv):-1;
				}
				/////////////used size////////end/////////
			}else{
				//MAGIC2 is not right!!!!
				DBG("MAGIC2 is not right!!!!");
				return head->pkg_len;
			}
		}else{
			return 0;
		}
	}else if(shp_recv_buf_get_lenght((SHP_RECV_DATA_T*)r_data) >= sizeof(SHP_HeadForRecv)){
		DBG("data error!!! Skip one byte!!");
		return 1;//判定为出错，跳过一个字节
	}else{
		return 0;
	}
}



/******************************************************************************
 * FUNCTION NAME:	shp_send_system_tick_data
 * DESCRIPTION:	    Send the 9999 pid data, 
					keep center systerm know the status of this device !!!
 * --------------------
 *    2013/09/14, Q.Bryan create this function
 ******************************************************************************/
S16 shp_send_system_tick_data(void)
{
	S16		i=0;
	U16		j=0;
	char	device_data_buf[SHP_MAX_DEVICE_DATA_SIZE];
	S16		max_device_data_size = SHP_MAX_DEVICE_DATA_SIZE;
	S16		temp = 0;
	DBG("shp_send_system_tick_data...\r\n");
#if SHP_SEND_TICK_BY_BUFFER
	/*////////////BY BUFFER////////*/{
	int		used_size = 0;
	char	send_tick_buffer[SHP_SEND_TICK_BY_BUFFER_SIZE];
	temp = SHP_DEV_TICK_PID;//pid
	//pid
	memcpy(send_tick_buffer+used_size,&temp,2);
	used_size+=2;
	//mac
	memcpy(send_tick_buffer+used_size,"mactst",6);
	used_size+=6;
	//num
	memcpy(send_tick_buffer+used_size,&temp,2);
	used_size+=2;
	//data
	for(i=0,j=0;i<callback_list.registered_num;i++)
	{
		max_device_data_size = SHP_MAX_DEVICE_DATA_SIZE;
		if(callback_list.callbacks[i].get){
			callback_list.callbacks[i].get(callback_list.callbacks[i].id,device_data_buf,&max_device_data_size);
			if(max_device_data_size>0 && used_size+max_device_data_size<SHP_SEND_TICK_BY_BUFFER_SIZE){
				memcpy(send_tick_buffer+used_size,&callback_list.callbacks[i].id,2);
				used_size+=2;
				temp = max_device_data_size+4;
				memcpy(send_tick_buffer+used_size,&temp,2);
				used_size+=2;
				memcpy(send_tick_buffer+used_size,device_data_buf,max_device_data_size);
				used_size+=max_device_data_size;
				j++;
			}else{
				DBG("Warning: the send tick memory is not enough! skip some device!");
				break;
			}
		}
	}
	*((U16*)(send_tick_buffer+2)) = j;
	shp_send_package('B',send_tick_buffer,used_size);
	/*////////////BY BUFFER////////*/}
#else

#endif
	DBG("shp_send_system_tick_data.*DONE*\r\n");
}

#if SHP_CALL_shp_system_tick_PER_100MS
#define TICK_DELAY_COUNT	50
#else
#define TICK_DELAY_COUNT	3
#endif
S16 shp_system_tick(void)//typicly be called per second or per 100ms
{
	static U8 count = 0;
	
	if(count>0){count--;}

	switch(GetShpStae())
	{
	case SHP_STATE_POWERON:
		count = TICK_DELAY_COUNT;
		shp_sys.send_system_mac();
		break;
	default:
		break;
	}

	if(count==0){
		count = TICK_DELAY_COUNT;
		shp_send_system_tick_data();
	}
}


/******************************************************************************
 * FUNCTION NAME:	shp_system_init
 * DESCRIPTION:	    initalize the shp system,Must be called before 
					using any otherSHP function
 * modification history
 * --------------------
 *    2013/09/08, Q.Bryan create this function
 ******************************************************************************/
S16 shp_system_init(	
	S16 (*send_data)(U8* bin,U16 len),
	S16 (*send_system_mac)(void),
	S16 (*send_system_topology)(void)
)
{
	int i = 0;
	//init
	for(i=0;i<callback_list.max_num;i++){
		callback_list.callbacks[i].id = -1;
		callback_list.callbacks[i].get = NULL;
		callback_list.callbacks[i].func = NULL;
	}

	//memset
	memset((U8*)&shp_sys,0,sizeof(shp_sys));

	//init buffer
	shp_recv_buf_init(&shp_sys.buf);

	shp_sys.state = SHP_STATE_POWERON;
	shp_sys.send_data = send_data;
	shp_sys.send_system_mac = send_system_mac;//==NULL?shp_defult_send_system_mac:send_system_mac;
	shp_sys.send_system_topology = send_system_topology;//==NULL?shp_defult_send_send_system_topology:send_system_topology;


}

/******************************************************************************
 * FUNCTION NAME:	shp_handle_recviced_data
 * DESCRIPTION:	    This function handle the recived data from Networt or UART 
					or SPI or Bluetooth or somewhere else!!!!!
					You MUST call this fuction when data arrived!!!!!!!
 * modification history
 * --------------------
 *    2013/09/08, Q.Bryan create this function
 ******************************************************************************/
S16 shp_handle_recviced_data(U8* bin,U16 len)
{
	int used = 0;
	DBG_MEM(bin,len);

	shp_recv_buf_put_data(&shp_sys.buf,bin,len);
	
	/*/////handle///////begin///*/{
	do{
		DBG("There are %d Bytes Data want to handle!!!!\r\n",shp_recv_buf_get_lenght(&shp_sys.buf));
		used = shp_handle_buffer(&shp_sys.buf);
		DBG("handle_recieved_buffer Used %d Bytes Data\r\n",used);
		if(used>0){
			shp_recv_buf_set_used(&shp_sys.buf,used);
		}else if(used<0){
			shp_recv_buf_set_used(&shp_sys.buf,shp_recv_buf_get_lenght(&shp_sys.buf));
		}
	}while(used>0 && shp_recv_buf_get_lenght(&shp_sys.buf)>0);
	/*/////handle///////end/////*/}
	return len;
}






