#include <map>
#include <vector>
#include <assert.h>

#include "Arrange.h"

using namespace std;

const int INF = (1<<30)-1;

// Original Graph
int NofEdge;
int NofPoint;
map<Point_t, int> Point2Id;
vector<Point_t> Id2Point;
vector<pair<int, int>> EdgePair;
map<int, QString> PluginPoint;

// Abstracted Graph
int NofCoil;
int NofNode;
int S, T, N, line;
vector<int> Point2Node;
vector<int> head;
vector<Edge_t> e;

// Max Flow
vector<int> dis;
vector<int> que;
vector<bool> inq;
vector<int> from;
vector<int> link;
map<int, pair<int, int>> route;



// Initialize original graph
void Arrange_t::initiation(SwitchesShow *ss)
{
	qDebug() << "Entered Initiation";

	Point2Id.clear();
	NofEdge = ss->lines.size();
	for (set<Switch3Line>::iterator it = ss->lines.begin(); it != ss->lines.end(); ++it)
	{
		Point2Id.insert(make_pair(Point_t(it->a), 0));
		Point2Id.insert(make_pair(Point_t(it->b), 0));
	}

	Id2Point.clear();
	NofPoint = 0;
	for (map<Point_t,int>::iterator it = Point2Id.begin(); it != Point2Id.end(); ++it)
	{
		it->second = NofPoint++;
		Id2Point.push_back(it->first);
	}

	EdgePair.clear();
	for (set<Switch3Line>::iterator it = ss->lines.begin(); it != ss->lines.end(); ++it)
		EdgePair.push_back(make_pair(Point2Id[Point_t(it->a)], Point2Id[Point_t(it->b)]));

	PluginPoint.clear();
	for (set<PluginDevice>::iterator it = ss->plugins.begin(); it != ss->plugins.end(); ++it)
		PluginPoint.insert(make_pair(Point2Id[Point_t(it->position)], it->name));

	qDebug() << "Finished Initiation";
}

// Build abstracted graph
void addEdge(int x, int y, int c, int v)
{
	e.push_back(Edge_t(y, c, v, head[x]));
	head[x] = line++;
}

vector<int> getCoil(Point_t origin, int pattern)
{
	vector<int> coil;
	if (pattern == 6 || pattern == 12)
	{
		for (int i = 0; i < NofDirection; ++i)
			coil.push_back(Point2Id[origin.neighbour(i)]);
	}
	if (pattern == +3)
	{
		coil.push_back(Point2Id[origin]);
		for (int i = 0; i < 2; ++i)
			coil.push_back(Point2Id[origin.neighbour(i)]);
	}
	if (pattern == -3)
	{
		coil.push_back(Point2Id[origin]);
		for (int i = 3; i < 5; ++i)
			coil.push_back(Point2Id[origin.neighbour(i)]);
	}
	return coil;
}

