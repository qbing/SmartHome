/******************************************************************************
 *  Copyright Statement:
 *    Copyright (c)  Q.Bryan <qbing2010@163.com>
 *    All rights reserved.
 *-----------------------------------------------------------------------------
 * Filename:      SmartHomeInfr.h
 * Description:   The system information file
 * Author:        Q.Bryan	2013/09/29 17:10:56
 *=============================================================================
 * HISTORY
 *------Below this line--------------------------------------------------------
 *3.your_name datetime->add_your_change_for_this_file.
 *2.your_name datetime->add_your_change_for_this_file.
 *1.Q.Bryan 2013/09/29 17:10:56->create this file.
 *------Upper this line--------------------------------------------------------
 *=============================================================================
******************************************************************************/


#ifndef SMARTHOME_INFO_H__
#define SMARTHOME_INFO_H__

#include <stdio.h> 
#include <stdlib.h>
#include <netinet/in.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <semaphore.h>
#include <vector>
#include <list>
#include <string.h>
#include "json/json.h"

#ifndef DBG
#define DBG printf
#elif 1
#undef DBG
#define DBG printf
#endif
void shp_dbg_mem(char *data, int len);

#include "SHP.h"
#include "CAtom.h"

extern "C" char* MyMD5(const char* data,int len);

using namespace std;

#define MAX_DEVICE_NUM      16
#define MAX_DEVICE_NAME_LEN 64

class   CAtomBase;
class   CDeviceTree;
typedef CDeviceTree CDeviceTreeNode;

class CDataBuffer
{
public:
    char    data[1024];
    int     total_len;
    int     start;
    int     end;
public:
    CDataBuffer()
    {
        total_len = sizeof(data);
        start = 0;
        end = 0;
    }
    char* GetData()
    {
        return data+start;
    }
    int PutData(U8* bin,int size)
    {   
        if(end+size<total_len){
            memcpy(data+end,bin,size);
            end+=size;
        }else{
            DBG("The buffer is not enough!!!!!!!!!!!!!!!!!!!!!\r\n");
        }
    }
    int GetLengh()
    {
        return end-start;
    }
    int SetUsed(int size)
    {
        start+=size;
        if(start==end){
            start=0;
            end=0;
        }
    }
};

class CDeviceConnection
{
public:
    char                name[MAX_DEVICE_NAME_LEN];
    char                mac[6];
    U8                  dev_type;
    int                 socket_id;
    CDataBuffer         buf;
    SHP_SVR_STATUS_T    state;
    CDeviceTreeNode*    treeNode;
    int                 timeout_second;
public:
    CDeviceConnection(void);
    ~CDeviceConnection(void);
    int handle_recieved_data(U8* bin,U16 len);
    int handle_recieved_buffer(CDataBuffer& buf);
public:
    int  shp_handle_consult_data(void* bin,int size);
    int  shp_handle_binary_data(void* bin,int size);
    int  shp_handle_json_data(char* json_str,int size);
    int  shp_handle_xml_data(char* xml_json_str,int size);
    int  shp_update_system_device_tree_infr(const char* json_str,int len);
public:
    ////*type= 'C' or 'B' or 'J' or 'X'*///////////
    S16 shp_send_package(S8 type,void* bin,S16 len);
};

class CControlorConnection
{
public:
    char            name[MAX_DEVICE_NAME_LEN];
    int             socket_id;
    CDataBuffer     buf;
public:
    int handle_control_data(U8* bin,U16 len);
    int handle_control_buffer(CDataBuffer& buf);
public:
    int  shp_handle_consult_data(void* bin,int size);
    int  shp_handle_binary_data(void* bin,int size);
    int  shp_handle_json_data(char* json_str,int size);
    int  shp_handle_xml_data(char* xml_json_str,int size);
public:
    //json
    U8 shp_handle_json_get_optype_by_name(S8* op_type_name);
    const S8* shp_handle_json_get_optypename_by_type(U8 op_type);
};


class CDeviceTree
{
public:
    const static int    MAX_DEVICE_NODE_NAME_SIZE = 64;
    typedef enum DEVICE_NODE_TYPE
    {
        TYPE_DEV,
        TYPE_SET,
        TYPE_TERM,
        TYPE_INVALID
    }DEVICE_NODE_TYPE;
    
public:
    CDeviceTree();
    ~CDeviceTree();
    char        name[MAX_DEVICE_NODE_NAME_SIZE];        //node name
    char        mac[6];
    DEVICE_NODE_TYPE        type;
    CDeviceConnection*      device_conn;                     //if This device node is a real device,this pointer point to it.
                                                        //else if it is not a real device,This pointer **MUST** be NULL
    bool                    isActive;
    std::list<CDeviceTree*>      children_nodes;
    
    CAtomBase*              atom;
public:
    //int     data_type;
    //int     data_len;
    //char*   data;
public:
    CDeviceTree* findChild_By_name(char* name);
    std::string  toJsonString(void);
    CDeviceTree* shp_parse_device_tree(CDeviceConnection*conn,const char* json_str,int len);
    CDeviceTree* shp_parse_device_tree(Json::Value root);
    void setIsActive(bool active=true);
public:
    DEVICE_NODE_TYPE get_node_type_by_name(std::string name);
    std::string      get_name_by_node_type(DEVICE_NODE_TYPE node_type);
    CAtomBase*       find_atom_by_devid(U16 dev_id);
};


class CSystemInfr
{
public:
    CSystemInfr(void);
    ~CSystemInfr(void);
    int RemoveInvalidDevice(void);
    int RemoveInvalidControl(void);
public:
    CDeviceTree         dev_tree;
public:
    sem_t               sys_sem;
    sem_t               ctl_sem;
    sem_t               dev_sem;
    std::vector<CDeviceConnection*>     devices;    //all device
    std::vector<CControlorConnection*>  ctls;       //all controlor
public:
    CDeviceConnection* find_connectd_device(char* path,U16 dev_id);
    int shp_update_system_device_tree_infr_to_file(void);
};
extern CSystemInfr sys;

#endif


