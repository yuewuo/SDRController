#include "SwitchesShow.h"

#ifdef QT_DEBUG
static int SDRprintf(const char* format, ...)
{
	char buf[1024];
	int i;
	va_list vlist;
	va_start(vlist, format);
	i = vsprintf(buf, format, vlist);
	va_end(vlist);
	qDebug() << buf;
	return i;
}
#else
int SDRprintf(const char* format, ...) {}
#endif
#define SDRerror(format, ...) SDRprintf("error %s(%d)" format , __FILE__, __LINE__, ##__VA_ARGS__)

SwitchesShow::SwitchesShow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setFocusPolicy(Qt::ClickFocus);
	extraInit();
}

SwitchesShow::~SwitchesShow()
{
}

ESP32::ESP32()
{
	DuHttpReceiver_Initialize(&duHttpReceiver);
	DuHttp_Initialize(&duHttp);
	//DuHttpReceiver_AvailableSize(&duHttpReceiver)
}

ESP32::ESP32(const ESP32 & x)
{
	addr = x.addr;
	port = x.port;
	name = x.name;
	id = x.id;
	ESP32();
}

ESP32::~ESP32()
{
	socket.close();
}

bool ESP32::tryConnect(int timeout) const
{
	if (socket.isValid()) return true;
	socket.connectToHost(addr, port);
	if (!socket.waitForConnected(timeout)) {
		qDebug() << "Connection failed!" << addr.toString() << port;
		return false;
	}
	connect(&socket, SIGNAL(readyRead()), this, SLOT(socket_Read_Data()));
	qDebug() << "Connection success!" << addr.toString() << port;
	return true;
}

void ESP32::reboot() const
{
	if (socket.state() != QAbstractSocket::ConnectedState) {
		qDebug() << "reboot called, not ConnectedState" << QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
	}
	qDebug() << "reboot at" << QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
	DuHttp_Initialize_GET(&sendDuHttp, "/exec/reboot");
	DuHttp_PushHeadline(&sendDuHttp, "Connection", "keep-alive");
	DuHttp_EndHeadline(&sendDuHttp);
	int len = DuHttpSend(&sendDuHttp, sendbuf, SENDBUFSIZE);
	socket.write(sendbuf, len);
}

void ESP32::probeAndSync(const SwitchesShow& ss) const
{
	if (socket.state() != QAbstractSocket::ConnectedState) {
		qDebug() << "probeAndSync called, not ConnectedState" << QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
	}
	qDebug() << "probeAndSync at" << QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
	DuHttp_Initialize_POST(&sendDuHttp, "/exec");
	DuHttp_PushHeadline(&sendDuHttp, "Connection", "keep-alive");
	DuHttp_EndHeadline(&sendDuHttp);
	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd,hh:mm:ss.zzz");
	QString timestr = QString("set time ") + QDateTime::currentDateTime().toString("yyyy-MM-dd,hh:mm:ss.zzz") + "\n";
	DuHttp_PushDataString(&sendDuHttp, timestr.toLatin1().data());
	for (auto bg = ss.gpioexts.begin(); bg != ss.gpioexts.end(); ++bg) {
		if (bg->bindType != "ESP32" || bg->bindId != id) continue;
		QString setstr = QString("start proxy ") + bg->bindMethod + "\n";
		DuHttp_PushDataString(&sendDuHttp, setstr.toLatin1().data());
	}
	int len = DuHttpSend(&sendDuHttp, sendbuf, SENDBUFSIZE);
	socket.write(sendbuf, len);
	socket.flush();
}

void ESP32::proxyTest() const
{
	if (socket.state() != QAbstractSocket::ConnectedState) {
		qDebug() << "proxyTest called, not ConnectedState" << QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
	}
	qDebug() << "proxyTest at" << QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
	DuHttp_Initialize_POST(&sendDuHttp, "/exec");
	DuHttp_PushHeadline(&sendDuHttp, "Connection", "keep-alive");
	DuHttp_PushHeadline(&sendDuHttp, "ESPProxyTo", "UART1");
	DuHttp_PushHeadline(&sendDuHttp, "ESPProxyWait", "no-wait");
	DuHttp_EndHeadline(&sendDuHttp);
	DuHttp_PushDataString(&sendDuHttp, "souart clear 777A1 555B14 070D2 flush\n");
	int len = DuHttpSend(&sendDuHttp, sendbuf, SENDBUFSIZE);
	socket.write(sendbuf, len);
}

void ESP32::fetchInfo()
{
	if (socket.state() != QAbstractSocket::ConnectedState) {
		qDebug() << "fetchInfo called, not ConnectedState" << QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
	}
	qDebug() << "fetchInfo at" << QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
	DuHttp_Initialize_POST(&sendDuHttp, "/exec");
	DuHttp_PushHeadline(&sendDuHttp, "Connection", "keep-alive");
	DuHttp_EndHeadline(&sendDuHttp);
	DuHttp_PushDataString(&sendDuHttp, "print name\n");
	DuHttp_PushDataString(&sendDuHttp, "print status\n");
	int len = DuHttpSend(&sendDuHttp, sendbuf, SENDBUFSIZE);
	socket.write(sendbuf, len);
}

void ESP32::socket_Read_Data() {
	QByteArray buffer;
	buffer = socket.readAll();
	if (!buffer.isEmpty()) {
		qDebug() << "socket_Read_Data" << tr(buffer);
		qDebug() << "socket_Read_Data at" << QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
	}
}
