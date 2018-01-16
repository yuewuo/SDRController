#pragma once

/*
 * using WERZXC to generate lines, using long press to delete line
 */

#include <QWidget>
#include <QPainter>
#include <QMutex>  
#include <vector>
#include <stack>
#include <set>
#include <map>
#include <cstring>
#include <QDebug>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMouseEvent>
#include <QMessageBox>
#include "ui_SwitchesShow.h"
#include "SDRVersion.h"
#include <cmath>
#include <QKeyEvent>
#include <QTcpSocket>
#include <QHostAddress>
#include "duhttp.h"
#include <QStandardItemModel>
#include <QTreeView>
#include <QAbstractSocket>

using std::vector;
using std::map;
using std::set;
using std::string;

class SwitchesShow;

struct IntPoint {
	int alpha = 0;
	int beta = 0;
	double x() const;
	double y() const;
	bool operator < (const IntPoint& p) const;
};

struct Switch3Line {
	mutable IntPoint a; // it's dangerous, but OK
	mutable IntPoint b;
	int bindExtId;
	QString bindExtGPIO;
	mutable int lineStates[3] = { 0, 0, 0 };
	vector<double> giveme3line() const;
	QColor getColor(int lineno) const;
	bool operator < (const Switch3Line& l) const;
};

class ESP32: public QObject{
	Q_OBJECT
private:
	mutable QTcpSocket socket;
	mutable DuHttpReceiver duHttpReceiver;
	mutable DuHttp duHttp;
	mutable DuHttp sendDuHttp;
	static const int SENDBUFSIZE = 4096;
	mutable char sendbuf[SENDBUFSIZE];
public:
	QHostAddress addr;
	int port = 80;
	int id;
	QString name;
	QString bootscript;
	ESP32();
	ESP32(const ESP32 & x);
	~ESP32();
	bool tryConnect(int timeout = 3000) const;
	bool operator < (const ESP32& x) const;
	void reboot() const;
	void probeAndSync(const SwitchesShow& ss) const;
	void proxyTest() const;

	friend class SwitchesShow;
private slots:
	void socket_Read_Data();
	void fetchInfo();
};

struct GPIOExt {
	mutable set<QString> binded; // binded ports
	int id;
	QString bindType; // e.g. "ESP32"
	int bindId; // e.g. 1
	QString bindMethod; // e.g. "UART1", "SPI1", "GPIO7"
	bool operator < (const GPIOExt& p) const;
};

struct PluginDevice {
	QString name;
	IntPoint position;
	QString descriptor; // this could be modified, just for human reading
	mutable int state = 0;
	bool operator < (const PluginDevice& p) const;
	QColor getColor() const;
	QString bindType; // e.g. "ESP32"
	int bindId; // e.g. 1
	QString bindMethod; // e.g. "uart1", "spi1", "GPIO7"
};

class SwitchesShow : public QWidget
{
	Q_OBJECT

public:
	SwitchesShow(QWidget *parent = Q_NULLPTR);
	~SwitchesShow();
	bool isAutoFitToShow = true;
	int importConfig(string jsonstr);
	string exportConfig();

	set<Switch3Line> lines;
	set<PluginDevice> plugins;
	set<ESP32> esp32s;
	set<GPIOExt> gpioexts;
	void lockDataMutex();
	void unlockDataMutex();

	void enabletreeWidget_ESP32s(QTreeView * treeView_ESP32s);

	static void autoRotateSwitch3Lines(const Switch3Line& l);

protected:
	void paintEvent(QPaintEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *e);
	void keyPressEvent(QKeyEvent  *event);
	void keyReleaseEvent(QKeyEvent  *event);

private:
	int keypressed;
	QTime keypressedTime;
	bool isfocusIntPoint = false;
	IntPoint focusedIntPoint;
	set<IntPoint> paintedNodes;
	QMutex mutex; // must lock this one to operate data
	Ui::SwitchesShow ui;
	vector<double> getPolygons(const IntPoint& a, const IntPoint& b);
	double toPixelX(double normalx);
	double toPixelY(double normaly);
	void autoFitToShow(); // must call this when mutex is locked
	void extraInit();
	QTreeView * treeView_ESP32s = NULL;
	void updatetreeWidget_ESP32s();
};
