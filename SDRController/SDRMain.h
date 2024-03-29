#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SDRMain.h"
#include "SDRVersion.h"
#include "Arrange.h"
#include <QFile>
#include <QTimer>
#include <QMessageBox>
#include <cstring>
#include <TestsClass.h>
using std::string;

const int timer_interval = 5000;

class SDRMain : public QMainWindow
{
	Q_OBJECT

public:
	SDRMain(QWidget *parent = Q_NULLPTR);

public slots:
	void pushButton_importconfig_onclick();
	void pushButton_exportconfig_onclick();
	void pushButton_changLine_onclick();
	void pushButton_testButton_onclick();
	void FinishedCharging();
	void onTimeout();
	friend class TestsClass;

private:
	Ui::SDRMainClass ui;
	Arrange_t arr;
	QTimer *timer;
	TestsClass *testUnit;
};
