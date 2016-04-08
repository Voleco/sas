
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
	
	//std::cout << "\n************state geting actions:\n" << nodeID;
	//std::cout << "node blankidx: " << nodeID.blankIdx << "node width" << nodeID.width << "\n";
	if ((nodeID.blankIdx % nodeID.width < nodeID.width - 1) 
		&& (historyActions.top()!=Left))
		actions.push_back(Right);
	if ((nodeID.blankIdx % nodeID.width > 0)
		&& (historyActions.top() != Right))
		actions.push_back(Left);
	if ((nodeID.blankIdx >= nodeID.width)
		&&(historyActions.top() != Down))
		actions.push_back(Up);
	if ((nodeID.blankIdx < nodeID.width*(nodeID.height-1))
		&&(historyActions.top() != Up))
		actions.push_back(Down);

	
	//std::cout << "hisA top: " << historyActions.top() <<"\nactions: ";
	//for (int i = 0; i < actions.size(); i++)
	//	std::cout << actions[i] << " ";
	//std::cout << "\n";
}

void SlidingTilePuzzle::ApplyAction(SlidingTilePuzzleState& s, SlidingTilePuzzleAction a)
{
	//std::cout << "state applying actions:\n" << s;
	//std::cout << "action: \n" << a<<"\n";
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
	historyActions.push(a);
	//std::cout << "state applied actions:\n" << s;
}

void SlidingTilePuzzle::UndoAction(SlidingTilePuzzleState& s, SlidingTilePuzzleAction a)
{
	//td::cout << "state undoing actions:\n" << s;
	//std::cout << "action: \n" << a << "\n";
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
	historyActions.pop();
	//std::cout << "state undid actions:\n" << s;
}