#ifndef Relay_USE
#define Relay_USE
#include "../FWlib/inc/stm32f10x_gpio.h"

//extern		_Bool		Relay1_PIN;		//relay pin
//extern		_Bool		Relay2_PIN;
//extern		_Bool		Relay3_PIN;
//extern		_Bool		Relay4_PIN;
#define RPORT GPIOA

#define R1_PIN GPIO_Pin_0
#define R2_PIN GPIO_Pin_1
#define R3_PIN GPIO_Pin_4
#define R4_PIN GPIO_Pin_5

#define Relay1_PIN_connected  GPIO_WriteBit(RPORT, R1_PIN,(BitAction)1)
#define Relay1_PIN_disconnected  GPIO_WriteBit(RPORT, R1_PIN,(BitAction)0)
#define Relay2_PIN_connected  GPIO_WriteBit(RPORT, R2_PIN,(BitAction)1)
#define Relay2_PIN_disconnected  GPIO_WriteBit(RPORT, R2_PIN,(BitAction)0)
#define Relay3_PIN_connected  GPIO_WriteBit(RPORT, R3_PIN,(BitAction)1)
#define Relay3_PIN_disconnected  GPIO_WriteBit(RPORT, R3_PIN,(BitAction)0)
#define Relay4_PIN_connected  GPIO_WriteBit(RPORT, R4_PIN,(BitAction)1)
#define Relay4_PIN_disconnected  GPIO_WriteBit(RPORT, R4_PIN,(BitAction)0)

typedef enum Relay{
	Relay1,
	Relay2,
	Relay3,
	Relay4
}RelayNum;

int Relay_init(RelayNum r);
int Relay_Connect(RelayNum r);
int Relay_Disconnect(RelayNum r);
#endif



