
#ifndef SLIDING_TILE_PUZZLE_H
#define SLIDING_TILE_PUZZLE_H

#include <iostream>
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
	SlidingTilePuzzle() {}
	SlidingTilePuzzle(unsigned int w, unsigned int h) {}
	~SlidingTilePuzzle() {}

	void GetActions(SlidingTilePuzzleState &nodeID, std::vector<SlidingTilePuzzleAction> &actions);
	void ApplyAction(SlidingTilePuzzleState &s, SlidingTilePuzzleAction a);
	void UndoAction(SlidingTilePuzzleState &s, SlidingTilePuzzleAction a);


};


#endif