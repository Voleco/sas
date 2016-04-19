
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

void SlidingTilePuzzle::GetRankFromState(const SlidingTilePuzzleState& state, uint64_t& rank)
{
	//make a copy of the state
	SlidingTilePuzzleState Pi(state);
	int size = width*height;
	//caculate Pi^-1
	std::vector<int> dual;
	dual.resize(size);
	for (int i = 0; i < size; i++)
	{
		dual[Pi.puzzle[i]] = i;
	}
	rank = 0;
	int s = 0;
	int tmp = 0;
	for (int n = size; n > 0; n--)
	{
		s = Pi.puzzle[n - 1];
		//swap Pi[n-1], Pi[Pi^-1[n-1]]
		tmp = Pi.puzzle[n - 1];
		Pi.puzzle[n - 1] = Pi.puzzle[dual[n - 1]];
		Pi.puzzle[dual[n - 1]] = tmp;
		//swap Pi^-1[s], Pi^-1[n-1]
		tmp = dual[s];
		dual[s] = dual[n - 1];
		dual[n - 1] = tmp;

		rank += s*Factorial(n - 1);
	}

}

void SlidingTilePuzzle::GetStateFromRank(SlidingTilePuzzleState& state, const uint64_t& rank)
{
	state = SlidingTilePuzzleState(width, height);
	state.Reset();
	int s = 0;
	uint64_t r = rank;
	for (int n = width*height ; n > 0; n--)
	{
		s = r / Factorial(n-1);
		r = r % Factorial(n-1);
		//swap state.puzzle[index] with state.puzzle[n-1]
		int tmp = state.puzzle[n-1];
		state.puzzle[n - 1] = state.puzzle[s];
		state.puzzle[s] = tmp;
	}
	for (int i = 0; i < width*height;i++)
		if (state.puzzle[i] == 0)
		{
			state.blankIdx = i;
			break;
		}
}