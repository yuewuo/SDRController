#include "SDRMain.h"

SDRMain::SDRMain(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.pushButton_importconfig, SIGNAL(clicked()), this, SLOT(pushButton_importconfig_onclick()));
	connect(ui.pushButton_exportconfig, SIGNAL(clicked()), this, SLOT(pushButton_exportconfig_onclick()));
	connect(ui.pushButton_changLine, SIGNAL(clicked()), this, SLOT(pushButton_changLine_onclick()));
}

/* 
 * 这两个函数用来加载和输出配置文件，添加设备可以通过配置文件或是程序修改
 */

void SDRMain::pushButton_importconfig_onclick()
{
	QFile file("switches.json");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::about(NULL, "ERROR", "Cannot open \"switches.json\" for reading");
		return;
	}
	QByteArray bytes = file.readAll();
	QString str(bytes);
	string json = str.toStdString();
	if (0 != ui.switchesShowWidget->importConfig(json)) {
		QMessageBox::about(NULL, "ERROR", "Configure file error");
	}
}

void SDRMain::pushButton_exportconfig_onclick()
{
	QFile file("switches.json");
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QMessageBox::about(NULL, "ERROR", "Cannot open \"switches.json\" for writing");
		return;
	}
	file.write(ui.switchesShowWidget->exportConfig().c_str());
}

/*
 * 这个函数用来演示改变边的状态
 * 需要注意的是：必须使用lock和unlock来确保多线程的安全
 */

void SDRMain::pushButton_changLine_onclick()
{
	bool ok = true;
	int x1, y1, x2, y2, num, stat;
	if (ok) x1 = ui.lineEdit_changLinex1->text().toInt(&ok);
	if (ok) y1 = ui.lineEdit_changLiney1->text().toInt(&ok);
	if (ok) x2 = ui.lineEdit_changLinex2->text().toInt(&ok);
	if (ok) y2 = ui.lineEdit_changLiney2->text().toInt(&ok);
	if (ok) {
		num = ui.lineEdit_changLinenum->text().toInt(&ok);
		if (ok && (num < 0 || num > 2)) ok = false;
	}
	if (ok) stat = ui.lineEdit_changLinestat->text().toInt(&ok);
	if (ok) {
		ui.switchesShowWidget->lockDataMutex(); // must lock when manipulate data
		set<SwitchesShow::Switch3Line> & lines = ui.switchesShowWidget->lines;
		SwitchesShow::Switch3Line tmp;
		tmp.a.alpha = x1; tmp.a.beta = y1;
		tmp.b.alpha = x2; tmp.b.beta = y2;
		set<SwitchesShow::Switch3Line>::iterator fnd = lines.find(tmp);
		if (fnd != lines.end()) {
			fnd->lineStates[num] = stat;
		}
		ui.switchesShowWidget->unlockDataMutex(); // must unlock (it also auto refresh!)
	} else {
		QMessageBox::about(NULL, "Tips", "input integer (x1, y1)(x2, y2), the num=0,1,2, stat\nnot");
	}
}
