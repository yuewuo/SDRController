#include "TestsClass.h"

TestsClass::TestsClass(QObject *parent, SDRMain * m)
	: QObject(parent)
{
	sdr = m;
	qDebug() << "TestsClass Initialized";
}

TestsClass::~TestsClass()
{
}

void TestsClass::TcpLink(QString IP, int port)
{
	socket = new QTcpSocket();
	connect(socket, SIGNAL(readyRead()), this, SLOT(socket_Read_Data()));
	socket->connectToHost(IP, port);
	if (!socket->waitForConnected(3000)) {
		qDebug() << "Connection failed!";
	}
	socket->write("hhhh");
	socket->flush();
	qDebug() << "Connect successfully!";
	//delete socket;
}

void TestsClass::socket_Read_Data() {
	QByteArray buffer;
	if (socket->isValid()) {
		qDebug() << "valid";
		qDebug() << socket->peerAddress().toString() << ' ' << socket->peerPort();
	}
	buffer = socket->readAll();
	if (!buffer.isEmpty()) {
		qDebug() << tr(buffer);
	}
}