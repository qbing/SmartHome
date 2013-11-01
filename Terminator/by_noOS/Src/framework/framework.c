
#include "framework.h"


/******************************************************************************
 * FUNCTION NAME:	init
 * DESCRIPTION:	    initialize framework
 * --------------------
 *    2013/08/22, Q.Bryan create this function
 ******************************************************************************/
void init(void)
{
    //deviec init
    driver_init();
    //set device info
    //SendPkgData('C',(U8*)DEVICE_INFO,sizeof(DEVICE_INFO));
    //SetProgrameStae
    //SetProgrameStae(PRG_STATE_RUNNING);
}


/******************************************************************************
 * FUNCTION NAME:	SystemEnterPer1ms
 * DESCRIPTION:	    Enter Per 1ms
 * --------------------
 *    2013/06/30, Q.Bryan create this function
 ******************************************************************************/
void SystemEnterPer1ms(void)
{
    extern void report_system_state(void);
    //test
    static int i = 0;
    i++;
    #ifdef SYSTEM_RUNNING_IND
    //indicator
    if(i%233==0)SYSTEM_RUNNING_IND();
    #endif

    if(i==1000){
        i=0;
        //myprintf("T=%d",sys.current_temp);
        report_system_state();
    }
    drive_device();
}


/******************************************************************************/
static PRG_STATE system_state = PRG_STATE_POWERON;
/*******************************************************************************
 * FUNCTION NAME:	SetProgrameStae
 * DESCRIPTION:	    设置系统状态
 * --------------------
 *    2013/08/22, Q.Bryan create this function
 ******************************************************************************/
void SetProgrameStae(PRG_STATE state)
{
    system_state = state;
}

/*******************************************************************************
 * FUNCTION NAME:	SetProgrameStae
 * DESCRIPTION:	    得到系统状态
 * --------------------
 *    2013/08/22, Q.Bryan create this function
 ******************************************************************************/
PRG_STATE GetProgrameStae(void)
{
    return system_state;
}


