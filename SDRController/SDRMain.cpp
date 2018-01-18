#include "SDRMain.h"

/*
 * SDRMain负责三个工作：
 *	1. 即时响应ui操作，包括导入导出config文件。（已完成）
 *  2. 定时计算布线方案并更新ui中数据结构。（TODO:新布线算法/加入扫描线圈）
 *  3. 即时响应esp32进一步通信的逻辑控制。
 *		0).	布线
 *		1). 给充电/扫描设备发信号，设置模式，启动
 *		2).	充电设备离开
 *		3).	扫描到新设备，扫描线圈变通信线圈
 *		4).	读卡成功，解析tag信息，通信线圈变充电线圈
 *		5).	读卡失败，输出到log，告知用户，删除通信线圈
 */

SDRMain::SDRMain(QWidget *parent)
	: QMainWindow(parent)
{
	testUnit = new TestsClass(this, this);
	ui.setupUi(this);
	ui.switchesShowWidget->enabletreeWidget_ESP32s(ui.treeView_ESP32s);
	timer = new QTimer(this);
	connect(ui.pushButton_importconfig, SIGNAL(clicked()), this, SLOT(pushButton_importconfig_onclick()));
	connect(ui.pushButton_exportconfig, SIGNAL(clicked()), this, SLOT(pushButton_exportconfig_onclick()));
	connect(ui.pushButton_changLine, SIGNAL(clicked()), this, SLOT(pushButton_changLine_onclick()));
	connect(ui.pushButton_testButton, SIGNAL(clicked()), this, SLOT(pushButton_testButton_onclick()));
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
		timer->start(0);
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
		set<Switch3Line> & lines = ui.switchesShowWidget->lines;
		Switch3Line tmp;
		tmp.a.alpha = x1; tmp.a.beta = y1;
		tmp.b.alpha = x2; tmp.b.beta = y2;
		set<Switch3Line>::iterator fnd = lines.find(tmp);
		if (fnd != lines.end()) {
			fnd->lineStates[num] = stat;
		}
		ui.switchesShowWidget->unlockDataMutex(); // must unlock (it also auto refresh!)
	} else {
		QMessageBox::about(NULL, "Tips", "input integer (x1, y1)(x2, y2), the num=0,1,2, stat\nnot");
	}
}

void SDRMain::pushButton_testButton_onclick()
{
	//testUnit->TcpLink("127.0.0.1", 666);
	ui.switchesShowWidget->lockDataMutex();
	//ui.switchesShowWidget->esp32s.begin()->reboot(); // test OK
	ui.switchesShowWidget->esp32s.begin()->proxyTest();
	ui.switchesShowWidget->unlockDataMutex(); // must unlock (it also auto refresh!)
} 
int timeout_cnt = 0;
// TODO：新布线算法
void SDRMain::onTimeout()
{
	timer->stop();

	arr.lockDataMutex();

	arr.commuList.clear();
	arr.chargeList.clear();
	if (timeout_cnt  == 0)
	{
		arr.commuList.push_back(Coil_t{ Point_t(2, 0), 3, 1 });  // for debug
		arr.chargeList.push_back(Coil_t{ Point_t(3, 5), 6, 1 });  // for debug
	}
	else
	{
		arr.commuList.push_back(Coil_t{ Point_t(2, 0), 3, 0 });  // for debug
		arr.chargeList.push_back(Coil_t{ Point_t(3, 5), 6, 0 });  // for debug
		//arr.chargeList.push_back(Coil_t{ Point_t(5, 15), 12, 1 });
	}
	if (timeout_cnt % 4 != 0)
		arr.chargeList.push_back(Coil_t{ Point_t(8, 7), -3, 0 });  // for debug
	arr.commuList.push_back(Coil_t{ Point_t(5, timeout_cnt%10), 3, 1 });
	arr.NofCharging = arr.chargeList.size();
	arr.coilList.assign(arr.chargeList.begin(), arr.chargeList.end());  // 充电
	arr.coilList.insert(arr.coilList.end(), arr.commuList.begin(), arr.commuList.end());  // 通信
	// TODO：扫描
	arr.unlockDataMutex();

	ui.switchesShowWidget->lockDataMutex();
	arr.initiation(ui.switchesShowWidget); // ui的线路信息和源信息
	arr.arrange_algorithm(ui.switchesShowWidget);
	ui.switchesShowWidget->unlockDataMutex();
	timer->start(timer_interval);

	timeout_cnt++;
}

void SDRMain::FinishedCharging()
{
}