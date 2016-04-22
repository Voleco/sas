
#ifndef SLIDING_TILE_PUZZLE_H
#define SLIDING_TILE_PUZZLE_H

#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include "MyEnvironment.h"
#include "MyHeuristic.h"

#define Right 0
#define Up    1
#define Left  2
#define Down  3

#define Myrvold_Ruskey_ranking
//#define Lexicographical_ranking

#define ASGS

uint64_t Factorial(int val);
uint64_t FactorialN_K(int n, int k);


#ifdef ASGS
static int distances[16][16] =
{
	{ 0, 1, 2, 3, 1, 2, 3, 4, 2, 3, 4, 5, 3, 4, 5, 6 }, // 0
	{ 1, 0, 1, 2, 2, 1, 2, 3, 3, 2, 3, 4, 4, 3, 4, 5 }, // 1
	{ 2, 1, 0, 1, 3, 2, 1, 2, 4, 3, 2, 3, 5, 4, 3, 4 }, // 2
	{ 3, 2, 1, 0, 4, 3, 2, 1, 5, 4, 3, 2, 6, 5, 4, 3 }, // 3
	{ 1, 2, 3, 4, 0, 1, 2, 3, 1, 2, 3, 4, 2, 3, 4, 5 }, // 4
	{ 2, 1, 2, 3, 1, 0, 1, 2, 2, 1, 2, 3, 3, 2, 3, 4 }, // 5
	{ 3, 2, 1, 2, 2, 1, 0, 1, 3, 2, 1, 2, 4, 3, 2, 3 }, // 6
	{ 4, 3, 2, 1, 3, 2, 1, 0, 4, 3, 2, 1, 5, 4, 3, 2 }, // 7
	{ 2, 3, 4, 5, 1, 2, 3, 4, 0, 1, 2, 3, 1, 2, 3, 4 }, // 8
	{ 3, 2, 3, 4, 2, 1, 2, 3, 1, 0, 1, 2, 2, 1, 2, 3 }, // 9
	{ 4, 3, 2, 3, 3, 2, 1, 2, 2, 1, 0, 1, 3, 2, 1, 2 }, // 10
	{ 5, 4, 3, 2, 4, 3, 2, 1, 3, 2, 1, 0, 4, 3, 2, 1 }, // 11
	{ 3, 4, 5, 6, 2, 3, 4, 5, 1, 2, 3, 4, 0, 1, 2, 3 }, // 12
	{ 4, 3, 4, 5, 3, 2, 3, 4, 2, 1, 2, 3, 1, 0, 1, 2 }, // 13
	{ 5, 4, 3, 4, 4, 3, 2, 3, 3, 2, 1, 2, 2, 1, 0, 1 }, // 14
	{ 6, 5, 4, 3, 5, 4, 3, 2, 4, 3, 2, 1, 3, 2, 1, 0 }	// 15
};
#endif

class SlidingTilePuzzleState
{
public:
	SlidingTilePuzzleState() {}
	SlidingTilePuzzleState(unsigned int x, unsigned int y) :width(x), height(y) { Reset(); }
	SlidingTilePuzzleState(const SlidingTilePuzzleState& s)
	{
		width = s.width;
		height = s.height;
		puzzle.resize(s.puzzle.size());
		for (int i = 0; i < puzzle.size(); i++)
			puzzle[i] = s.puzzle[i];
		blankIdx = s.blankIdx;
#ifdef ASGS
		hCost = s.hCost;
#endif
	}
	~SlidingTilePuzzleState() {}
	void Reset() 
	{ 
		puzzle.resize(0);
		for (int i = 0; i < width*height; i++)
			puzzle.push_back(i);
		blankIdx = 0;
#ifdef ASGS
		hCost = 0;
#endif
	}
	unsigned int width;
	unsigned int height;
	std::vector<int> puzzle;
	unsigned int blankIdx;
#ifdef ASGS
	int hCost;
#endif // ASGS
};

