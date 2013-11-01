/******************************************************************************
 *  Copyright Statement:
 *    Copyright (c)  Q.Bryan <qbing2010@163.com>
 *    All rights reserved.
 *-----------------------------------------------------------------------------
 * Filename:      main.cpp
 * Description:   SmartHome main file
 * Author:        Q.Bryan	2013/09/06 14:14:41
 *=============================================================================
 * HISTORY
 *------Below this line--------------------------------------------------------
 *3.your_name datetime->add_your_change_for_this_file.
 *2.your_name datetime->add_your_change_for_this_file.
 *1.Q.Bryan 2013/09/06 14:14:41->create this file.
 *------Upper this line--------------------------------------------------------
 *=============================================================================
******************************************************************************/

#include <netinet/in.h> 
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <termios.h>
#include <arpa/inet.h>
#include "SmartHomeInfr.h"

#define SERVER_CONTROL_PORT			9596 
#define DEVICE_CONNECT_PORT			9595 
#define LENGTH_OF_LISTEN_QUEUE		20
#define TEMP_BUFFER_SIZE			2048



CSystemInfr sys;

static void* control_connection_handler_thread(void*arg)
{
	int             i;
	fd_set         	fdread;
	fd_set         	fderror;
	int             ret;
	int				ctl_count=0;
	char			temp_buf[TEMP_BUFFER_SIZE];
	int				max_soc_id = -1;
	int 			main_socket = *((int*)arg);
	DBG("\"control_connection_handler_thread\" created *SUCCESS*\r\n");
	while(1)
	{
		#if  1
			struct timeval 	tv = {15, 0};
			max_soc_id = main_socket;
			//set cared read event
			FD_ZERO(&fdread);
			FD_ZERO(&fderror);
			ctl_count = sys.ctls.size();
			if(ctl_count==0)
			{
				//no connection
				sleep(3);
				continue;
			}
			for (i = 0; i < ctl_count; i++)
			{
				if(max_soc_id<sys.ctls[i]->socket_id){
					max_soc_id = sys.ctls[i]->socket_id;
				}
				FD_SET(sys.ctls[i]->socket_id, &fdread);
				FD_SET(sys.ctls[i]->socket_id, &fderror);
			}
			FD_SET(main_socket, &fdread);
			FD_SET(main_socket, &fderror);
			DBG("control_connection_handler_thread:waiting com data...\r\n");
			ret = select(max_soc_id+1, &fdread, NULL, &fderror,  /*&tv*/NULL);
			if (ret <= 0)
			{
				printf("Control thread: Time expired or Something error.\r\n",ret);
				// Time expired
				continue;
			} 
			//check event
			for (i = 0; i < ctl_count; i++)
			{
				//error
				if (FD_ISSET(sys.ctls[i]->socket_id, &fderror))
				{
					DBG("Socket Error,close socket!!!\r\n");
					close(sys.ctls[i]->socket_id);
					sys.ctls[i]->socket_id = -1;
					continue;
				}
				//recv
				if (FD_ISSET(sys.ctls[i]->socket_id, &fdread))
				{
					// A read event happened on g_CliSocketArr
					ret = recv(sys.ctls[i]->socket_id, temp_buf, sizeof(temp_buf), MSG_DONTWAIT);
					if (ret == 0)
					{
						
					}else if(ret < 0){
						DBG("Socket recv data error,close socket!!!\r\n");
						close(sys.ctls[i]->socket_id);
						sys.ctls[i]->socket_id = -1;
						continue;
					}else{
						int used = 0;
						DBG("\r\n******************Recived*******Begin*****************\r\n");
						shp_dbg_mem(temp_buf,ret);
						sem_wait(&sys.sys_sem);
						used = sys.ctls[i]->handle_control_data((U8*)temp_buf,ret);
						sem_post(&sys.sys_sem);
						DBG("Handled %d Byte data\r\n",used);
						DBG("\r\n******************Recived*******End*******************\r\n");
					}
				}
			}
			sys.RemoveInvalidControl();
		#else
			/*///////**JUST FOR TEST***///////////*/{
			static SHP_HeadForRecv head = {SHP_MAGIC1,sizeof(SHP_HeadForRecv)+sizeof(SHP_SWITCH_CONTROL_T),SHP_MAGIC2,'B',0};
			static SHP_SWITCH_CONTROL_T test_swtich={{SHP_SWITCH_PID,0,0,0},0};
			static U8 temp_buf[sizeof(SHP_HeadForRecv)+sizeof(SHP_SWITCH_CONTROL_T)];
			static CControlorConnection ctl;
			test_swtich.bin_head.op_type=!test_swtich.bin_head.op_type;
			memcpy(temp_buf,&head,sizeof(SHP_HeadForRecv));
			memcpy(temp_buf+sizeof(SHP_HeadForRecv),&test_swtich,sizeof(test_swtich));
			ctl.handle_control_data(temp_buf,head.pkg_len);

			sleep(4);
			/*///////**JUST FOR TEST***///////////*/}
		#endif
	}
}


