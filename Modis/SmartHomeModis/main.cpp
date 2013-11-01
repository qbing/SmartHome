#include "smarthomemodis.h"
#include <QtGui/QApplication>
#include <QtDebug>
#include <stdio.h>
#include <stdlib.h>

static SmartHomeModis* temp_modis_main=NULL;
void myMessageOutput(QtMsgType type, const char *msg)
{
	static char buffer[2048];
	switch (type) {
		case QtDebugMsg:
			sprintf(buffer, "%s", msg);
			if(temp_modis_main)temp_modis_main->AddDebugStr(buffer);
			fprintf(stderr,buffer);
			break;
		case QtWarningMsg:
			sprintf(buffer, "Warning: %s", msg);
			if(temp_modis_main)temp_modis_main->AddDebugStr(buffer);
			fprintf(stderr,buffer);
			break;
		case QtCriticalMsg:
			sprintf(buffer, "Critical: %s", msg);
			if(temp_modis_main)temp_modis_main->AddDebugStr(buffer);
			fprintf(stderr,buffer);
			break;
		case QtFatalMsg:
			sprintf(buffer, "Fatal: %s", msg);
			if(temp_modis_main)temp_modis_main->AddDebugStr(buffer);
			fprintf(stderr,buffer);
			abort();
		default:
			break;
	}
}

extern "C" void modis_debug(char* msg)
{
	qDebug(msg);
}

#define LINE_SIZE 16
extern "C" void shp_dbg_mem(char *data, int len)
{
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
	qDebug(">now dump %d bytes memory from %p\r\n", len, data);
	if (len <= 0)
		return;
	for (i = 0; i < len; i++)
	{
		line[i % LINE_SIZE] = data[i];
		if (i % LINE_SIZE == 0)
			qDebug("%08x: ", i);
		qDebug("%02x ", data[i]);
		if ((i + 1) % LINE_SIZE == 0)
		{
			for (j = 0; j < LINE_SIZE; j++)
			{
				unsigned char c = '.';
				if (strchr(displayble_chars, line[j]))
					c = line[j];
				if (c == 0)
					c = '.';
				qDebug("%c", c);
			}
			qDebug("\r\n");
		}
	}
	//print the last line
	if (len % LINE_SIZE != 0)
	{
		for (i = 0; i < LINE_SIZE - len % LINE_SIZE; i++)
		{
			qDebug("   ");
		}
		for (i = 0; i < len % LINE_SIZE; i++)
		{

			unsigned char c = '.';
			if (strchr(displayble_chars, line[i]))
				c = line[i];
			if (c == 0)
				c = '.';
			qDebug("%c", c);
		}
	}
	qDebug("\r\n----------------------\r\n");	
}

int main(int argc, char *argv[])
{
	qInstallMsgHandler(myMessageOutput);
	QApplication a(argc, argv);
#if 1
	SmartHomeModis w;
	temp_modis_main = &w;
	new SystemTimer();
	w.show();
	qDebug("===================================================\r\n");
	qDebug("==================QBing SmartHome==================\r\n");
	qDebug("=============%s %s==============\r\n",__DATE__,__TIME__);
	qDebug("===================================================\r\n");
#else
	QAxWidget *flash = new QAxWidget(0,0); 
	flash->resize(1000,700);
	flash->setControl(QString::fromUtf8("{d27cdb6e-ae6d-11cf-96b8-444553540000}"));
	flash->dynamicCall("LoadMovie(long,string)",0,"./fan.swf");
	flash->show();
#endif
	return a.exec();
}