static bool operator==(const SlidingTilePuzzleState &s1, const SlidingTilePuzzleState &s2)
{
	if (s1.width != s2.width || s1.height != s2.height)
		return false;
	for (int i = 0; i < s1.width*s1.height; i++)
		if (s1.puzzle[i] != s2.puzzle[i])
			return false;
	return true;
}


static std::ostream& operator <<(std::ostream & out, const SlidingTilePuzzleState &s)
{
	out << "width: " << s.width << ", height: " << s.height<<"\n";
	for (int i = 0; i < s.width*s.height; i++)
		out << s.puzzle[i] << " ";
	out << "\n";
	return out;
}

void GetSildingTileInstance(int index, SlidingTilePuzzleState& s);

typedef char SlidingTilePuzzleAction;

static std::ostream& operator <<(std::ostream & out, const SlidingTilePuzzleAction &a)
{
	switch (a)
	{
	case Right:
		out << "Right";
		break;
	case Up:
		out << "Up";
		break;
	case Left:
		out << "Left";
		break;
	case Down:
		out << "Down";
		break;
	default:
		break;
	}
	return out;
}

class ManhattanDistanceHeuristic:public MyHeuristic<SlidingTilePuzzleState>
{
public:
	ManhattanDistanceHeuristic() { }
	~ManhattanDistanceHeuristic() {}
	void SetGoal(SlidingTilePuzzleState& s) { goal = s; }
	int GetHCost(SlidingTilePuzzleState& s);
	SlidingTilePuzzleState goal;
};


class SlidingTilePuzzle : public MyEnvironment<SlidingTilePuzzleState, SlidingTilePuzzleAction> 
{
public:
	SlidingTilePuzzle() { historyActions.push(-1); }
	SlidingTilePuzzle(unsigned int w, unsigned int h, bool _allow_move_back=false):width(w),height(h),allowMoveBack(_allow_move_back) { historyActions.push(-1); }
	SlidingTilePuzzle(const SlidingTilePuzzle& s)
		:width(s.width),height(s.height),allowMoveBack(s.allowMoveBack),historyActions(s.historyActions)
	{}
	~SlidingTilePuzzle() {}

	void GetActions(SlidingTilePuzzleState &nodeID, std::vector<SlidingTilePuzzleAction> &actions);
	void ApplyAction(SlidingTilePuzzleState &s, SlidingTilePuzzleAction a);
	void UndoAction(SlidingTilePuzzleState &s, SlidingTilePuzzleAction a);



	void GetRankFromState(const SlidingTilePuzzleState& state, uint64_t& rank);
	void GetStateFromRank(SlidingTilePuzzleState& state, const uint64_t& rank);
	std::stack<SlidingTilePuzzleAction> historyActions;
	unsigned int width;
	unsigned int height;
	bool allowMoveBack;
};

class SlidingTilePuzzlePDB :public MyHeuristic<SlidingTilePuzzleState>
{
public:
	SlidingTilePuzzlePDB(SlidingTilePuzzle e, SlidingTilePuzzleState s, std::vector<int> p);
	SlidingTilePuzzlePDB() {}
	SlidingTilePuzzlePDB(const SlidingTilePuzzlePDB& s) 
		:env(s.env),goalState(s.goalState),pattern(s.pattern),pdbSize(s.pdbSize),pdbData(s.pdbData)
	{}
	~SlidingTilePuzzlePDB() {}

	int GetHCost(SlidingTilePuzzleState& s);

	uint64_t GetPDBSize() const { return pdbSize; }

	//void GetAbstractState(const SlidingTilePuzzleState& state, SlidingTilePuzzleState& abstate);
	void GetPDBRankFromState(const SlidingTilePuzzleState& state, uint64_t& rank);
	void GetStateFromPDBRank(SlidingTilePuzzleState& state, const uint64_t& rank);

	void BuildPDB();

	std::string GetFileName(const char *prefix);
	void Save(const char* prefix);
	bool Load(const char* prefix);

	
protected:

	SlidingTilePuzzle env;
	SlidingTilePuzzleState goalState;
	std::vector<int> pattern;
	uint64_t pdbSize;
	std::vector<uint8_t> pdbData;

};


#endif