/******************************************************************************
 * FUNCTION NAME:	server_control_thread
 * DESCRIPTION:		This is a thread function for listen PORT;
 					After accept a connect create a socket thread to handle the data
 * --------------------
 *    2013/09/06, Q.Bryan create this function
 ******************************************************************************/
static void* server_control_thread(void* arg)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
    struct sockaddr_in server_addr;
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	DBG("\"server_control_thread\" created *SUCCESS*\r\n");
	//initialize
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family 		= AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port		= htons(SERVER_CONTROL_PORT);

	//open socket
    int main_socket = socket(PF_INET,SOCK_STREAM,0);
    if( main_socket < 0)
    {
        printf("Create Socket Failed!\r\n");
        exit(1);
    }

	//set socket option
	{ 
		int opt =1;
		setsockopt(main_socket,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	}

	//bind
    if( bind(main_socket,(struct sockaddr*)&server_addr,sizeof(server_addr)))
    {
        printf("Server Bind Port : %d Failed!", SERVER_CONTROL_PORT); 
        exit(1);
    }

	//listen
    if (listen(main_socket, LENGTH_OF_LISTEN_QUEUE) )
    {
        printf("Server Listen Failed!"); 
        exit(1);
    }

	//create a thread to handle controlor data
	{
		pthread_t pid;
		pthread_create(&pid,NULL,control_connection_handler_thread,&main_socket);
	}
    while (1)
    {
		struct sockaddr_in client_addr;
        socklen_t length = sizeof(client_addr);

		//waiting client to connect
		printf("waiting control client to connect\r\n");
        int new_server_socket = accept(main_socket,(struct sockaddr*)&client_addr,&length);
        if ( new_server_socket < 0)
        {
            printf("Server Accept Failed!\n");
        }else{
        	CControlorConnection* ctl = new CControlorConnection();
            printf("Server Accept success!\n");
			ctl->socket_id = new_server_socket;
			sem_wait(&sys.sys_sem);
        	sys.ctls.push_back(ctl);
			sem_post(&sys.sys_sem);
		}
    }
	//close server socket
    close(main_socket);

	return 0;
}




/******************************************************************************
 * FUNCTION NAME:	device_connection_handler_thread
 * DESCRIPTION:		
 					device connection handler;
 * --------------------
 *    2013/09/06, Q.Bryan create this function
 ******************************************************************************/
