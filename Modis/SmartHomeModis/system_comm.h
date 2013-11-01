#ifndef __SYSTEM_COMM_H__
#define __SYSTEM_COMM_H__
#include <QtGui/QApplication>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QThread>
#include <QTimer>


//#define SERVER_HOST_IP_ADDRESS		"10.239.83.182"
#define SERVER_HOST_IP_ADDRESS		"127.0.0.1"
//#define SERVER_HOST_IP_ADDRESS		"192.168.0.106"
#define SERVER_HOST_PORT			(9595)

#include "smarthomemodis.h"
#define DBG	qDebug

#if 1
///////////////////////////////////////临时代码/////////////////////////begin//////////////////////
extern System sys;
extern void sendData(U8* data,U16 size);
///////////////////////////////////////临时代码///////////////////////end////////////////////////
#endif

class ModisSocket:public QThread
{
	Q_OBJECT
public:
	ModisSocket(QTcpSocket * tcpSoc);
	~ModisSocket();
public:
	QTcpSocket * tcpSoc;
	char *	recv_buffer;
	int		recved_len;
	int     used_len;
	char *	send_buffer;
	int		sended_len;
	int		unsended_len;
	RECV_DATA_T rd;
	const static int RECV_BUFFER_LEN = 1024;
	const static int SEND_BUFFER_LEN = 1024;
public:
	RECV_HANDLER_T recived_data_handler;
	void SetHandler(RECV_HANDLER_T recived_data_handler);
public:
	void run(void);
	void sendData(U8* data,U16 size);
public slots:
	void readyRead(void);
};


class ModisTcpServer:public QTcpServer
{
	Q_OBJECT
public:
	ModisSocket* modisSoc;
public slots:
	void newConnection(void)
	{
		QTcpSocket * tcpSoc = NULL;
		while(hasPendingConnections())//do
		{
			tcpSoc = nextPendingConnection();
			qDebug("Accept connect " + tcpSoc->peerAddress().toString());
			modisSoc = new ModisSocket(tcpSoc);
			//modisSoc->start();
			extern U8 recived_data_handler(RECV_DATA_T* r_data);
			modisSoc->SetHandler(recived_data_handler);
		}
		QString e_str = errorString();
	}
};

class SystemTimer:public QObject
{
	Q_OBJECT
public:
	SystemTimer()
	{	
		QTimer *timer = new QTimer(this);
		connect(timer, SIGNAL(timeout()), this, SLOT(update()));
		timer->start(1000);
		
	}
public slots:
	void update(void);
};//SystemTimer

#endif//__SYSTEM_COMM_H__