void Arrange_t::build(SwitchesShow *ss)
{
	qDebug() << "Entered Build";

	Point2Node.clear();
	NofNode = NofPoint;
	for (int i = 0; i < NofPoint; ++i)
		Point2Node.push_back(i);

	NofCoil = coilList.size();
	for (int i = 0; i < NofCoil; ++i)
	{
		vector<int> coil = getCoil(coilList[i].o, coilList[i].pattern);
		int n = coil.size();
		for (int  j = 0; j < n; ++j)
			Point2Node[coil[j]] = NofNode;
		Point2Node.push_back(NofNode++);
	}

	S = 2 * NofNode;
	T = 2 * NofNode + 1;
	N = 2 * NofNode + 2;
	line = 0;
	head.clear();
	for (int i = 0; i < N; ++i)
		head.push_back(-1);
	e.clear();

	for (int i = NofPoint; i < NofNode; ++i)
	{
		addEdge(i+NofNode, T, 1, 0);
		addEdge(T, i+NofNode, 0, 0);
	}
	for (int i = 0; i < NofNode; ++i)
		if (Point2Node[i] == i && PluginPoint.find(i) == PluginPoint.end())  // neither source nor terminal
		{
			addEdge(i, i+NofNode, 1, 0);
			addEdge(i+NofNode, i, 0, 0);
		}
	for (int i = 0; i < NofEdge; ++i)
	{
		int p = EdgePair[i].first;
		int q = EdgePair[i].second;
		if (Point2Node[p] != p && Point2Node[q] != q) continue;
		if (Point2Node[p] != p)
		{
			addEdge(q+NofNode, Point2Node[p], 1, 1);
			addEdge(Point2Node[p], q+NofNode, 0, -1);
		}
		else if (Point2Node[q] != q)
		{
			addEdge(p+NofNode, Point2Node[q], 1, 1);
			addEdge(Point2Node[q], p+NofNode, 0, -1);
		} 
		else
		{
			addEdge(p+NofNode, q, 1, 1);
			addEdge(q, p+NofNode, 0, -1);
			addEdge(q+NofNode, p, 1, 1);
			addEdge(p, q+NofNode, 0, -1);
		}
	}
	for (int i = 0; i < NofPoint; ++i)
		if (Point2Node[i] == i && PluginPoint.find(i) == PluginPoint.end())
		{
			set<QString> nearbyPlugin;
			nearbyPlugin.clear();
			for (int j = 0; j < NofDirection; ++j)
			{
				Point_t Q = Id2Point[i].neighbour(j);
				if (Point2Id.find(Q) == Point2Id.end()) continue;
				int q = Point2Id[Q];
				if (PluginPoint.find(q) != PluginPoint.end())
					nearbyPlugin.insert(PluginPoint[q]);
			}
			if (nearbyPlugin.size() == NofPluginType)
			{
				addEdge(S, i, 1, 0);
				addEdge(i, S, 0, 0);
			}
		}
	qDebug() << "Finished Build";
}

// pattern in {-3, +3, 6, 12}  分别对应单匝反三角、单匝正三角、单匝六边形、双匝六边形
int getTurns(int pattern)
{
	if (pattern == 12) return 2;
	return 1;
}

int getDir(int x, int y)
{
	Point_t P = Id2Point[y];
	for (int i = 0; i < NofDirection; ++i)
		if (Id2Point[x].neighbour(i) == P) return i;
	return -1;
}

int antiDirection(int d)
{
	return (d + 3) % NofDirection;
}

int antiState(int inDir, int outDir, int outState)
{
	if (inDir == -1) return outState;
	int inState = 0;
	for (int i = 0; i < NofSwitchPerLine; ++i)
		if (outState & (1 << i)) inState |= 1 << Hub2Switch[Switch2Hub[i][outDir]][inDir];
	return inState;
}

int circle(vector<int> coil, int hub, bool arrange)
{
	int SwitchId;
	int n = coil.size();
	for (int i = 0; i < n - 1; ++i)
	{
		int p = coil[i];
		int q = coil[i + 1];
		if (i)
		{
			int inDir = getDir(coil[i], coil[i - 1]);
			int outDir = getDir(coil[i], coil[i + 1]);
			hub = Switch2Hub[SwitchId][inDir];
			SwitchId = Hub2Switch[hub][outDir];
			qDebug() << inDir << ' ' << outDir << ' ' << hub << ' ' << SwitchId;
		}
		else
		{
			int outDir = getDir(coil[i], coil[i + 1]);
			SwitchId = Hub2Switch[hub][outDir];
			qDebug() << outDir << ' ' << hub << ' ' << SwitchId;
		}
		if (arrange)
		{
			map<int, pair<int, int>>::iterator it = route.find(p);
			it->second.second |= 1 << SwitchId;
//			qDebug() << it->first << ' ' << it->second.first << ' ' << it->second.second;
		}
	}
	int inDir = getDir(coil[n-1], coil[n-2]);
	hub = Switch2Hub[SwitchId][inDir];
	qDebug() << inDir << ' ' << hub << ' ' << SwitchId;
	return hub;
}

