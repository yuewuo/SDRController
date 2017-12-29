#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SDRMain.h"
#include "SDRVersion.h"
#include <QFile>
#include <QMessageBox>
#include <cstring>
using std::string;

class SDRMain : public QMainWindow
{
	Q_OBJECT

public:
	SDRMain(QWidget *parent = Q_NULLPTR);

public slots:
	void pushButton_importconfig_onclick();
	void pushButton_exportconfig_onclick();
	void pushButton_changLine_onclick();

private:
	Ui::SDRMainClass ui;
};
