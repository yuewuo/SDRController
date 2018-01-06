#include "SDRMain.h"

/*
 * SDRMain负责三个工作：
 *	1. 即时响应ui操作，包括导入导出config文件。
 *  2. 定时计算布线方案并更新ui中数据结构。
 *  3. 即时响应esp32进一步通信的逻辑控制。
 */

SDRMain::SDRMain(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	timer = new QTimer(this);
	connect(ui.pushButton_importconfig, SIGNAL(clicked()), this, SLOT(pushButton_importconfig_onclick()));
	connect(ui.pushButton_exportconfig, SIGNAL(clicked()), this, SLOT(pushButton_exportconfig_onclick()));
	connect(ui.pushButton_changLine, SIGNAL(clicked()), this, SLOT(pushButton_changLine_onclick()));
	connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
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
	if (timer->isActive()) timer->stop();
	QByteArray bytes = file.readAll();
	QString str(bytes);
	string json = str.toStdString();
	if (0 != ui.switchesShowWidget->importConfig(json)) {
		QMessageBox::about(NULL, "ERROR", "Configure file error");
	}
	else
		timer->start(timer_interval);
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

// TODO：新布线算法
void SDRMain::onTimeout()
{
	arr.lockDataMutex();
	arr.NofCharging = arr.chargeList.size();
	arr.coilList.assign(arr.chargeList.begin(), arr.chargeList.end());  // 充电
	arr.coilList.insert(arr.coilList.end(), arr.commuList.begin(), arr.commuList.end());  // 通信
	// TODO：扫描
	arr.unlockDataMutex();

	ui.switchesShowWidget->lockDataMutex();
	arr.initiation(ui.switchesShowWidget);  // ui的线路信息和源信息
	arr.build(ui.switchesShowWidget);  // controller的汇信息
	arr.flow(ui.switchesShowWidget);  // 布线并更新ui
	ui.switchesShowWidget->unlockDataMutex();
}