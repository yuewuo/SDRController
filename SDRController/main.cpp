#include <QtWidgets/QApplication>
#include "SDRMain.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SDRMain w;
	w.show();
	return a.exec();
}
