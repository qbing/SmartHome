#ifndef SMARTHOMEMODIS_H
#define SMARTHOMEMODIS_H

#include <QtGui/QMainWindow>
#include "ui_smarthomemodis.h"
#include <stdio.h>
#include <stdlib.h>
#include <QtDebug>
#include <ActiveQt/QAxWidget>
#include <QMovie>

#include "SHP_example.h"
extern "C" void sendData(U8* bin,U16 size);

#include "..\..\Terminator\by_noOS\Src\framework\framework.h"
#include "system_comm.h"

class SmartHomeModis : public QMainWindow
{
	Q_OBJECT

public:
	SmartHomeModis(QWidget *parent = 0, Qt::WFlags flags = 0);
	~SmartHomeModis();
	QMovie* fan1_movie; 
	QMovie* fan2_movie; 
public:
	void operate(char dev_id,char op_type,void* op_data);
	void AddDebugStr(QString txt);
	void ClearDebugStr(void);
	void disp_temperature(int temp);
private:
	Ui::SmartHomeModisClass ui;
public:
	QString debugStr;

};

#endif // SMARTHOMEMODIS_H