pair<QString, int> Arrange_t::dfs(SwitchesShow *ss, int x, int inDir)
{
	map<int, pair<int, int>>::iterator it = route.find(x);
	if (it == route.end())
	{
		int CoilId = Point2Node[x] - NofPoint; assert(CoilId >= 0);
		QString type = (CoilId < NofCharging) ? "QiDemo" : "PN532";
		int turns = getTurns(coilList[CoilId].pattern);
		vector<int> coil = getCoil(coilList[CoilId].o, coilList[CoilId].pattern);
		int n = coil.size();
		for (int i = 1; i < turns; ++i)
			coil.insert(coil.end(), coil.begin(), coil.begin() + n);
		int start = -1;
		for (int i = 0; i < n; ++i)
		{
			route.insert(make_pair(coil[i], make_pair(coil[i + 1], 0)));
			if (start == -1) coil.push_back(coil[i]);
			if (coil[i] == x) start = i;
		}
		coil.erase(coil.begin(), coil.begin() + start);
		n = coil.size();

		for (start = 0; start < NofSwitchPerLine; ++start)
			if (circle(coil, start, false) != start) break;
		int end = circle(coil, start, true);

		bool vs = false;
		if (inDir == -1)
		{
			vs = true;  // x is virtual source
			for (int i = 0; i < NofDirection; ++i)
			{
				Point_t Q = Id2Point[x].neighbour(i);
				if (Point2Id.find(Q) == Point2Id.end()) continue;
				int q = Point2Id[Q];  // potential real source
				map<int, QString>::iterator fnd = PluginPoint.find(q);
				if (fnd != PluginPoint.end() && fnd->second == type)
				{
					auto ret = route.insert(make_pair(q, make_pair(x, 0)));  // real source
					it = ret.first; inDir = i;
					break;
				}
			}
		}
		int outDir = getDir(coil[0], coil[1]);
		int inDir = getDir(coil[n - 1], coil[n - 2]);
		int state = 1 << Hub2Switch[end][inDir];
		state |= 1 << Hub2Switch[start][outDir];
		qDebug() << start << ' ' << end << ' ' << inDir << ' ' << outDir << ' ' << state;
		if (vs)
		{
			it->second.second = state;
			return make_pair("Finished", 0);
		}
		return make_pair(type, state);
	}

	int y = it->second.first;
	int outDir = getDir(x, y);
	pair<QString, int> ret = dfs(ss, y, antiDirection(outDir));
	it->second.second = ret.second;
	if (inDir != -1)
		return make_pair(ret.first, antiState(inDir, outDir, ret.second));
	else
	{
		for (int i = 0; i < NofDirection; ++i)
		{
			Point_t Q = Id2Point[x].neighbour(i);
			if (Point2Id.find(Q) == Point2Id.end()) continue;
			int q = Point2Id[Q];  // potential real source
			map<int, QString>::iterator it = PluginPoint.find(q);
			if (it != PluginPoint.end() && it->second == ret.first)
			{
				route.insert(make_pair(q, make_pair(x, antiState(i, outDir, ret.second))));  // real source
				return make_pair("Finished", 0);
			}
		}
	}
	return make_pair("Failed", -1);
}

