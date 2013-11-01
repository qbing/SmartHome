#ifndef __DRIVER_20130628_H__
#define __DRIVER_20130628_H__

#define   MY_STM8S003F
//#define QB_MCU_S105C6

#if defined(QB_MCU_S003S3P)||defined(MY_STM8S003F)
#include "stm8s003f3p.h"
_Bool   indicator @PB_ODR:5;//≥‰µÁ÷–
#define SYSTEM_RUNNING_IND()    do{indicator = ~indicator;}while(0)

#elif defined(QB_MCU_S105C6)
#include "STM8S105C6.h"
/* UART1 Status Register */
DEF_8BIT_REG_AT(UART1_SR,0x5240);

/* UART1 Data Register */
DEF_8BIT_REG_AT(UART1_DR,0x5241);

/* UART1 Baud Rate Register 1 */
DEF_8BIT_REG_AT(UART1_BRR1,0x5242);

/* UART1 Baud Rate Register 2 */
DEF_8BIT_REG_AT(UART1_BRR2,0x5243);

/* UART1 Control Register 1 */
DEF_8BIT_REG_AT(UART1_CR1,0x5244);

/* UART1 Control Register 2 */
DEF_8BIT_REG_AT(UART1_CR2,0x5245);

/* UART1 Control Register 3 */
DEF_8BIT_REG_AT(UART1_CR3,0x5246);

/* UART1 Control Register 4 */
DEF_8BIT_REG_AT(UART1_CR4,0x5247);

/* UART1 Control Register 5 */
DEF_8BIT_REG_AT(UART1_CR5,0x5248);

/* UART1 Control Register 6 */
DEF_8BIT_REG_AT(UART1_CR6,0x5249);

/* UART1 Guard time Register */
DEF_8BIT_REG_AT(UART1_GTR,0x524a);

/* UART1 Prescaler Register */
DEF_8BIT_REG_AT(UART1_PSCR,0x524b);

#define indicator system.lamb1_state
#define SYSTEM_RUNNING_IND()    do{system.lamb1_state = ~system.lamb1_state;}while(0)

#endif
#define  INTERUPT_ENABLE()	_asm("rim")//RIM,SIM,HALT,WFI,IRET
#define  INTERUPT_DISENABLE()	_asm("sim")

#include "framework.h"

extern void driver_init(void);
extern void drive_device(void);
extern void operate(DEVICE_ID dev_id,OP_TYPE op_type,void* op_data);
extern U8 generate_state_buffer(U8* dest);
extern S16 GetT(void);

#endif//__DRIVER_20130628_H__