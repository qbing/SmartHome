/******************************************************************************
 *  Copyright Statement:
 *    Copyright (c)  Q.Bryan <qbing2010@163.com>
 *    All rights reserved.
 *-----------------------------------------------------------------------------
 * Filename:      communicate.c
 * Description:   通信处理
 * Author:        Q.Bryan	2013/06/30 16:35:29
 *=============================================================================
 * HISTORY
 *------Below this line--------------------------------------------------------
 *3.your_name datetime->add_your_change_for_this_file.
 *2.your_name datetime->add_your_change_for_this_file.
 *1.Q.Bryan 2013/06/30 16:35:29->create this file.
 *------Upper this line--------------------------------------------------------
 *=============================================================================
******************************************************************************/


#if 1


#ifdef WIN32
#include "..\..\Terminator\by_noOS\Src\framework\framework.h"
#else
#include "framework.h"
#endif//WIN32

///////////////////////////////////////临时代码///////////////////////////////////begin////////////
System sys = {{'Q',sizeof(sys),'B','B',0}};
extern "C" void operate(DEVICE_ID dev_id,U8 op_type,void* op_data);




/******************************************************************************
 * FUNCTION NAME:	consultation_handler
 * DESCRIPTION:	    consultation with server
 * PARAMETER:	    r_data :recived data struct
 * RETURNS:         used sized
 * modification history
 * --------------------
 *    2013/08/22, Q.Bryan create this function
 ******************************************************************************/
U8 consultation_handler(void* bin)
{
	CMD_PKG* cmd_ptr = (CMD_PKG*)bin;
	operate(cmd_ptr->dev_id,cmd_ptr->op_type,0);
	return sizeof(CMD_PKG);			
}

/******************************************************************************
 * FUNCTION NAME:	binary_data_handler
 * DESCRIPTION:	    handle the recived binary data 
 * PARAMETER:	    r_data :recived data struct
 * RETURNS:         used sized
 * modification history
 * --------------------
 *    2013/06/30, Q.Bryan create this function
 ******************************************************************************/
U8 binary_data_handler(void* bin)
{
	CMD_PKG* cmd_ptr = (CMD_PKG*)bin;
	operate(cmd_ptr->dev_id,cmd_ptr->op_type,0);
	return sizeof(CMD_PKG);			
}

/******************************************************************************
 * FUNCTION NAME:	recived_data_handler
 * DESCRIPTION:	    handle the recived data 
 * PARAMETER:	    r_data :recived data struct
 * RETURNS:         used sized
 * modification history
 * --------------------
 *    2013/06/30, Q.Bryan create this function
 ******************************************************************************/
U8 recived_data_handler(RECV_DATA_T* r_data)
{
	HeadForRecv* head = (HeadForRecv*)r_data->recv_buffer;
	if(head->magic1 == MAGIC1){
		if(head->pkg_len <= r_data->recved_size){
			if(head->magic2 == MAGIC2)
			{
			    if(GetProgrameStae()==PRG_STATE_CONSULTATION)
                {
                    switch(head->content_type)
                    {
        				case 'C':
        					//Consultation
        					return consultation_handler(r_data->recv_buffer+sizeof(HeadForRecv))+sizeof(HeadForRecv);
        				default:
        					//Unsurported content type
        					DBG("Unsurported content type 0X%02X when consultatin!",head->content_type);
        					return head->pkg_len;
                    }
                }else
                {
    				switch(head->content_type)
    				{
    				case 'B':
    					//handle
    					return binary_data_handler(r_data->recv_buffer+sizeof(HeadForRecv))+sizeof(HeadForRecv);
    				case 'C':
    					//Consultation
    					DBG("Unsurported content type 'C' when not consultatin!");
    					return head->pkg_len;
    				default:
    					//Unsurported content type
    					DBG("Unsurported content type 0X%02X!",head->content_type);
    					return head->pkg_len;
    				}
                }
			}else{
				//MAGIC2 is not right!!!!
				DBG("MAGIC2 is not right!!!!");
				return head->pkg_len;
			}
		}else{
			return 0;
		}
	}else if(r_data->recved_size >= sizeof(HeadForRecv)){
		DBG("data error!!! Skip one byte!!");
		return 1;//判定为出错，跳过一个字节
	}else{
		return 0;
	}
}


/******************************************************************************
 * FUNCTION NAME:	SendPkgData
 * DESCRIPTION:	    发送数据包
 * --------------------
 *    2013/08/22, Q.Bryan create this function
 ******************************************************************************/
extern "C"	void sendData(U8* data,U16 size);
void SendPkgData(S8 type,U8* data,U16 len)
{
    HeadForSend head = {'Q',sizeof(head),'B','B',0};
    head.pkg_len = sizeof(head) + len;
    sendData((U8*)&head,sizeof(head));
    sendData(data,len);
}

#endif