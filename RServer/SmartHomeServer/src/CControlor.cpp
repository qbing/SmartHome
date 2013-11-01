/******************************************************************************
 *  Copyright Statement:
 *    Copyright (c)  Q.Bryan <qbing2010@163.com>
 *    All rights reserved.
 *-----------------------------------------------------------------------------
 * Filename:      CControlorConnection.cpp
 * Description:   Controlor class
 * Author:        QBryan	2013/09/10 23:23:13
 *=============================================================================
 * HISTORY
 *------Below this line--------------------------------------------------------
 *3.your_name datetime->add_your_change_for_this_file.
 *2.your_name datetime->add_your_change_for_this_file.
 *1.QBryan 2013/09/10 23:23:13->create this file.
 *------Upper this line--------------------------------------------------------
 *=============================================================================
******************************************************************************/

#include "SmartHomeInfr.h"


/******************************************************************************
 * FUNCTION NAME:	handle_control_data
 * DESCRIPTION:	    This function handle the recived data from Networt or UART 
					or SPI or Bluetooth or somewhere else!!!!!
					You MUST call this fuction when data arrived!!!!!!!
 * modification history
 * --------------------
 *    2013/09/08, Q.Bryan create this function
 ******************************************************************************/
int CControlorConnection::handle_control_data(U8* bin,U16 len)
{
	int used = 0;
	buf.PutData(bin,len);
	
	/*/////handle///////begin///*/{
	do{
		used = handle_control_buffer(buf);
		DBG("Used %d Bytes Data\r\n",used);
		if(used>0){
			buf.SetUsed(used);
		}else if(used<0){
			buf.SetUsed(buf.GetLengh());
		}
	}while(used>0 && buf.GetLengh()>0);
	/*/////handle///////end/////*/}
	return len;
}

/******************************************************************************
 * FUNCTION NAME:	handle_control_buffer
 * --------------------
 *    2013/09/08, Q.Bryan create this function
 ******************************************************************************/
