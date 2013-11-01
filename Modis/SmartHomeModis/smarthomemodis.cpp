#include "smarthomemodis.h"
#include "system_comm.h"

#include "..\..\Terminator\by_noOS\Src\framework\framework.h"



///////////////////////////////////////临时代码///////////////////////////////////////////////
static SmartHomeModis* temp_modis_main=NULL;
extern "C" void operate(DEVICE_ID dev_id,OP_TYPE op_type,void* op_data)
{
	temp_modis_main->operate((char)dev_id,(char)op_type,op_data);
}

extern "C" void disp_temperature(int temp)
{
	temp_modis_main->disp_temperature(temp);
}
///////////////////////////////////////临时代码///////////////////////////////////////////////


SmartHomeModis::SmartHomeModis(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	temp_modis_main = this;

	ui.setupUi(this);

	ui.MAC->setText("MAC:"SHP_MAC_TEMP);
	/*
	QAxWidget *flash = new QAxWidget(ui.Fan1,0); 
	flash->resize(1000,700);
	flash->setControl(QString::fromUtf8("{d27cdb6e-ae6d-11cf-96b8-444553540000}"));
	flash->dynamicCall("LoadMovie(long,string)",0,"./fan.swf");
	flash->show();
*/

	fan1_movie = new QMovie(":/Resources/fan.gif");
	ui.FAN1_lable->setMovie(fan1_movie);
	qDebug()<<"..rrrr.."<<fan1_movie->isValid();
	fan1_movie->start();	
	
	fan2_movie = new QMovie(":/Resources/fan.gif");
	ui.FAN2_lable->setMovie(fan2_movie);
	qDebug()<<"..rrrr.."<<fan2_movie->isValid();
	fan2_movie->start();

	operate(0,0,0);
	operate(1,1,0);
	operate(2,0,0);
	operate(3,1,0);

	////////////////////socket//////////////////////////////////////
#if 0
	ModisTcpServer* svr=new ModisTcpServer();
	svr->listen (QHostAddress::Any,5000);
	connect(svr,SIGNAL(newConnection()),svr,SLOT(newConnection()));
#else

#endif
}

SmartHomeModis::~SmartHomeModis()
{

}

void SmartHomeModis::AddDebugStr(QString txt)
{
	if(debugStr.length() > 1024)
		debugStr.clear();

	debugStr+=txt;
	ui.debugTv->setText(debugStr);
	ui.debugTv->moveCursor(QTextCursor::End);
}
void SmartHomeModis::ClearDebugStr(void)
{
	debugStr="";
	ui.debugTv->setText(debugStr);
}

void SmartHomeModis::operate(char dev_id,char op_type,void* op_data)
{
	QLabel* LEDS[]={ui.LED0,ui.LED1,ui.LED2,ui.LED3};
	//for(int i=0;i<sizeof(LEDS)/sizeof(LEDS[0]);i++)
	if(dev_id>=0 && dev_id<sizeof(LEDS)/sizeof(LEDS[0]))
	{
		if(op_type==1/*OP_OPEN*/){
			LEDS[dev_id]->setPixmap(QPixmap(QString::fromUtf8(":/Resources/on_green.png")));
			switch(dev_id)
			{
			case 0:
				sys.lamb1_state = 1;
				break;
			case 1:
				sys.lamb2_state = 1;
				break;
			case 2:
				sys.cdplayer_state = 1;
				break;
			default:
				break;
			}
		}else{
			LEDS[dev_id]->setPixmap(QPixmap(QString::fromUtf8(":/Resources/off.png")));
			switch(dev_id)
			{
			case 0:
				sys.lamb1_state = 0;
				break;
			case 1:
				sys.lamb2_state = 0;
				break;
			case 2:
				sys.cdplayer_state = 0;
				break;
			default:
				break;
			}
		}
	}
}

void SmartHomeModis::disp_temperature(int temp)
{
	this->ui.lcdNumber->display(temp);
}