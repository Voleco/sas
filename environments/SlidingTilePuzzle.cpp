
#include "SlidingTilePuzzle.h"

int  ManhattanDistanceHeuristic::GetHCost(SlidingTilePuzzleState& s)
{
	int hcost = 0;
	int xdiff = 0;
	int ydiff = 0;
	int size = s.puzzle.size();
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (goal.puzzle[j] == s.puzzle[i]&& goal.puzzle[j]!=0)
			{
				ydiff = j / s.width > i / s.width ? (j / s.width - i / s.width) : (i / s.width - j / s.width);
				xdiff = j%s.width > i%s.width ? (j%s.width - i%s.width) : (i%s.width - j%s.width);
				hcost += xdiff + ydiff;
				break;
			}
		}

	}
	return hcost;
}

void SlidingTilePuzzle::GetActions(SlidingTilePuzzleState& nodeID, std::vector<SlidingTilePuzzleAction> &actions)
{
	//TODO not to generate the coming action
	actions.resize(0);
	
	if (nodeID.blankIdx % nodeID.width < nodeID.width - 1)
		actions.push_back(Right);
	if (nodeID.blankIdx % nodeID.width > 0)
		actions.push_back(Left);
	if (nodeID.blankIdx >= nodeID.width)
		actions.push_back(Up);
	if (nodeID.blankIdx < nodeID.width*(nodeID.height))
		actions.push_back(Down);
}

void SlidingTilePuzzle::ApplyAction(SlidingTilePuzzleState& s, SlidingTilePuzzleAction a)
{
	unsigned int inx = 0;
	switch (a)
	{
	case Right:
		inx = s.blankIdx + 1;
		break;
	case Up:
		inx = s.blankIdx - s.width;
		break;
	case Left:
		inx = s.blankIdx - 1;
		break;
	case Down:
		inx = s.blankIdx + s.width;
		break;
	default:
		break;
	}
	s.puzzle[s.blankIdx] = s.puzzle[inx];
	s.puzzle[inx] = 0;
	s.blankIdx = inx;
}

void SlidingTilePuzzle::UndoAction(SlidingTilePuzzleState& s, SlidingTilePuzzleAction a)
{
	unsigned int inx = 0;
	switch (a)
	{
	case Right:
		inx = s.blankIdx - 1;
		break;
	case Up:
		inx = s.blankIdx + s.width;
		break;
	case Left:
		inx = s.blankIdx + 1;
		break;
	case Down:
		inx = s.blankIdx - s.width;
		break;
	default:
		break;
	}
	s.puzzle[s.blankIdx] = s.puzzle[inx];
	s.puzzle[inx] = 0;
	s.blankIdx = inx;
}