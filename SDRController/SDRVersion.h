#pragma once

#include <QObject>

class SDRVersion : public QObject
{
	Q_OBJECT

public:
	SDRVersion(QObject *parent);
	~SDRVersion();

	static QString version();

};
