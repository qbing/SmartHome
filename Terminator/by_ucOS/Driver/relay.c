#include "relay.h"

int Relay_init(RelayNum r){
	GPIO_InitTypeDef GPIO_InitStructure;
	static u8 Relay_Busy=0;
	if(Relay_Busy){
		return 0;
	}else{
		Relay_Busy=1;
		GPIO_InitStructure.GPIO_Pin = R1_PIN|R2_PIN|R3_PIN|R4_PIN;								
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(RPORT, &GPIO_InitStructure);
	}
	return 1;
}
int Relay_Connect(RelayNum r){
	switch(r){
		case Relay1:
			Relay1_PIN_connected;break;
		case Relay2:
			Relay2_PIN_connected;break;
		case Relay3:
			Relay3_PIN_connected;break;
		case Relay4:
			Relay4_PIN_connected;break;
		default:break;
	}
	return 1;
}

int Relay_Disconnect(RelayNum r){
	switch(r){
		case Relay1:
			Relay1_PIN_disconnected;break;
		case Relay2:
			Relay2_PIN_disconnected;break;
		case Relay3:
			Relay3_PIN_disconnected;break;
		case Relay4:
			Relay4_PIN_disconnected;break;
		default:break;
	}
	return 1;
}

void operate(char dev_id,char op_type,void* op_data)
{
    tty_printf("operating...");
		//for(int i=0;i<sizeof(LEDS)/sizeof(LEDS[0]);i++)
    if(dev_id>=0 && dev_id<3)
    {
        if(op_type==0/*OP_OPEN*/){
						tty_printf("type == close\r\n");
            switch(dev_id)
            {
                case 0:
                    Relay_Connect(Relay1);
                    break;
                case 1:
                    Relay_Connect(Relay2);
                    break;
                case 2:
                    Relay_Connect(Relay3);
                    Relay_Connect(Relay4);
                    break;
                default:
                    break;
            }
        }else{
						tty_printf("type == open\r\n");
           switch(dev_id)
            {
            case 0:
                Relay_Disconnect(Relay1);
                break;
            case 1:
                Relay_Disconnect(Relay2);
                break;
            case 2:
                Relay_Disconnect(Relay3);
                Relay_Disconnect(Relay4);
                break;
            default:
                break;
            }
        }
    }
		tty_printf("operated *DONE*\r\n");
}

