#ifndef ARRANGE_H
#define ARRANGE_H
#define ALGORITHM2
#include "SwitchesShow.h"

using namespace std;

const int NofDirection = 6;
const int NofPluginType = 2;
const int NofSwitchPerLine = 3;

const int Switch2Hub[NofSwitchPerLine][NofDirection] = {
	{2, 2, 0, 0, 1, 2},
	{1, 1, 1, 1, 0, 0},
	{0, 0, 2, 2, 2, 1}
};

const int Hub2Switch[NofSwitchPerLine][NofDirection] = {
	{2, 2, 0, 0, 1, 1},
	{1, 1, 1, 1, 0, 2},
	{0, 0, 2, 2, 2, 0}
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

struct Coil_t
{
	Point_t o;
	int pattern;
	bool fresh;
	bool operator < (const struct Coil_t) const;
};

class Arrange_t
{
public:
	Arrange_t();

	int NofCharging;
	vector<Coil_t> chargeList;
	vector<Coil_t> commuList;
	vector<Coil_t> coilList;

	void initiation(SwitchesShow* ss);
	void build(SwitchesShow* ss);
	void flow(SwitchesShow* ss);
	void plot(SwitchesShow* ss);

	void arrange_algorithm(SwitchesShow* ss);
	void lockDataMutex();
	void unlockDataMutex();

private:
	QMutex mutex; // must lock this one to operate data
	pair<QString, int> dfs(SwitchesShow *ss, int x, int inDir);
};

#endif