static void* device_connection_handler_thread(void*arg)
{
	int             i;
	fd_set         	fdread;
	fd_set         	fdwrite;
	fd_set         	fderror;
	int             ret;
	int				device_count=0;
	char			temp_buf[TEMP_BUFFER_SIZE];
	int				max_soc_id = -1;
	int 			main_socket = *((int*)arg);
	DBG("\"device_connection_handler_thread\" created *SUCCESS*\r\n");
	while(1)
	{		
	#if 1
		struct timeval 	tv = {15, 0};
		//set cared read event
		FD_ZERO(&fdread);
		FD_ZERO(&fderror);
		max_soc_id = main_socket;
		device_count = sys.devices.size();
		if(device_count==0)
		{
			//no connection
			sleep(1);
			continue;
		}
		for (i = 0; i < device_count; i++)
		{
			if(max_soc_id<sys.devices[i]->socket_id){
				max_soc_id = sys.devices[i]->socket_id;
			}
			FD_SET(sys.devices[i]->socket_id, &fdread);
			FD_SET(sys.devices[i]->socket_id, &fderror);
		}
		FD_SET(main_socket, &fdread);
		FD_SET(main_socket, &fderror);
		DBG("device_connection_handler_thread:waiting com data...\r\n");
		ret = select(max_soc_id+1, &fdread, &fdwrite, &fderror, /*&tv*/NULL);
		if (ret <= 0)
		{
			printf("Device thread:Time expired or Something error.\r\n",ret);
			//Time expired
			//continue;
		} 
		//main thread have new socket
		//if (FD_ISSET(main_socket, &fdread))continue;
		//printf("line:%d\r\n",__LINE__);
		
		//check event
		for (i = 0; i < device_count; i++)
		{
			//error
			if (FD_ISSET(sys.devices[i]->socket_id, &fderror))
			{
				printf("Connection ERROR!! error no=%d\r\n",errno);
				close(sys.devices[i]->socket_id);//SD_SEND);
				sys.devices[i]->socket_id = -1;
				continue;
			}
			//recv
			if (FD_ISSET(sys.devices[i]->socket_id, &fdread))
			{
				// A read event happened on g_CliSocketArr
				ret = recv(sys.devices[i]->socket_id, temp_buf, sizeof(temp_buf), MSG_DONTWAIT);
				if (ret == 0)
				{
					//printf("Connectiong closed\r\n");
					//close(sys.devices[i].socket_id);
					//sys.devices[i].socket_id = -1;
				}else if(ret <= -1){
					printf("Recive data error, remove socket!!\r\n");
					close(sys.devices[i]->socket_id);
					sys.devices[i]->socket_id = -1;
				}else{
					int used = 0;
					DBG("\r\n*****Connection %d********Recived*******Begin*****************\r\n",i);
					shp_dbg_mem(temp_buf,ret);
					sem_wait(&sys.sys_sem);
					sys.devices[i]->timeout_second = 0;
					used = sys.devices[i]->handle_recieved_data((U8*)temp_buf,ret);
					sem_post(&sys.sys_sem);
					DBG("Handled %d Byte data\r\n",used);
					DBG("\r\n*****Connection %d********Recived*******End*******************\r\n",i);
				}
			}
		}
		sem_wait(&sys.sys_sem);
		sys.RemoveInvalidDevice();
		sem_post(&sys.sys_sem);
		#else
		/*************JUST FOR TEST**************************/{
		int used = 0;
		extern CDeviceTest_TTTTTT* getTest(int index);
		CDeviceTest_TTTTTT * tester = NULL;
		int  i = 0;
    	static CDeviceConnection dev;
		for(i=0;i<2;i++){
			DBG("\r\n******************Recived*******Begin*****************\r\n");
			tester = getTest(i);
			shp_dbg_mem(tester->s0_bin,tester->s0_len);
			sem_wait(&sys.sys_sem);
			used = dev.handle_recieved_data((U8*)tester->s0_bin,tester->s0_len);
			sem_post(&sys.sys_sem);
			DBG("Handled %d Byte data\r\n",used);
			DBG("\r\n******************Recived*******End*******************\r\n");
			sleep(2);
			DBG("\r\n******************Recived*******Begin*****************\r\n");
			tester = getTest(i);
			shp_dbg_mem(tester->s1_bin,tester->s1_len);
			sem_wait(&sys.sys_sem);
			used = dev.handle_recieved_data((U8*)tester->s1_bin,tester->s1_len);
			sem_post(&sys.sys_sem);
			DBG("Handled %d Byte data\r\n",used);
			DBG("\r\n******************Recived*******End*******************\r\n");
			sleep(2);
		}
		/*************JUST FOR TEST**************************/}
		#endif
	}
}


/******************************************************************************
 * FUNCTION NAME:	server_control_thread
 * DESCRIPTION:		This is a thread function for listen PORT;
 					After accept a connect create a socket thread to handle the data
 * --------------------
 *    2013/09/06, Q.Bryan create this function
 ******************************************************************************/
