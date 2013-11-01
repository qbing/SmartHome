/******************************************************************************
 *  Copyright Statement:
 *    Copyright (c)  Q.Bryan <qbing2010@163.com>
 *    All rights reserved.
 *-----------------------------------------------------------------------------
 * Filename:      mod_shp.c
 * Description:   SHP协议使用的 Web 服务器 后台程序
 * Author:        Q.Bryan	2013/09/11 16:27:33
 *=============================================================================
 * HISTORY
 *------Below this line--------------------------------------------------------
 *3.your_name datetime->add_your_change_for_this_file.
 *2.your_name datetime->add_your_change_for_this_file.
 *1.Q.Bryan 2013/09/11 16:27:33->create this file.
 *------Upper this line--------------------------------------------------------
 *=============================================================================
******************************************************************************/


#include "boa.h"
#include "../../../SHP.h"
#include <signal.h>


#undef DBG
#define DBG printf


////////////////////////////////////////////begin/////////////////////////////
#include <pthread.h>
#include <sys/socket.h>
extern void shp_dbg_mem(char *data, int len);
static int server_control_network_send(char* bin,int size);
typedef struct POSTDATA_T
{
	int len;
	char data[4096];
}POSTDATA_T;
typedef struct SystemInfr
{
	pthread_t	network_thread_id;
	int 		soc_id;
	POSTDATA_T  post_data;
}SystemInfr;
SystemInfr sys;
static POSTDATA_T* SmartHome_handler_get_post_data(request *r);
//#define SERVER_CONTROL_IP	"10.239.206.36"
#define SERVER_CONTROL_IP	"127.0.0.1"
#define SERVER_CONTROL_PORT	9596
///////////////////////////////////////////////end///////////////////////////




/************JUST FOR TEST******************begin**************************/
static int SmartHome_TEST(request *r)
{
	printf("\r\n===========================test=====begin==================\r\n");
	printf("request_rec->pathname=%s\r\n",r->pathname);
	printf("request_rec->logline=%s\r\n",r->logline);
	printf("request_rec->header_line=%s\r\n",r->header_line);
	printf("request_rec->http_version=%s\r\n",r->http_version);
	printf("request_rec->path_info=%s\r\n",r->path_info);
	printf("request_rec->path_translated=%s\r\n",r->path_translated);
    
	printf("request_rec->script_name=%s\r\n",r->script_name);
	printf("request_rec->query_string=%s\r\n",r->query_string);
	printf("request_rec->content_type=%s\r\n",r->content_type);
	printf("request_rec->content_length=%s\r\n",r->content_length);
	printf("=====shp_dbg_mem(r->range,r->clength)=======\r\n");
    #if 0
	printf("---shp_dbg_mem(r->buffer,sizeof(buffer))-----");
	shp_dbg_mem(r->buffer,sizeof(r->buffer));
	printf("---shp_dbg_mem(r->request_uri,sizeof(request_uri))-----");
	shp_dbg_mem(r->request_uri,sizeof(r->request_uri));
	printf("request_rec->parse_pos=%d\r\n",r->parse_pos);
	printf("request_rec->client_stream_pos=%d\r\n",r->client_stream_pos);
	printf("request_rec->buffer_start=%d\r\n",r->buffer_start);
	printf("request_rec->buffer_end=%d\r\n",r->buffer_end);
	printf("---shp_dbg_mem(r->client_stream,sizeof(client_stream))-----");
	shp_dbg_mem(r->client_stream,sizeof(r->client_stream));
	printf("---shp_dbg_mem(r->cgi_env,sizeof(cgi_env))-----");
	shp_dbg_mem(r->cgi_env,sizeof(r->cgi_env));
    #else
	/******ap_get_client_block***test*****begin**/{
	POSTDATA_T* post_data = SmartHome_handler_get_post_data(r);
	if(post_data)shp_dbg_mem(post_data->data,post_data->len);
	/******ap_get_client_block***test*****end*****/}
	#endif
	printf("\r\n===========================test=====end==================\r\n\r\n\r\n");

}
/************JUST FOR TEST******************end****************************/




S16  server_control_network_send_package(S8 type,void* bin,S16 len)
{
	int ret = 0;
	SHP_HeadForSend	head={SHP_MAGIC1,sizeof(SHP_HeadForSend),SHP_MAGIC2,type,0};
	head.pkg_len+=len;
	server_control_network_send(&head,sizeof(head));
	ret = server_control_network_send(bin,len);
	DBG("strerrno = %d\r\n",errno);
	return ret;
}

static POSTDATA_T* SmartHome_handler_get_post_data(request *r)
{
	int  ret = 0;
	int  len = 0;
	len = boa_atoi(r->content_length);
    if(len>0)
    {
        memcpy(sys.post_data.data,r->client_stream+r->parse_pos,len);
	}
	sys.post_data.len = len;
	return &sys.post_data;
}


