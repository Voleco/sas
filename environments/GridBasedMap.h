
#ifndef GRID_BASED_MAP_H
#define GRID_BASED_MAP_H

#include <iostream>
#include <unordered_set>
#include "MyEnvironment.h"

class GridBasedMapState
{
public:
	GridBasedMapState() {}
	GridBasedMapState(unsigned int x, unsigned int y):coorX(x),coorY(y) {}
	unsigned int coorX;
	unsigned int coorY;
};

static bool operator==(const GridBasedMapState &s1, const GridBasedMapState &s2)
{
	if (s1.coorX == s2.coorX && s1.coorY == s2.coorY)
		return true;
	return false;
}

static bool operator<(const GridBasedMapState &s1, const GridBasedMapState &s2)
{
	if (s1.coorX < s2.coorX)
		return true;
	else if (s1.coorX == s2.coorY)
		return s1.coorY < s2.coorY;
	else
		return false;
}

namespace std {
	template<>
	struct hash<GridBasedMapState>
	{
		std::size_t operator()(const GridBasedMapState& s) const
		{
			using std::hash;
			
			return ((hash<unsigned int>()(s.coorX)) ^ (hash<unsigned int>()(s.coorX)));
		}
	};
}

static std::ostream& operator <<(std::ostream & out, const GridBasedMapState &s)
{
	out << "x: "<<s.coorX<<", y: "<<s.coorY;
	return out;
}


//GridBasedMapAction contains 4 types of actions,
//0 for right, 1 for up, 2 for left, 3 for down
//for now we will only use 0 and 1
typedef unsigned int GridBasedMapAction;

class GridBasedMap :public MyEnvironment<GridBasedMapState, GridBasedMapAction>
{
public:
	GridBasedMap() {}
	GridBasedMap(unsigned int x, unsigned int y) :maxX(x),maxY(y){}
	void GetActions(GridBasedMapState &nodeID, std::vector<GridBasedMapAction> &actions);
	void ApplyAction(GridBasedMapState &s, GridBasedMapAction a);
	void UndoAction(GridBasedMapState &s, GridBasedMapAction a);
	GridBasedMapAction GetInvertAction(const GridBasedMapAction& a) { return -1; };
	unsigned int GetDepth(GridBasedMapState &s);
	unsigned int GetSolutionDepthUpperBound(GridBasedMapState &s, GridBasedMapState &g);

private:
	unsigned int maxX;
	unsigned int maxY;
};

class GridBasedMapHeuristic
{
public:
	GridBasedMapHeuristic() { }
	~GridBasedMapHeuristic() {}
	void SetGoal(GridBasedMapState& s) { goal = s; }
	int GetHCost(GridBasedMapState& s);
	GridBasedMapState goal;
};


#endif // !GRID_BASED_MAP_H
