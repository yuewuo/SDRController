#include "SwitchesShow.h"
// you can set those data
static double gap3line = 0.08;
static double gapcenterline = 0.25;
static double normToPixel = 100;
static double biasX = 100;
static double biasY = 100;

#ifdef QT_DEBUG
int SDRprintf(const char* format, ...)
{
	char buf[1024];
	int i;
	va_list vlist;
	va_start(vlist, format);
	i = vsprintf_s(buf, format, vlist);
	va_end(vlist);
	qDebug() << buf;
	return i;
}
#endif

SwitchesShow::SwitchesShow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	mutex.lock();
	Switch3Line tmp;
	tmp.a.alpha = 0;
	tmp.a.beta = 0;
	tmp.b.alpha = 1;
	tmp.b.beta = 0;
	lines.push_back(tmp);
	tmp.a.alpha = 0;
	tmp.a.beta = 0;
	tmp.b.alpha = 0;
	tmp.b.beta = 1;
	tmp.lineStates[1] = 1;
	lines.push_back(tmp);
	mutex.unlock();
}

SwitchesShow::~SwitchesShow()
{
}

void SwitchesShow::paintEvent(QPaintEvent * event)
{
	Q_UNUSED(event);

	QPainter painter(this);
	painter.setPen(QColor(0, 160, 230));
	painter.setRenderHint(QPainter::Antialiasing, true);
	QFont font;
	font.setFamily("Microsoft YaHei");
	font.setPointSize(50);
	font.setItalic(true);
	painter.setFont(font);

	painter.drawText(rect(), Qt::AlignCenter, "Qt");

	mutex.lock();
	//painter.setBrush
	for (int i = 0; i < lines.size(); ++i) {
		std::vector<double> pl = lines[i].giveme3line();
		for (int j = 0; j + 3 < pl.size(); j += 4) {
			painter.setPen(QPen(lines[i].getColor(j / 4), 3));
			painter.drawLine(biasX + normToPixel * pl[j], biasY + normToPixel * pl[j+1]
				, biasX + normToPixel * pl[j+2], biasY + normToPixel * pl[j+3]);
		}
	}
	mutex.unlock();
}

void SwitchesShow::mouseDoubleClickEvent(QMouseEvent * e)
{
	SDRprintf("mouseDoubleClickEvent");
}

double SwitchesShow::IntPoint::x()
{
	return alpha + ((double)beta) / 2;
}

double SwitchesShow::IntPoint::y()
{
	return ((double)beta) / 2 * sqrt(3);
}

std::vector<double> SwitchesShow::Switch3Line::giveme3line()
{
	std::vector<double> pl; // Ax1, Ay1, Ax2, Ay2, Bx1, By1, Bx2, By2, Cx1, Cy1, Cx2, Cy2, 12 elements
	double ax = a.x(), ay = a.y();
	double bx = b.x(), by = b.y();
	double deltaX = gap3line * (ay - by);
	double deltaY = gap3line * (bx - ax);
	// middle
	double x1 = ax + gapcenterline * (bx - ax);
	pl.push_back(x1);
	double y1 = ay + gapcenterline * (by - ay);
	pl.push_back(y1);
	double x2 = bx + gapcenterline * (ax - bx);
	pl.push_back(x2);
	double y2 = by + gapcenterline * (ay - by);
	pl.push_back(y2);
	// left
	pl.push_back(x1 + deltaX);
	pl.push_back(y1 + deltaY);
	pl.push_back(x2 + deltaX);
	pl.push_back(y2 + deltaY);
	// right
	pl.push_back(x1 - deltaX);
	pl.push_back(y1 - deltaY);
	pl.push_back(x2 - deltaX);
	pl.push_back(y2 - deltaY);
	return pl;
}

QColor SwitchesShow::Switch3Line::getColor(int lineno)
{
	if (lineno < 0 || lineno > 2) {
		SDRprintf("error %s(%d): lineno = %d\n", __FILE__, __LINE__, lineno);
		return QColor("red");
	}
	if (lineStates[lineno] == 0) {
		return QColor("grey");
	}
	if (lineStates[lineno] == 1) {
		return QColor("blue");
	} 
	else {
		SDRprintf("error %s(%d): lineStates[lineno] = %d", __FILE__, __LINE__, lineStates[lineno]);
		return QColor("red");
	}
}