void Arrange_t::flow(SwitchesShow *ss)
{
	qDebug() << "Entered Flow";

	int flow = 0, cost = 0;

	dis.resize(N);
	que.resize(N);
	inq.resize(N);
	from.resize(N);
	link.resize(N);
	while (1)
	{
		for (int i = 0; i < N; ++i)
			dis[i] = INF, inq[i] = false;
		dis[S] = 0;

		int take = -1, put = 0;
		que[put] = S;
		inq[S] = true;
		while (take != put)
		{
			if (++take == NofNode) take = 0;
			int x = que[take];
			for (int i = head[x]; i != -1; i = e[i].next)
			{
				if (e[i].c > 0 && dis[x] + e[i].v < dis[e[i].r])
				{
					dis[e[i].r] = dis[x] + e[i].v;
					from[e[i].r] = x;
					link[e[i].r] = i;
					if (!inq[e[i].r])
					{
						if (++put == NofNode) put = 0;
						que[put] = e[i].r;
					}
				}
			}
		}
		if (dis[T] == INF) break;

		flow += 1;
		cost += dis[T];
		int x = T;
		while (x != S)
		{
			int i = link[x];
			--e[i].c; ++e[i^1].c;
			x = from[x];
		}
	}

	if (flow != NofCoil)
	{
		QMessageBox::about(NULL, "ERROR", "flow != NofCoil");
		return;
	}

	route.clear();
	for (int x = 0; x < NofPoint; ++x)
	{
		int i = x+NofNode;
		for (int j = head[i]; j != -1; j = e[j].next)
			if (e[j].r != x && e[j].c == 0)
			{
				int y = e[j].r;
				if (y < NofPoint)
					route.insert(make_pair(x, make_pair(y, 7)));
				else
				{
					assert(y < NofNode);
					for (int k = 0; k < NofDirection; ++k)
					{
						Point_t Q = Id2Point[x].neighbour(k);
						map<Point_t, int>::iterator it = Point2Id.find(Q);
						if (it == Point2Id.end()) continue;
						if (Point2Node[Point2Id[Q]] == y)
						{
							route.insert(make_pair(x, make_pair(it->second, 7)));
							break;
						}
					}
				}
			}
	}

	for (int i = head[S]; i != -1; i = e[i].next)
		if (e[i].c == 0)
		{
			pair<QString, int> ret = dfs(ss, e[i].r, -1);
			if (ret.second != 0)
				QMessageBox::about(NULL, "ERROR", "Failed to Route");
		}

	for (map<int,pair<int,int>>::iterator it = route.begin(); it != route.end(); ++it)
	{
		Switch3Line tmp;
		tmp.a = Id2Point[it->first].to_IntPoint();
		tmp.b = Id2Point[it->second.first].to_IntPoint();
		SwitchesShow::autoRotateSwitch3Lines(tmp);
		set<Switch3Line>::iterator fnd = ss->lines.find(tmp);
		if (fnd != ss->lines.end()) {
			int stat = it->second.second;
			qDebug() << it->first << ' ' << it->second.first << ' ' << it->second.second;
			for (int i = 0; i < NofSwitchPerLine; ++i)
			fnd->lineStates[i] = (stat & (1 << i)) >> i;
		}
	}

	qDebug() << "Finished Flow";
}
void Arrange_t::algorithm2_flow()
{
	int flow = 0, cost = 0;

	dis.resize(N);
	que.resize(N);
	inq.resize(N);
	from.resize(N);
	link.resize(N);
	while (1)
	{
		for (int i = 0; i < N; ++i)
			dis[i] = INF, inq[i] = false;
		dis[S] = 0;

		int take = -1, put = 0;
		que[put] = S;
		inq[S] = true;
		while (take != put)
		{
			if (++take == NofNode) take = 0;
			int x = que[take];
			for (int i = head[x]; i != -1; i = e[i].next)
			{
				if (e[i].c > 0 && dis[x] + e[i].v < dis[e[i].r])
				{
					dis[e[i].r] = dis[x] + e[i].v;
					from[e[i].r] = x;
					link[e[i].r] = i;
					if (!inq[e[i].r])
					{
						if (++put == NofNode) put = 0;
						que[put] = e[i].r;
					}
				}
			}
		}
		if (dis[T] == INF) break;

		flow += 1;
		cost += dis[T];
		int x = T;
		while (x != S)
		{
			int i = link[x];
			--e[i].c; ++e[i ^ 1].c;
			x = from[x];
		}
	}
	qDebug() << "flow&cost" << flow << " " << cost;
	for (int x = 0; x < NofPoint; ++x)
	{
		int i = x + NofNode;
		for (int j = head[i]; j != -1; j = e[j].next)
			if (e[j].r != x && e[j].c == 0)
			{
				int y = e[j].r;
				if (y < NofPoint)
					route.insert(make_pair(x, make_pair(y, 7)));
				else
				{
					assert(y < NofNode);
					for (int k = 0; k < NofDirection; ++k)
					{
						Point_t Q = Id2Point[x].neighbour(k);
						map<Point_t, int>::iterator it = Point2Id.find(Q);
						if (it == Point2Id.end()) continue;
						if (Point2Node[Point2Id[Q]] == y)
						{
							route.insert(make_pair(x, make_pair(it->second, 7)));
							break;
						}
					}
				}
			}
	}
	for (int i = 1; i < line; i += 2)
		e[i].c = 0;
	for (int i = head[S]; i != -1; i = e[i].next)
		if(e[i].c == 1)
			e[i].c = -1;
}
void Arrange_t::algorithm2_build()
{
	Point2Node.clear();
	NofNode = NofPoint;
	for (int i = 0; i < NofPoint; ++i)
		Point2Node.push_back(i);

	NofCoil = coilList.size();
	for (int i = 0; i < NofCoil; ++i)
	{
		vector<int> coil = getCoil(coilList[i].o, coilList[i].pattern);
		int n = coil.size();
		for (int j = 0; j < n; ++j)
			Point2Node[coil[j]] = NofNode;
		Point2Node.push_back(NofNode++);
	}

	S = 2 * NofNode;
	T = 2 * NofNode + 1;
	N = 2 * NofNode + 2;
	line = 0;
	head.clear();
	for (int i = 0; i < N; ++i)
		head.push_back(-1);
	e.clear();

	for (int i = 0; i < NofNode; ++i)
		if (Point2Node[i] == i)  // not terminal
		{
			addEdge(i, i + NofNode, 1, 0);
			addEdge(i + NofNode, i, 0, 0);
		}
	for (int i = 0; i < NofEdge; ++i)
	{
		int p = EdgePair[i].first;
		int q = EdgePair[i].second;
		if (Point2Node[p] != p && Point2Node[q] != q) continue;
		if (Point2Node[p] != p)
		{
			addEdge(q + NofNode, Point2Node[p], 1, 1);
			addEdge(Point2Node[p], q + NofNode, 0, -1);
		}
		else if (Point2Node[q] != q)
		{
			addEdge(p + NofNode, Point2Node[q], 1, 1);
			addEdge(Point2Node[q], p + NofNode, 0, -1);
		}
		else
		{
			addEdge(p + NofNode, q, 1, 1);
			addEdge(q, p + NofNode, 0, -1);
			addEdge(q + NofNode, p, 1, 1);
			addEdge(p, q + NofNode, 0, -1);
		}
	}
}
void Arrange_t::algorithm2_plot(SwitchesShow* ss)
{
	for (int i = head[S]; i != -1; i = e[i].next)
		if (e[i].c == 0)
		{
			pair<QString, int> ret = algorithm2_dfs(ss, e[i].r, -1);
			if (ret.second != 0)
				QMessageBox::about(NULL, "ERROR", "Failed to Route");
		}

	for (map<int, pair<int, int>>::iterator it = route.begin(); it != route.end(); ++it)
	{
		Switch3Line tmp;
		tmp.a = Id2Point[it->first].to_IntPoint();
		tmp.b = Id2Point[it->second.first].to_IntPoint();
		SwitchesShow::autoRotateSwitch3Lines(tmp);
		set<Switch3Line>::iterator fnd = ss->lines.find(tmp);
		if (fnd != ss->lines.end()) {
			int stat = it->second.second;
			qDebug() << it->first << ' ' << it->second.first << ' ' << it->second.second;
			for (int i = 0; i < NofSwitchPerLine; ++i)
				fnd->lineStates[i] = (stat & (1 << i)) >> i;
		}
	}
}
pair<QString, int> Arrange_t::algorithm2_dfs(SwitchesShow *ss, int x, int inDir)
{
	map<int, pair<int, int>>::iterator it = route.find(x);
	if (it == route.end())
	{
		int CoilId = Point2Node[x] - NofPoint;
		qDebug() << CoilId;
		assert(CoilId >= 0);
		QString type = (CoilId < NofCharging) ? "QiDemo" : "PN532";
		int turns = getTurns(coilList[CoilId].pattern);
		vector<int> coil = getCoil(coilList[CoilId].o, coilList[CoilId].pattern);
		int n = coil.size();
		for (int i = 1; i < turns; ++i)
			coil.insert(coil.end(), coil.begin(), coil.begin() + n);
		int start = -1;
		for (int i = 0; i < n; ++i)
		{
			route.insert(make_pair(coil[i], make_pair(coil[i + 1], 0)));
			if (start == -1) coil.push_back(coil[i]);
			if (coil[i] == x) start = i;
		}
		coil.erase(coil.begin(), coil.begin() + start);
		n = coil.size();

		for (start = 0; start < NofSwitchPerLine; ++start)
			if (circle(coil, start, false) != start) break;
		int end = circle(coil, start, true);

		bool vs = false;
		if (inDir == -1)
		{
			vs = true;  // x is virtual source
			for (int i = 0; i < NofDirection; ++i)
			{
				Point_t Q = Id2Point[x].neighbour(i);
				if (Point2Id.find(Q) == Point2Id.end()) continue;
				int q = Point2Id[Q];  // potential real source
				map<int, QString>::iterator fnd = PluginPoint.find(q);
				if (fnd != PluginPoint.end() && fnd->second == type)
				{
					auto ret = route.insert(make_pair(q, make_pair(x, 0)));  // real source
					it = ret.first; inDir = i;
					break;
				}
			}
		}
		int outDir = getDir(coil[0], coil[1]);
		int inDir = getDir(coil[n - 1], coil[n - 2]);
		int state = 1 << Hub2Switch[end][inDir];
		state |= 1 << Hub2Switch[start][outDir];
		qDebug() << start << ' ' << end << ' ' << inDir << ' ' << outDir << ' ' << state;
		if (vs)
		{
			it->second.second = state;
			return make_pair("Finished", 0);
		}
		return make_pair(type, state);
	}

	int y = it->second.first;
	int outDir = getDir(x, y);
	pair<QString, int> ret = algorithm2_dfs(ss, y, antiDirection(outDir));
	it->second.second = ret.second;
	if (inDir != -1)
		return make_pair(ret.first, antiState(inDir, outDir, ret.second));
	else
	{
		return make_pair("Finished", 0);
	}
	return make_pair("Failed", -1);
}
void Arrange_t::algorithm2(SwitchesShow* ss)
{
	algorithm2_build();
	for (map<int, QString>::iterator it = PluginPoint.begin(); it != PluginPoint.end(); it++)
	{
		if (it->second == "QiDemo")
		{
			qDebug() << "it->first" << it->first;
			addEdge(S, it->first, 1, 0);
			addEdge(it->first, S, 0, 0);
		}
	}
	
	for (int i = NofPoint; i < NofPoint + NofCharging; ++i)
	{
		addEdge(i + NofNode, T, 1, 0);
		addEdge(T, i + NofNode, 0, 0);
	}
	algorithm2_flow();
	for (map<int, QString>::iterator it = PluginPoint.begin(); it != PluginPoint.end(); it++)
	{
		if (it->second == "PN532")
		{
			addEdge(S, it->first, 1, 0);
			addEdge(it->first, S, 0, 0);
		}
	}
	for (int i = NofPoint + NofCharging; i < NofNode; ++i)
	{
		addEdge(i + NofNode, T, 1, 0);
		addEdge(T, i + NofNode, 0, 0);
	}
	algorithm2_flow();
	algorithm2_plot(ss);
}

