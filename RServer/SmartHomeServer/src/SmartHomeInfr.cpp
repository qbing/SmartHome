/******************************************************************************
 *  Copyright Statement:
 *    Copyright (c)  Q.Bryan <qbing2010@163.com>
 *    All rights reserved.
 *-----------------------------------------------------------------------------
 * Filename:      SmartHomeInfr.cpp
 * Description:   The system information file
 * Author:        Q.Bryan	2013/09/29 17:12:05
 *=============================================================================
 * HISTORY
 *------Below this line--------------------------------------------------------
 *3.your_name datetime->add_your_change_for_this_file.
 *2.your_name datetime->add_your_change_for_this_file.
 *1.Q.Bryan 2013/09/29 17:12:05->create this file.
 *------Upper this line--------------------------------------------------------
 *=============================================================================
******************************************************************************/

#include "SmartHomeInfr.h"

CSystemInfr::CSystemInfr(void)
{
	devices.clear();
	ctls.clear();
	sem_init(&sys_sem,0,1);
	sem_init(&dev_sem,0,0);
	sem_init(&ctl_sem,0,0);

	strcpy(dev_tree.name,"RServer");
}
CSystemInfr::~CSystemInfr(void)
{

}

int CSystemInfr::RemoveInvalidDevice(void)
{
	std::vector<CDeviceConnection*>::iterator it=devices.begin();
	std::vector<CDeviceConnection*>::iterator end=devices.end();

	while(it!=end)
	{
		if((*it)->socket_id==-1){
			devices.erase(it);
			if(*it) delete *it;
			DBG("One CDeviceConnection removed!!!\r\n");
		}
		it++;
	}
	return 0;
}

CDeviceConnection* CSystemInfr::find_connectd_device(char* path,U16 dev_id)
{
#if 1
	char cur_dir[128];
	int start = 0;
	int end = 0;
	char* p = path;
	int i = 0;
	int len = 0;
	CDeviceTree* tree = &sys.dev_tree;

	if(path==NULL)return NULL;
	
	//split
	while(*p){
		if(*p=='\\'||*p=='/'){
			cur_dir[i++] = 0;
			p++;
		}else{
			cur_dir[i++] = *p++;
		}
	}
	len = i;

	//find
	p = cur_dir;
	while(*p==0 && p-cur_dir<len)p++;
	if(strcmp(p,this->dev_tree.name)){
		DBG("Warning:The first dir is not current RServer!!\r\n",p);
	}else{
		DBG("Skip current RServer dir.\r\n");
		p+=strlen(p);
	}
	while(*p==0 && p-cur_dir<len)p++;
	
	DBG("Find connection which named: %s\r\n",p);
	if(p-cur_dir<len){
		tree = tree->findChild_By_name(p);
	}else{
		tree = NULL;
	}
	if(tree){
		if(tree->device_conn){
			DBG("Find connection which named %s *DONE*\r\n",p);
		}else{
			DBG("Find connection which named %s *DONE*, but the connection is *NULL*\r\n",p);
		}
		return tree->device_conn;
	}
	return NULL;
#else
	////////**JUST FOR TEST NOW***////////////
	if(devices.size()>=1)
		return &devices[0];
	else
		return NULL;
	////////**JUST FOR TEST NOW***////////////
#endif
}


int CSystemInfr::RemoveInvalidControl(void)
{
	std::vector<CControlorConnection*>::iterator it=ctls.begin();
	std::vector<CControlorConnection*>::iterator end=ctls.end();

	while(it!=end)
	{
		if((*it)->socket_id==-1){
			ctls.erase(it);
			if(*it) delete *it;
			DBG("One CControlorConnection removed!!!\r\n");
		}
		it++;
	}
	return 0;
}

