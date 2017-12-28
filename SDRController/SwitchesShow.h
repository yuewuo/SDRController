#pragma once

#include <QWidget>
#include <QPainter>
#include <QMutex>  
#include <vector>
#include <stack>
#include <QDebug>
#include <QVBoxLayout>
#include <QPushButton>
#include "ui_SwitchesShow.h"

class SwitchesShow : public QWidget
{
	Q_OBJECT

public:
	SwitchesShow(QWidget *parent = Q_NULLPTR);
	~SwitchesShow();
	struct IntPoint {
		int alpha = 0;
		int beta = 0;
		double x();
		double y();
	};
	struct Switch3Line {
		IntPoint a;
		IntPoint b;
		int lineStates[3] = { 0, 0, 0 };
		std::vector<double> giveme3line();
		QColor getColor(int lineno);
	};
	std::vector<Switch3Line> lines;
	QMutex mutex; // must lock this one to operate data

protected:
	void paintEvent(QPaintEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *e);

private:
	Ui::SwitchesShow ui;
};
