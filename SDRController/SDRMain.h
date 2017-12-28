#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SDRMain.h"

class SDRMain : public QMainWindow
{
	Q_OBJECT

public:
	SDRMain(QWidget *parent = Q_NULLPTR);

private:
	Ui::SDRMainClass ui;
};
