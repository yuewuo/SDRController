#pragma once

#include <QObject>
#include <SDRMain.h>
#include <QDebug>
#include <QTcpSocket>
#include <QHostAddress>

class SDRMain;
class TestsClass : public QObject
{
	Q_OBJECT

private:
	QTcpSocket* socket;
	SDRMain* sdr;

public:
	TestsClass(QObject *parent, SDRMain * m);
	~TestsClass();
	void TcpLink(QString IP, int port);

private slots:
	void socket_Read_Data();
};
