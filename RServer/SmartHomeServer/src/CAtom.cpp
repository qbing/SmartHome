/******************************************************************************
 *  Copyright Statement:
 *    Copyright (c)  Q.Bryan <qbing2010@163.com>
 *    All rights reserved.
 *-----------------------------------------------------------------------------
 * Filename:      CAtom.cpp
 * Description:   Define the Atom of device
 * Author:        Q.Bryan	2013/09/16 15:05:40
 *=============================================================================
 * HISTORY
 *------Below this line--------------------------------------------------------
 *3.your_name datetime->add_your_change_for_this_file.
 *2.your_name datetime->add_your_change_for_this_file.
 *1.Q.Bryan 2013/09/16 15:05:40->create this file.
 *------Upper this line--------------------------------------------------------
 *=============================================================================
******************************************************************************/
#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <semaphore.h>
#include <vector>
#include <list>
#include <string.h>
#include "json/json.h"
#include "SHP.h"

#include "CAtom.h"

CAtomBase::CAtomBase(void)
{
	id = 0;
}

CAtomBase::~CAtomBase(void)
{

}

CAtomBase* CAtomBase::parse_jsonstr(Json::Value root)
{
    Json::Value temp;
	CAtomBase*  atom = NULL;

	temp = root["DEVTYPE"];
	if(temp.type()!=Json::nullValue)
	{
		DEV_TYPE dev_type = get_dev_type_by_name(temp.asString());
		atom = get_dev_instance_by_type(dev_type);

		if(atom){
			temp = root["DEV_ID"];
			if(temp.type()!=Json::nullValue)
			{
				atom->id = temp.asInt();
			}
		}else{
			DBG("Warning : get_dev_instance_by_type return NULL, **UNSUPPORT** device!\r\n");
		}
	}else{
		DBG("Warning : DEVTYPE  **NOT** found!\r\n");
	}
	return atom;
}

CAtomBase* CAtomBase::get_dev_instance_by_type(DEV_TYPE dev_type)
{
	switch(dev_type)
	{
		case SHP_SWITCH_PID:
			return new CSwitchAtom();
		case SHP_T_SENSOR_PID:
			return NULL;
		case SHP_G_SENSOR_PID:
			return NULL;
		case SHP_VARIABLE_PID:
			return new CVariableAtom();
		case SHP_KEYBOARD_PID:
			return NULL;
		case SHP_CAMERA_PID:
			return NULL;
		case SHP_CUSTOM_PID:
			return NULL;
	}
	return NULL;
}

DEV_TYPE CAtomBase::get_dev_type_by_name(std::string name)
{
	if(name == "SWITCH"){
		return SHP_SWITCH_PID;
	}else if(name == "T_SENSOR"){
		return SHP_T_SENSOR_PID;
	}else if(name == "G_SENSOR"){
		return SHP_G_SENSOR_PID;
	}else if(name == "VARIABLE"){
		return SHP_VARIABLE_PID;
	}else if(name == "KEYBOARD"){
		return SHP_KEYBOARD_PID;
	}else if(name == "CAMERA"){
		return SHP_CAMERA_PID;
	}else if(name == "CUSTUM"){
		return SHP_CAMERA_PID;
	}
}



std::string CSwitchAtom::tostring(void)
{
    std::string str="\"SWITCH_STATE\":";
    str+=(state==0)?"0":"1";
    return str;
}

bool CSwitchAtom::parse_tick_data(void*data,int len)
{
    if(len==1){
        state = *((U8*)data);
		DBG("Device id:%d,Switch state=%s\r\n",id,(state)?"OPENED":"CLOSED");
        return true;
    }else{
		DBG("Warning : CVariableAtom  parse_tick_data **NOT** valid!\r\n");
        return false;
    }
}



std::string CVariableAtom::tostring(void)
{
    std::string str="\"VARIABLE_VALUE\":";
    str+=my_itoa(variable,NULL,10);
    return str;
}

bool CVariableAtom::parse_tick_data(void*data,int len)
{
    if(len==4){
        variable = *((U32*)data);
		DBG("Device id:%d,Variable value=%d\r\n",id,variable);
        return true;
    }else{
		DBG("Warning : CVariableAtom  parse_tick_data **NOT** valid!\r\n");
        return false;
    }
}