static void SmartHome_handler_handle_post_data(POSTDATA_T* post)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	char* p 	= post->data;
	int   len	= post->len;
	
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	
	if(len<2){
		DBG("The post data is too short to handle,Skiped!!!!!\r\n");
		return ;
	}
	
	////JUST json string//////////////////
	if(*p=='{' && *(p+len-1)=='}'){
		server_control_network_send_package('J',p,len);
	////JUST xml string///////////////////
	}else if(*p=='<' && *(p+len-1)=='>'){
		server_control_network_send_package('X',p,len);	
	////head + json/xml string////////////
	}else if(*p==SHP_MAGIC1 && *(p+2)==SHP_MAGIC2){
		server_control_network_send(p,len);	
	}else{
		DBG("Unknow package!!!!!!!!\r\n");
	}
    DBG("SmartHome_handler_handle_post_data...DONE....\r\n");
	return;
}



/******************************************************************************
 * FUNCTION NAME:	server_control_network_send
 * --------------------
 *    2013/09/06, Q.Bryan create this function
 ******************************************************************************/
static int server_control_network_send(char* bin,int size)
{
	int ret = send(sys.soc_id,bin,size,0);
	if(ret < 0){
		DBG("Send error!!!\r\n");
	}else{
		DBG("Send %d bytes data!!!\r\n",ret);	
	}
	return ret;
}

void server_control_network_thread_signal_func(int sig)
{
	if(sig==SIGKILL){
		printf("server control network thread killed!!!!!!!\r\n");
		close(sys.soc_id);		
	}else{
		printf("server_control_network_thread_signal_func  **be called***\r\n");	
	}
	return ;
}



/******************************************************************************
 * FUNCTION NAME:	server_control_thread
 * DESCRIPTION:		This is a thread function for listen PORT;
 					After accept a connect create a socket thread to handle the data
 * --------------------
 *    2013/09/06, Q.Bryan create this function
 ******************************************************************************/
static void* server_control_network_thread(void* arg)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
    struct sockaddr_in server_addr;
	int ret = 0;
	static char	soc_recv_buffer[2048];
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	signal(SIGKILL,server_control_network_thread_signal_func);
	//initialize
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family 		= AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_CONTROL_IP);
    server_addr.sin_port		= htons(SERVER_CONTROL_PORT);
	
	while(1){
		//create
		printf("Create socket...\r\n");
		if(sys.soc_id)close(sys.soc_id);
		sys.soc_id = socket(PF_INET,SOCK_STREAM,0);
		if( sys.soc_id > 0){
			printf("Create socket success\r\n");
		}else{
			printf("Create Socket Failed!\r\n");
			sys.soc_id = 0;
			sleep(10);
			continue;
		}

	
		printf("Connecting server host...\r\n");
		//connect
		ret = connect(sys.soc_id,&server_addr,sizeof(server_addr));
		if(ret < 0){
			printf("Connect server host failed!!!!\r\n");
			close(sys.soc_id);
			sleep(10);
			continue;			
		}
		printf("Connect server host success!\r\n");

		/////////test///////begin///////
		//while(server_control_network_send("ABC",4)>0){sleep(1);}break;
		/////////test///////begin///////
		
		printf("Recv server data...\r\n");		
		while(1){
			//recv
			ret = recv(sys.soc_id, soc_recv_buffer, sizeof(soc_recv_buffer), 0);
			if(ret < 0){
				printf("recv error,retry...!!!!\r\n");
				close(sys.soc_id);
				sleep(1);
				break;				
			}else if(ret == 0){
				printf("recv timeout,retry...!!!!\r\n");
				close(sys.soc_id);
				sleep(1);
				break;				
				//continue;
			}else if(ret > 0 ){
				printf("!!!!!!!recieved %d bytes data...!!!!\r\n",ret);			
				shp_dbg_mem(soc_recv_buffer,ret);
				printf("!!!!!!!All %d bytes data done!!!!\----------------------\r\n",ret);			
			}
		}
	}
	return 0;
}



/******************************************************************************
 * FUNCTION NAME:	server_control_thread
 * DESCRIPTION:		
                    **MUST** be called when initialize!!!!
 * --------------------
 *    2013/09/06, Q.Bryan create this function
 ******************************************************************************/
int SmartHome_init_service(void* arg)
{
    int ret = 0;
    /************start thread*****begin******************/{
    extern void start_smarthome_service(void);
    ret = pthread_create(&sys.network_thread_id,NULL,server_control_network_thread,NULL); 
    if(ret!=0){
        sys.network_thread_id = 0;
        printf("Create net work thread failed!!!!!!!!\r\n");
    }else{
        printf("Create net work thread SUCCESS!!!!!!!!\r\n");
    }
    /************start thread*****end********************/}
    return ret;
}


int SmartHome_handle(request *r)
{
	
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	POSTDATA_T* post;
    char POST_URL[32];
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
    printf("SmartHome_handle..................!!!\r\n");
    memcpy(POST_URL,r->request_uri,sizeof(POST_URL));
    to_upper(POST_URL);
	shp_dbg_mem(POST_URL,sizeof(POST_URL));
    if(strcmp(POST_URL,"/SMARTHOME")){
        printf("Not SmartHome URL...done!!!\r\n");
        return 0;
    }
    
    #if 1
    SmartHome_TEST(r);
    post = SmartHome_handler_get_post_data(r);
    if(post){
        SmartHome_handler_handle_post_data(post);
    }else{
        DBG("Have no post data!!!\r\n");
        return 0;
    }
    #endif
    return 1;
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
		printf("%02x ", data[i]);
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

