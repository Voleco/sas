
#ifndef MAP2D_H
#define MAP2D_H

#include <string>
#include <iostream>
#include <fstream>
#include <string.h>
#include <math.h>
#include "MyEnvironment.h"
#include "MyHeuristic.h"
#include "MyAStar.h"

#define SQUARE_ROOT_OF2 1.414213562373
#define AMP_FACTOR 4

class Map2DState
{
public:
	Map2DState():x(-1),y(-1) {}
	Map2DState(int xval, int yval) :x(xval), y(yval) {}
	int x;
	int y;
};

struct Map2DStateHash
{
	std::size_t operator()(const Map2DState & s) const
	{
		return (s.y << 16) | (s.x);
	}
};

static std::ostream& operator <<(std::ostream & out, const Map2DState &s)
{
	out << "(" << s.x << ", " << s.y << ")";
	return out;
}

static bool operator==(const Map2DState &s1, const Map2DState &s2) {
	return (s1.x == s2.x) && (s1.y == s2.y);
}

enum Map2DAction {
	tR = 0x1,
	tU = 0x2,
	tL = 0x4,
	tD = 0x8,
	tUR = 0x3,
	tUL = 0x6,
	tDR = 0x9,
	tDL = 0xC,
	tUNKNOWN = 0
};

static std::ostream& operator <<(std::ostream & out, const Map2DAction &a)
{
	switch (a)
	{
	case tR:
		out << "Right";
		break;
	case tU:
		out << "Up";
		break;
	case tL:
		out << "Left";
		break;
	case tD:
		out << "Down";
		break;
	case tUR:
		out << "UpperRight";
		break;
	case tUL:
		out << "UpperLeft";
		break;
	case tDR:
		out << "DownRight";
		break;
	case tDL:
		out << "DownLeft";
		break;
	case tUNKNOWN:
	default:
		break;
	}
	return out;
}

enum Map2DType {
	FOUR_WAY = 0x1,
	OCTILE = 0x2
};

class Map2D : public MyEnvironment<Map2DState, Map2DAction>
{
public:
	Map2D():map(NULL) {  }
	//copy constructor
	Map2D(const Map2D& s);
	~Map2D() 
	{ 
		if(map)
			delete map; 
	}

	void GetActions(Map2DState &nodeID, std::vector<Map2DAction> &actions);
	void ApplyAction(Map2DState &s, Map2DAction a);
	void UndoAction(Map2DState &s, Map2DAction a);

	double GetActionCost(const Map2DAction& a);
	Map2DAction GetInvertAction(const Map2DAction& a);

	void GetRankFromState(const Map2DState& state, uint64_t& rank);
	void GetStateFromRank(Map2DState& state, const uint64_t& rank);

	bool LoadMap(std::string fileName);

	int height;
	int width;
	Map2DType mapType;
	char* map;
};

class OctileDistanceHeuristic :public MyHeuristic<Map2DState>
{
public:
	OctileDistanceHeuristic() { }
	~OctileDistanceHeuristic() {}
	void SetGoal(Map2DState& s) { goal = s; }
	double GetHCost(Map2DState& s);
	double GetHCost(Map2DState& s1, Map2DState& s2);
	Map2DState goal;
};


class Map2DDifferentialHeuristic :public MyHeuristic<Map2DState>
{
public:
	Map2DDifferentialHeuristic():img(NULL),width(0),height(0) { }
	Map2DDifferentialHeuristic(Map2D e):env(e),img(NULL),width(0),height(0) { }
	~Map2DDifferentialHeuristic() {}
	void SetGoal(Map2DState& s) { goal = s; }
	double GetHCost(Map2DState& s);
	double GetHCost(Map2DState& s1, Map2DState& s2);

	void AddPivot(Map2DState& s);
	void ChangePivot(Map2DState& s, int index);

	Map2DState GetFarthestPivot();
	
	void BuildPDBs();
	void BuildPDB(int i);

	bool LoadMap(std::string fileName);
	void SaveAsBMP(std::string fileName,int index);

	int height;
	int width;

protected:
	Map2D env;
	Map2DState goal;

	unsigned char *img;

	std::vector<Map2DState> pivots;
	std::vector<bool> built;
	std::vector<double*> heurTable;
	std::vector<unsigned char *> imgs;
};

#endif // !MAP2D_H