int CControlorConnection::handle_control_buffer(CDataBuffer& buf)
{
	SHP_HeadForRecv* head = (SHP_HeadForRecv*)buf.GetData();
	shp_dbg_mem((char*)head,buf.GetLengh());
	DBG("SHP_MAGIC1=%c head->magic1=%c\r\n",SHP_MAGIC1,head->magic1);
	if(head->magic1 == SHP_MAGIC1){
		if(head->pkg_len <= buf.GetLengh()){
			if(head->magic2 == SHP_MAGIC2)
			{
				int used = 0;
				DBG("handleing...\r\n");
				switch(head->content_type)
				{
				case 'B':
					//handle
					used = shp_handle_binary_data(((char*)head)+sizeof(SHP_HeadForRecv),head->pkg_len+sizeof(SHP_HeadForRecv));
					break;
				case 'J':
					//handle
					used = shp_handle_json_data(((char*)head)+sizeof(SHP_HeadForRecv),head->pkg_len+sizeof(SHP_HeadForRecv));
					break;
				case 'X':
					//handle
					used = shp_handle_xml_data(((char*)head)+sizeof(SHP_HeadForRecv),head->pkg_len+sizeof(SHP_HeadForRecv));
					break;
				case 'C':
					//Consultation
					used = shp_handle_consult_data(((char*)head)+sizeof(SHP_HeadForRecv),head->pkg_len+sizeof(SHP_HeadForRecv));
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
				DBG("MAGIC2 is not right!!!!\r\n");
				return head->pkg_len;
			}
		}else{
			DBG("too few data to handle!!\r\n");
			return 0;
		}
	}else if(buf.GetLengh() >= sizeof(SHP_HeadForRecv)){
		DBG("MAGIC1 is not right or Data error!!! Skip one byte!!\r\n");
		return 1;//判定为出错，跳过一个字节
	}else{
		DBG("MAGIC1 is not right!!!!\r\n");
		return 0;
	}
}


int  CControlorConnection::shp_handle_consult_data(void* bin,int size)
{
	return SHP_USED_SIZE_ALL;
}


/******************************************************************************
 * FUNCTION NAME:	CControlorConnection.shp_handle_binary_data
 * DESCRIPTION:		handle the BINARY format package
 * return :	The used data size.
 * modification history
 * --------------------
 *    2013/09/10, QBryan create this function
 ******************************************************************************/
int  CControlorConnection::shp_handle_binary_data(void* bin,int size)
{
	SHP_BIN_HEAD_T* bin_head = (SHP_BIN_HEAD_T*)bin;
	char*			name = NULL;
	CDeviceConnection*		pDev = NULL;
	switch(bin_head->pid)
	{
	case SHP_SWITCH_PID:
		name=(bin_head->path_size)>0?((char*)bin)+sizeof(SHP_SWITCH_CONTROL_T):NULL;
		pDev = sys.find_connectd_device(name,bin_head->dev_id);
		if(pDev){
			pDev->shp_send_package('B',bin,sizeof(SHP_SWITCH_CONTROL_T));
		}else{
			DBG("Can **NOT** find connection which named: %s\r\n",name);
		}
		break;
	default:
		DBG("Unknow pid!!!!\r\n");
		break;
	}
	return sizeof(SHP_SWITCH_CONTROL_T)+bin_head->path_size;			
}

/******************************************************************************
 * FUNCTION NAME:	CControlorConnection.shp_handle_json_get_optype_by_name
 * --------------------
 *    2013/09/10, QBryan create this function
 ******************************************************************************/
U8 CControlorConnection::shp_handle_json_get_optype_by_name(S8* op_type_name)
{
	if(strcmp("OPEN",(char*)op_type_name)==0){
		return (U8)SHP_OP_OPEN;
	}else if(strcmp("CLOSE",(char*)op_type_name)==0){
		return (U8)SHP_OP_CLOSE;
	}else if(strcmp("READ",(char*)op_type_name)==0){
		return (U8)SHP_OP_READ;
	}
	DBG("Unknow op_type_name:%s!!!!\r\n",op_type_name);
	return SHP_OP_NOT;
}

/******************************************************************************
 * FUNCTION NAME:	shp_handle_json_get_optypename_by_type
 ******************************************************************************/
const S8* shp_handle_json_get_optypename_by_type(U8 op_type)
{
	switch(op_type){
		case SHP_OP_OPEN:
			return (S8*)"OPEN";
		case SHP_OP_CLOSE:
			return (S8*)"CLOSE";
		case SHP_OP_READ:
			return (S8*)"READ";
		default:
			DBG("Unknow op_type:%d!!!!\r\n",(int)op_type);
			break;
	}
	return NULL;
}

/******************************************************************************
 * FUNCTION NAME:	CControlorConnection.shp_handle_json_data
 * DESCRIPTION:		handle the json format package
 * return :	The used data size.
 * modification history
 * --------------------
 *    2013/09/10, QBryan create this function
 ******************************************************************************/
int  CControlorConnection::shp_handle_json_data(char* json_str,int size)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	Json::Reader 	reader; 
    Json::Value		root; 
	CDeviceConnection*		pDev = NULL;
	bool parser_ret	 = reader.parse(json_str,json_str+size,root,false);
    U16 pid 		 = root["PID"].asInt(); 
	U16 dev_id 		 = root["DEV_ID"].asInt();
	S8*	op_type_name = (S8*)root["OP_TYPE"].asString().c_str();
	U8	op_type 	 = shp_handle_json_get_optype_by_name(op_type_name);
	S8* path		 = (S8*)root["PATH"].asString().c_str();
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/

#if 1//////test//////////////////////////////begin//////////////////////////
	if(false == parser_ret){
		DBG("parse **failed**\r\n",op_type_name);
	}else{
		DBG("json_str = %s\r\n",json_str);
		DBG("pid = %d\r\n",pid);
		DBG("dev_id = %d\r\n",dev_id);
		DBG("op_type_name = %s\r\n\r\n",op_type_name);
		DBG("path = %s\r\n\r\n",path);
	}
#endif//////test//////////////////////////////end////////////////////////////
	
	switch(pid)
	{
		case SHP_SWITCH_PID:{
			SHP_SWITCH_CONTROL_T bin = {{1001,0,0,0},0};
			bin.bin_head.op_type = op_type;
			bin.bin_head.dev_id= dev_id;
			pDev = sys.find_connectd_device((char*)path,dev_id);
			if(pDev){
				DBG("Found dev connectiong.\r\n",json_str);
				pDev->shp_send_package('B',&bin,sizeof(SHP_SWITCH_CONTROL_T));
			}else{
				DBG("Can *NOT* found dev connection.\r\n",json_str);
			}
			break;
		}
		default:
			DBG("Unknow pid!!!!\r\n");
			break;
	}
	return SHP_USED_SIZE_ALL;
}


/******************************************************************************
 * FUNCTION NAME:	CControlorConnection.shp_handle_xml_data
 * DESCRIPTION:		handle the xml format package
 * return :	The used data size.
 * modification history
 * --------------------
 *    2013/09/10, QBryan create this function
 ******************************************************************************/
int  CControlorConnection::shp_handle_xml_data(char* xml_json_str,int size)
{
	return SHP_USED_SIZE_ALL;
}