static void* device_connect_thread(void* arg)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
    struct sockaddr_in server_addr;
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	DBG("\"device_connect_thread\" created *SUCCESS*\r\n");
	//initialize
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family 		= AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port		= htons(DEVICE_CONNECT_PORT);

	//open socket
    int main_socket = socket(PF_INET,SOCK_STREAM,0);
    if( main_socket < 0)
    {
        printf("Create Device Socket Failed!\r\n");
        exit(1);
    }

	//set socket option
	{ 
		int opt =1;
		setsockopt(main_socket,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	}

	//bind
    if( bind(main_socket,(struct sockaddr*)&server_addr,sizeof(server_addr)))
    {
        printf("Server Bind Port : %d Failed!", SERVER_CONTROL_PORT); 
        exit(1);
    }

	//listen
    if (listen(main_socket, LENGTH_OF_LISTEN_QUEUE) )
    {
        printf("Server Listen Failed!"); 
        exit(1);
    }

	//create a thread to handle device data
	{
		pthread_t pid;
		pthread_create(&pid,NULL,device_connection_handler_thread,&main_socket);
	}
	//create a thread for each connection
    while (1)
    {
        struct sockaddr_in client_addr;
        socklen_t length = sizeof(client_addr);

		//waiting client to connect
		printf("waiting device to connect\r\n");
        int new_socket = accept(main_socket,(struct sockaddr*)&client_addr,&length);
        if ( new_socket < 0)
        {
            printf("Server Accept Failed!\n");
        }else{
        	CDeviceConnection* dev = new CDeviceConnection();
			dev->socket_id = new_socket;
			sem_wait(&sys.sys_sem);
        	sys.devices.push_back(dev);
			sem_post(&sys.sys_sem);
		}
    }
	//close server socket
    close(main_socket);

	return 0;
}

/******************************************************************************
 * FUNCTION NAME:	com_handler_thread
 * DESCRIPTION:		Convert the COM data to TCP/IP data.
 * --------------------
 *    2013/09/30, Q.Bryan create this function
 ******************************************************************************/
#define COM_PATH	"/dev/ttyS27"
void* com_handler_thread(void*arg)
{
	extern void SetSpeed(int fd, int speed);
	extern int SetParity(int fd, int databits, int stopbits, int parity);
	int				ret = 0;

	DBG("\"com_handler_thread\" created *SUCCESS*\r\n");
	while(1){
		
		//wait a moument
		sleep(5);
		
		/****************com to socket*************begin*************/{
		int fd;
		fd = open(COM_PATH,O_RDWR|O_NOCTTY/*|O_NDELAY*/);
		if(fd < 0)
		{
		    DBG("Open "COM_PATH"failed!!!\r\n");
		}else{
			struct sockaddr_in server_addr;
			int soc = 0;
			//set
			SetSpeed(fd,38400);
			SetParity(fd,8,1,'N');
			//socket
			soc = socket(AF_INET,SOCK_STREAM,0);
			if(soc>0)
			{
				//initialize server_addr
			    bzero(&server_addr,sizeof(server_addr));
			    server_addr.sin_family 		= AF_INET;
			    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
			    server_addr.sin_port		= htons(DEVICE_CONNECT_PORT);
				
				ret = connect(soc,(sockaddr*)&server_addr,sizeof(server_addr));
				if(ret>=0)
				{
					fd_set         	fdread;
					fd_set         	fdwrite;
					fd_set         	fderror;
					
					char buffer[1024];
					int length;
					int readByte;
					
					DBG("Com thread Connect to Server Socket **SUCCESS**\r\n");
					while(1)
					{
						int max_soc_id = fd;
						if(max_soc_id<soc)max_soc_id = soc;
						
						FD_ZERO(&fdread);
						FD_ZERO(&fderror);

						FD_SET(fd,&fdread);
						FD_SET(fd,&fderror);
						FD_SET(soc,&fdread);
						FD_SET(soc,&fderror);
						
						DBG("waiting com data...\r\n");
						//select
						ret = select(max_soc_id+1, &fdread, NULL, &fderror, /*&tv*/NULL);
						if (ret <= 0)
						{
							printf("com_handler_thread:select ->Time expired or Something error.\r\n",ret);
							//Time expired
							//continue;
						} 

						//error
						if (FD_ISSET(fd, &fderror) || FD_ISSET(soc, &fderror)){
							DBG("COM or SOCKET error!!! try once again\r\n");
							break;
						}

						//com data comming
						if(FD_ISSET(fd,&fdread)){
							//read and send to socket
							do{
								DBG("read...\r\n");
								readByte = read(fd, buffer, sizeof(buffer));
								if(readByte>0){
									send(soc,buffer,readByte,0);
								}else{
									break;
								}
							}while(1);
						}

						//soc data comming
						if(FD_ISSET(soc,&fdread)){
							//read and send to com
							do{
								readByte = recv(fd, buffer, sizeof(buffer),MSG_DONTWAIT);
								if(readByte>0){
									write(fd,buffer,readByte);
								}else{
									break;
								}
							}while(1);
						}

						
					}
				}else{
				    DBG("Com thread Connect to Server Socket **FAILED**!!!\r\n");
				}
				close(soc);
			}else{
				DBG("error no.=%d soc=%d\r\n",errno,soc);
			    DBG("Com thread create socket **FAILED**!!!\r\n");
			}
			close(fd);
		}
		/****************com to socket*************end***************/}
	}
	return 0;
}


