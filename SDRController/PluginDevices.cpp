#include "PluginDevices.h"
using namespace Plugin;

PluginBase::PluginBase(QObject * parent) : QObject(parent)
{
	classname = "PluginBase";
}

PluginBase::~PluginBase()
{
}

QString Plugin::PluginBase::className()
{
	return classname;
}

PN532::PN532(QObject * parent) : PluginBase(parent)
{
	classname = "PN532";
}

PN532::~PN532()
{
}

QiCharging::QiCharging(QObject * parent) : PluginBase(parent)
{
	classname = "QiCharging";
}

QiCharging::~QiCharging()
{
}

