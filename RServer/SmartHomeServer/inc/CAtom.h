/******************************************************************************
 *  Copyright Statement:
 *    Copyright (c)  Q.Bryan <qbing2010@163.com>
 *    All rights reserved.
 *-----------------------------------------------------------------------------
 * Filename:      CAtom.h
 * Description:   Define the Atom of device
 * Author:        Q.Bryan	2013/09/16 15:02:12
 *=============================================================================
 * HISTORY
 *------Below this line--------------------------------------------------------
 *3.your_name datetime->add_your_change_for_this_file.
 *2.your_name datetime->add_your_change_for_this_file.
 *1.Q.Bryan 2013/09/16 15:02:12->create this file.
 *------Upper this line--------------------------------------------------------
 *=============================================================================
******************************************************************************/
#ifndef __CATOM_H__
#define __CATOM_H__

#include "SmartHomeInfr.h"
char *my_itoa(int val, char *buf, unsigned radix);



class CAtomBase
{
public:
    U16 id;
public:
    CAtomBase(void);
    virtual ~CAtomBase(void);
public:
    virtual std::string tostring(void)=0;
    virtual bool parse_tick_data(void*data,int len)=0;
    static CAtomBase* parse_jsonstr(Json::Value root);
    static DEV_TYPE get_dev_type_by_name(std::string name);
    static CAtomBase* get_dev_instance_by_type(DEV_TYPE dev_type);
};


class CSwitchAtom : public CAtomBase
{
public:
    U8 state;
public:
    std::string tostring(void);
    bool parse_tick_data(void*data,int len);
};

class CVariableAtom : public CAtomBase
{
public:
    U32 variable;
public:
    std::string tostring(void);
    bool parse_tick_data(void*data,int len);
};


#endif//__CATOM_H__


