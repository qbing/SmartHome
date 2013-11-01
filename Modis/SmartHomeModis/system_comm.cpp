#include "system_comm.h"


char* mem2Hex(char* src,int size)
{
	static char hex[1024*10];
	char temp[16];
	hex[0]=0;
	for(int i=0;i<size;i++)
	{
		sprintf(temp,"0X%02X ",*(src+i));
		strcat(hex,temp);
		if(i!=0 && i%10==0)strcat(hex,"\r\n");
	}
	return hex;
}

ModisSocket::ModisSocket(QTcpSocket * tcpSoc)
{
	this->tcpSoc = tcpSoc;
	recv_buffer = new char[RECV_BUFFER_LEN];
	recved_len = 0;
	used_len = 0;
	send_buffer = new char[SEND_BUFFER_LEN];
	sended_len = 0;
	unsended_len = 0;

	memset(&rd,0,sizeof(rd));
	recived_data_handler = NULL;

	connect(tcpSoc,SIGNAL(readyRead()),this,SLOT(readyRead()));

	////////for sendData//////////////begin////////////
	extern ModisSocket* lastSoc;
	if(lastSoc!=this && lastSoc!=NULL){
		DBG((lastSoc->tcpSoc->errorString()));
		delete lastSoc;
	}
	lastSoc = this;
	////////for sendData//////////////end//////////////
}

ModisSocket::~ModisSocket()
{
	////////for sendData//////////////begin////////////
	extern ModisSocket* lastSoc;
	if(lastSoc==this) lastSoc = NULL;
	////////for sendData//////////////end//////////////
	
	delete[] recv_buffer;
	delete[] send_buffer;
}

void ModisSocket::SetHandler(RECV_HANDLER_T recived_data_handler)
{
	this->recived_data_handler = recived_data_handler;
}

void ModisSocket::run()
{
	qint64 len = 0;
	int min_len = 0;
	char temp_recv[128];

	while(1)
	{
		break;
	}
}

void ModisSocket::readyRead (void)
{
	qint64 len = 0;
	int min_len = 0;
	char temp_recv[128];

	while(1)
	{
		len = tcpSoc->read(temp_recv,sizeof(temp_recv));
#if 1
		/*//////shp example***************************begin*************/{
			if(len>0){
				shp_example_data_arrived_called_by_system((U8*)temp_recv,len);
			}else{
				DBG(tcpSoc->errorString());
				break;
			}
		/*//////shp example***************************end***************/}
#else
		if(len>0)
		{
			qDebug("recived %d byte data :\r\n",len);
			//qDebug(mem2Hex(temp_recv,len));
			shp_dbg_mem(temp_recv,len);
			//memcpy(temp_recv,recv_buffer+recved_len,len);
			min_len = sizeof(rd.recv_buffer) - rd.recved_size;
			if(min_len>len){
				min_len=len;
			}

			memcpy(rd.recv_buffer+rd.recved_size,temp_recv,min_len);
			rd.recved_size+=min_len;

			if(recived_data_handler){
				int used_len = 0;
				while(rd.recved_size){
					used_len = recived_data_handler(&rd);
					if(used_len>0 && used_len<=rd.recved_size){
						rd.recved_size -= used_len;
						memcpy(rd.recv_buffer+used_len,rd.recv_buffer,rd.recved_size);
					}else
					{
						rd.recved_size = 0;
					}
				}
			}else
			{
				rd.recved_size = 0;
			}
		}else{
			qDebug("len = %d break",len);
			break;
		}
#endif
	}
}

/******************************************************************************
 * FUNCTION NAME:	sendData
 * DESCRIPTION:	    发送数据
 * modification history
 * --------------------
 *    2013/06/30, Q.Bryan create this function
 ******************************************************************************/
void ModisSocket::sendData(U8* data,U16 size)
{
	tcpSoc->write((const char*)data,size);
}


///////////////////////////////////////临时代码///////////////////////////////////begin////////////
ModisSocket* lastSoc = NULL;
extern "C" void sendData(U8* data,U16 size)
{
#if 1
	if(lastSoc && lastSoc->tcpSoc->state()==QAbstractSocket::ConnectedState)
		lastSoc->sendData(data,size);
	else
		DBG("Socket is not connect ,cannot send data !!!");
#endif
}

/******************************************************************************
 * FUNCTION NAME:	report_system_state
 * DESCRIPTION:	    报告系统当前状态，起到心跳包的作用
 * modification history
 * --------------------
 *    2013/06/30, Q.Bryan create this function
 ******************************************************************************/
extern "C" void report_system_state(void)
{
  sys.current_temp +=1 ;
  //send to network
  //sendData((U8*)&sys,sizeof(sys));
}
extern "C" void driver_init(void)
{
}
extern "C" void drive_device(void)
{
}
extern "C" void updata_temperature(void);
void SystemTimer::update(void)
{
	//disp temperature
	updata_temperature();

	//if disconnected form server, try to connect once again!!!
	if(NULL==lastSoc || (lastSoc!=NULL && lastSoc->tcpSoc->state()==QAbstractSocket::UnconnectedState)){
		QTcpSocket* tcpSoc = new QTcpSocket();
		tcpSoc->connectToHost(SERVER_HOST_IP_ADDRESS,SERVER_HOST_PORT);
		DBG(tcpSoc->errorString());
		ModisSocket* modisSoc = new ModisSocket(tcpSoc);
		/*//////shp example***************************begin*************/{
		shp_example_init_called_by_system();
		/*//////shp example***************************end***************/}
	}

	//tick
	report_system_state();

	
	/*//////shp example***************************begin*************/{
	shp_example_tick_called_by_system();
	/*//////shp example***************************end***************/}
}
///////////////////////////////////////临时代码/////////////////////////////////////end//////////