/******************************************************************************
 * FUNCTION NAME:	shp_update_system_infr_timer_thread
 * DESCRIPTION:		JUST for update systerm infr , emulate as a timer.
 * --------------------
 *    2013/09/17, QBryan create this function
 ******************************************************************************/
static void* shp_update_system_infr_timer_thread(void* arg)
{
	DBG("\"shp_update_system_infr_timer_thread\" created *SUCCESS*\r\n");
	while(1){
		sleep(10);
		sem_wait(&sys.sys_sem);
		//sys.shp_update_system_device_tree_infr_to_file();
		sem_post(&sys.sys_sem);
	}
	return 0;
}


/******************************************************************************
 * FUNCTION NAME:	printf_system_information
 * DESCRIPTION:		Out the system infr to the std output.
 * modification history
 * --------------------
 *    2013/09/10, QBryan create this function
 ******************************************************************************/
static void printf_system_information(FILE* f)
{
	std::string sysTree = sys.dev_tree.toJsonString();
	fprintf(f,"\r\n==================\r\n");
	fprintf(f,"There num of controlor connected : \t%d\r\n",sys.ctls.size());
	fprintf(f,"There num of device connected : \t%d\r\n",sys.devices.size());
	fprintf(f,"The System Tree:\r\n%s",sysTree.c_str());
	fflush(f);
	return ;
}

/******************************************************************************
 * FUNCTION NAME:	shp_timer_handler_for_remove_died_connection
 * DESCRIPTION:		To remove died connection.
 * --------------------
 *    2013/09/10, QBryan create this function
 ******************************************************************************/
static void shp_timer_handler_for_remove_died_connection(int a)
{
	sem_wait(&sys.sys_sem);
	/*********check if timeout or not***********begin*******/{
	std::vector<CDeviceConnection*>::iterator it=sys.devices.begin();
	std::vector<CDeviceConnection*>::iterator end=sys.devices.end();
	while(it!=end)
	{
		if((*it)->socket_id==-1)
		{
			if(*it){
				if((*it)->timeout_second<20){
					(*it)->timeout_second++;
				}else{
					(*it)->socket_id = -1;
				}
			}
		}
		it++;
	}
	sys.RemoveInvalidDevice();
	/*********check if timeout or not***********end*********/}
	
	/*//////////update to file////////begin///////////////////*/{
	sys.shp_update_system_device_tree_infr_to_file();
	/*//////////update to file////////end/////////////////////*/}
	
	sem_post(&sys.sys_sem);
	
	alarm(5);
	return;
}

