/******************
*function:include all kind of driver
*author:Zachary
*time:2013/8/12
********************/

#include "relay.h"
#include "DS1820.h"
#include "usart.h"
#include "tty.h"

#include "framework.h"
extern S16 GetT(void);
/******************************************************************************
 * FUNCTION NAME:	sendData
 * DESCRIPTION:	    ·¢ËÍÊý¾Ý
 * modification history
 * --------------------
 *    2013/06/30, Q.Bryan create this function
 ******************************************************************************/
extern void sendData(U8* data,U16 size);


void report_system_state(void);


