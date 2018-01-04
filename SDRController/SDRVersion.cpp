#include "SDRVersion.h"

const static char version_name[] = "SDRController V0.1";

SDRVersion::SDRVersion(QObject *parent)
	: QObject(parent)
{
}

SDRVersion::~SDRVersion()
{
}

QString SDRVersion::version()
{
	return QString(version_name);
}

void DuHttp_ELOG(const char* str) {

}