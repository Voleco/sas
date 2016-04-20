
#ifndef SLIDING_TILE_PUZZLE_H
#define SLIDING_TILE_PUZZLE_H

#include <iostream>
#include <string>
#include <stack>
#include "MyEnvironment.h"

#define Right 0
#define Up    1
#define Left  2
#define Down  3

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
	}
	void Reset() 
	{ 
		puzzle.resize(0);
		for (int i = 0; i < width*height; i++)
			puzzle.push_back(i);
		blankIdx = 0;
	}
	unsigned int width;
	unsigned int height;
	std::vector<int> puzzle;
	unsigned int blankIdx;
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

class ManhattanDistanceHeuristic
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
	SlidingTilePuzzle(unsigned int w, unsigned int h):width(w),height(h) { historyActions.push(-1); }
	~SlidingTilePuzzle() {}

	void GetActions(SlidingTilePuzzleState &nodeID, std::vector<SlidingTilePuzzleAction> &actions);
	void ApplyAction(SlidingTilePuzzleState &s, SlidingTilePuzzleAction a);
	void UndoAction(SlidingTilePuzzleState &s, SlidingTilePuzzleAction a);

	uint64_t Factorial(int val)
	{
		static uint64_t table[21] =
		{ 1ll, 1ll, 2ll, 6ll, 24ll, 120ll, 720ll, 5040ll, 40320ll, 362880ll, 3628800ll, 39916800ll, 479001600ll,
			6227020800ll, 87178291200ll, 1307674368000ll, 20922789888000ll, 355687428096000ll,
			6402373705728000ll, 121645100408832000ll, 2432902008176640000ll };
		if (val > 20)
			return (uint64_t)-1;
		return table[val];
	}


	void GetRankFromState(const SlidingTilePuzzleState& state, uint64_t& rank);
	void GetStateFromRank(SlidingTilePuzzleState& state, const uint64_t& rank);
	std::stack<SlidingTilePuzzleAction> historyActions;
	unsigned int width;
	unsigned int height;

};

class SlidingTilePuzzlePDB
{
public:
	SlidingTilePuzzlePDB(SlidingTilePuzzle* e, SlidingTilePuzzleState &s, std::vector<int> p);
	uint64_t GetPDBSize() const { return pdbSize; }

	void GetAbstractState(const SlidingTilePuzzleState& state, SlidingTilePuzzleState& abstate);
	void GetPDBRankFromState(const SlidingTilePuzzleState& state, uint64_t& rank);
	void GetStateFromPDBRank(SlidingTilePuzzleState& state, const uint64_t& rank);

	//void BuildPDB();

	std::string GetFileName(const char *prefix);
	void Save(const char* prefix);
	bool Load(const char* prefix);

	uint64_t FactorialN_K(int n, int k);


	SlidingTilePuzzle* env;
	SlidingTilePuzzleState goalState;
	std::vector<int> pattern;
	uint64_t pdbSize;
};


#endif