int main(int argc, char **argv)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	static char cmd_buffer[512];
	pthread_t control_pid;
	pthread_t device_pid;
	pthread_t timer_pid;
	pthread_t com_pid;
	FILE*	  system_infr_fd = 0;	
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	printf("create \"shp_update_system_infr_timer_thread\"\r\n");
	pthread_create(&timer_pid,NULL,shp_update_system_infr_timer_thread,NULL);
	printf("create \"server_control_thread\"\r\n");
	pthread_create(&control_pid,NULL,server_control_thread,NULL);
	printf("create \"device_connect_thread\"\r\n");
	pthread_create(&device_pid,NULL,device_connect_thread,NULL);
	//printf("create \"com_handler_thread\"\r\n");
	//pthread_create(&com_pid,NULL,com_handler_thread,NULL);
	
	/*//set a timeout timer to remove the connection which have no tick//*/{
	signal(SIGALRM,shp_timer_handler_for_remove_died_connection);
	alarm(3);
	/*//set a timeout timer to remove the connection which have no tick//*/}
		
	while(1){
		scanf("%s",cmd_buffer);
		/************print system infro*********begin**************/
		system_infr_fd = fopen("sys_state.log","w+");
		if(system_infr_fd==NULL){
			printf("open sys_state.log failed!!\r\n");
		}else{
			printf("open sys_state.log success!!\r\n");
			printf_system_information(system_infr_fd);
		}
		fclose(system_infr_fd);
		/************print system infro*********end****************/
	}

    return 0;
}

#define LINE_SIZE 16
void shp_dbg_mem(char *data, int len)
{
	static int	shp_dbg_mem_called_count = 0;
	static const char *displayble_chars = 
		"abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"0123456789"
		")!@#$%^&*(`-=~_+"
		"[]{}\\|;':\""
		",./<>?";
	int i = 0;
	char line[LINE_SIZE];
	int j = 0;
	printf(">>>%d>>>>\r\nnow dump %d bytes memory from %p\r\n",shp_dbg_mem_called_count++,len, data);
	if (len <= 0)
		return;
	for (i = 0; i < len; i++)
	{
		line[i % LINE_SIZE] = data[i];
		if (i % LINE_SIZE == 0)
			printf("%08x: ", i);
		printf("%02x ", (unsigned char)data[i]);
		if ((i + 1) % LINE_SIZE == 0)
		{
			for (j = 0; j < LINE_SIZE; j++)
			{
				unsigned char c = '.';
				if (strchr(displayble_chars, line[j]))
					c = line[j];
				if (c == 0)
					c = '.';
				printf("%c", c);
			}
			printf("\r\n");
		}
	}
	//print the last line
	if (len % LINE_SIZE != 0)
	{
		for (i = 0; i < LINE_SIZE - len % LINE_SIZE; i++)
		{
			printf("   ");
		}
		for (i = 0; i < len % LINE_SIZE; i++)
		{

			unsigned char c = '.';
			if (strchr(displayble_chars, line[i]))
				c = line[i];
			if (c == 0)
				c = '.';
			printf("%c", c);
		}
	}
	printf("\r\n----------------------\r\n");	
}


void rever(char s[]){  
    int len=strlen(s);  
    int i=0;  
    int j=len-1;  
    char c;  
    while (i<j)  
    {  
        c=s[i];  
        s[i]=s[j];  
        s[j]=c;  
        i++;  
        j--;  
    }  
} 

char *my_itoa(int val, char *buf, unsigned radix)
{
	char   *p;             
	char   *firstdig;      
	char   temp;           
	unsigned   digval;   
	static char s_itoa[64];

	if(buf == NULL)buf = s_itoa;

	p = buf;
	if(val <0)
	{
		*p++ = '-';
		val = (unsigned long)(-(long)val);
	}
	firstdig = p; 
		do{
		digval = (unsigned)(val % radix);
		val /= radix;

		if  (digval > 9)
			*p++ = (char)(digval - 10 + 'a'); 
		else
			*p++ = (char)(digval + '0');      
	}while(val > 0);

	*p-- = '\0';         
	do{
		temp = *p;
		*p = *firstdig;
		*firstdig = temp;
		--p;
		++firstdig;        
	}while(firstdig < p);  
	return buf;
}

int my_memcmp(void* mem1,void*mem2,int size)
{
	char* p1=(char*)mem1;
	char* p2=(char*)mem2;
	
	while(size && *p1==*p2){size--;p1++;p2++;}
	
	return size;
}

