#pragma once

#include <QObject>

namespace Plugin {

	class PluginBase : public QObject {
		Q_OBJECT
	protected:
		QString classname;
	public:
		PluginBase(QObject *parent);
		~PluginBase();
		QString className();
	};

	class PN532 : public PluginBase {
		Q_OBJECT
	public:
		PN532(QObject *parent);
		~PN532();
		void startScan();
		void getScanResult();
	};

	class QiCharging : public PluginBase {
		Q_OBJECT
	public:
		QiCharging(QObject *parent);
		~QiCharging();
	};
}