Arrange_t::Arrange_t()
{
	commuList.push_back(Coil_t{ Point_t(2, 0), 3, 1 });  // for debug
	chargeList.push_back(Coil_t{ Point_t(5, 5), 12, 1 });  // for debug
}

void Arrange_t::lockDataMutex()
{
	mutex.lock();
}

void Arrange_t::unlockDataMutex()
{
	mutex.unlock();
}

bool Point_t::operator < (const struct Point_t P) const
{
	return x < P.x || x == P.x && y < P.y;
}

bool Point_t::operator == (const struct Point_t P) const
{
	return (x == P.x) && (y == P.y);
}

Point_t Point_t::neighbour(int direction)
{
	int _x = x, _y = y;
	if (direction == 0) ++_x;
	else if (direction == 1) ++_y;
	else if (direction == 2) { ++_y; --_x; }
	else if (direction == 3) --_x;
	else if (direction == 4) --_y;
	else if (direction == 5) { --_y; ++_x; }
	return Point_t(_x, _y);
}

IntPoint Point_t::to_IntPoint()
{
	IntPoint ret;
	ret.alpha = x;
	ret.beta = y;
	return ret;
}

bool Coil_t::operator < (const struct Coil_t C) const
{
	return o < C.o || (o == C.o && pattern < C.pattern);
}