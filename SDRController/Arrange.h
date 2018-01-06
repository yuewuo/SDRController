#ifndef ARRANGE_H
#define ARRANGE_H

#include "SwitchesShow.h"

using namespace std;

const int NofDirection = 6;
const int NofPluginType = 2;
const int NofSwitchPerLine = 3;

const int Switch2Hub[NofSwitchPerLine][NofDirection] = {
	{ 2, 0, 0, 1, 2, 2 },
	{ 1, 1, 1, 0, 0, 1 },
	{ 0, 2, 2, 2, 1, 0 }
};

const int Hub2Switch[NofSwitchPerLine][NofDirection] = {
	{2, 0, 0, 1, 1, 2},
	{1, 1, 1, 0, 2, 1},
	{0, 2, 2, 2, 0, 0}
};

struct Point_t
{
	int x, y;
	Point_t() : x(0), y(0) {}
	Point_t(int _x, int _y) : x(_x), y(_y) {}
	Point_t(IntPoint P) : x(P.alpha), y(P.beta) {}
	bool operator < (const struct Point_t) const;
	bool operator == (const struct Point_t) const;
	Point_t neighbour(int direction);
	IntPoint to_IntPoint();
};

struct Edge_t
{
	int r, c, v, next;
	Edge_t() : r(0), c(0), v(0), next(-1) {}
	Edge_t(int _r, int _c, int _v, int _n) : r(_r), c(_c), v(_v), next(_n) {}
};

class Arrange_t
{
public:
	Arrange_t();

	int NofCharging;
	vector<pair<Point_t, int>> chargeList;
	vector<pair<Point_t, int>> commuList;
	vector<pair<Point_t, int>> coilList;

	void initiation(SwitchesShow* ss);
	void build(SwitchesShow* ss);
	void flow(SwitchesShow* ss);

	void lockDataMutex();
	void unlockDataMutex();

private:
	QMutex mutex; // must lock this one to operate data
	pair<QString, int> dfs(SwitchesShow *ss, int x, int inDir);
};

#endif