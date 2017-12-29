#pragma once

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

using std::vector;
using std::map;
using std::set;
using std::string;

class SwitchesShow : public QWidget
{
	Q_OBJECT

public:
	SwitchesShow(QWidget *parent = Q_NULLPTR);
	~SwitchesShow();
	bool isAutoFitToShow = true;
	int importConfig(string jsonstr);
	string exportConfig();

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
		mutable int lineStates[3] = { 0, 0, 0 };
		vector<double> giveme3line() const;
		QColor getColor(int lineno) const;
		bool operator < (const Switch3Line& l) const;
	};
	struct PluginDevice {
		QString name;
		IntPoint position;
		QString descriptor; // this could be modified, just for human reading
		mutable int state = 0;
		bool operator < (const PluginDevice& p) const;
		QColor getColor() const;
	};
	set<Switch3Line> lines;
	set<PluginDevice> plugins;
	void lockDataMutex();
	void unlockDataMutex();

protected:
	void paintEvent(QPaintEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *e);

private:
	set<IntPoint> paintedNodes;
	QMutex mutex; // must lock this one to operate data
	Ui::SwitchesShow ui;
	vector<double> getPolygons(const IntPoint& a, const IntPoint& b);
	void autoRotateSwitch3Lines(const Switch3Line& l);
	double toPixelX(double normalx);
	double toPixelY(double normaly);
	void autoFitToShow(); // must call this when mutex is locked
};
