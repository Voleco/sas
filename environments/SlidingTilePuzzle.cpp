
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

//Lexicographical ranking
void SlidingTilePuzzle::GetRankFromState(const SlidingTilePuzzleState& state, uint64_t& rank)
{
	int size = state.width*state.height;
	rank = 0;
	int s = 0;
	for (int i = 0; i < size; i++)
	{
		s = 0;
		for (int j = i + 1; j < size; j++)
			if (state.puzzle[j] < state.puzzle[i])
				s++;

		rank += s*Factorial(size - 1 - i);
	}

}
void SlidingTilePuzzle::GetStateFromRank(SlidingTilePuzzleState& state, const uint64_t& rank)
{
	state = SlidingTilePuzzleState(width, height);
	state.Reset();
	int size = width*height;
	std::vector<int> pz;
	for (int i = 0; i < size; i++)
		pz.push_back(i);
	uint64_t countRight = 0;
	uint64_t r = rank;
	for (int i = 0; i < size; i++)
	{
		countRight = r / Factorial(size - 1 - i);
		r = r%Factorial(size - 1 - i);
		state.puzzle[i] = pz[countRight];
		pz.erase(pz.begin() + countRight);
	}

	for (int i = 0; i < size; i++)
	{
		if (state.puzzle[i] == 0)
		{
			state.blankIdx = i;
			break;
		}
	}
}

// Myrvold/Ruskey linear-time method
/*
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
*/

SlidingTilePuzzlePDB::SlidingTilePuzzlePDB(SlidingTilePuzzle* e, SlidingTilePuzzleState &s, std::vector<int> p)
	:env(e),pattern(p),pdbSize(0),goalState(s)
{
	pdbSize = FactorialN_K(s.puzzle.size(), s.puzzle.size() - pattern.size());
	uint64_t r;
	GetPDBRankFromState(s, r);
	GetStateFromPDBRank(goalState, r);
}

uint64_t SlidingTilePuzzlePDB::FactorialN_K(int n, int k)
{
	uint64_t value = 1;

	for (int i = n; i > k; i--)
	{
		value *= i;
	}

	return value;
}

void SlidingTilePuzzlePDB::GetPDBRankFromState(const SlidingTilePuzzleState& state, uint64_t& rank)
{
	int size = pattern.size();
	int K = state.puzzle.size() - size;
	std::vector<int> copy;
	for (int i = 0; i < state.puzzle.size(); i++)
		copy.push_back(-1);
	for (int i = 0; i < pattern.size(); i++)
		for (int i2 = 0; i2 < state.puzzle.size(); i2++)
			if (state.puzzle[i2] == pattern[i])
				copy[i2] = state.puzzle[i2];

	rank = 0;
	int j = 0;
	for (int i = 0; i < size; i++)
	{
		j = 0;
		for (j = 0; j < copy.size(); j++)
		{
			if (copy[j] == pattern[i])
				break;
		}

		rank += j * FactorialN_K(copy.size()-1,K);

		copy.erase(copy.begin() + j);
	}
}

void SlidingTilePuzzlePDB::GetStateFromPDBRank(SlidingTilePuzzleState& state, const uint64_t& rank)
{
	int K = state.puzzle.size() - pattern.size();
	uint64_t r = rank;
	int count = 0;
	int index = 0;
	for (int i = 0; i < state.puzzle.size(); i++)
		state.puzzle[i] = -1;
	for (int i = 0; i < pattern.size(); i++)
	{
		count = r / FactorialN_K(state.puzzle.size() - 1 - i, K);
		r = r % FactorialN_K(state.puzzle.size() - 1 - i, K);
		index = 0;
		while (count > 0) 
		{
			if (state.puzzle[index] == -1)
				count--;
			index++;
		}
		while (state.puzzle[index] != -1)
			index++;
		state.puzzle[index] = pattern[i];
	}
	for (int i = 0; i < state.puzzle.size(); i++)
		if (state.puzzle[i] == 0)
		{
			state.blankIdx = i;
			break;
		}

}


std::string SlidingTilePuzzlePDB::GetFileName(const char *prefix)
{
	std::string fileName;
	fileName += prefix;

	fileName += std::to_string(env->width)+"x"+ std::to_string(env->height)+"puzzle";
	fileName += "-";
	for (int x = 0; x < goalState.puzzle.size(); x++)
	{
		fileName += std::to_string(goalState.puzzle[x]);
		fileName += ";";
	}
	fileName.pop_back(); // remove colon
	fileName += "-";
	for (int x = 0; x < pattern.size(); x++)
	{
		fileName += std::to_string(pattern[x]);
		fileName += ";";
	}
	fileName.pop_back(); // remove colon
	fileName += ".pdb";

	return fileName;
}

void SlidingTilePuzzlePDB::Save(const char* prefix)
{
	std::string fileName = GetFileName(prefix);
	FILE *f = fopen(fileName.c_str(), "w+b");
	//TODO add real reaing code here
	//fwrite(&type, sizeof(type), 1, f);
	//fwrite(&goalState, sizeof(goalState), 1, f);
	//PDB.Write(f);
	fclose(f);
	std::cout << "Saved PDB: " << fileName << "\n";
}

bool SlidingTilePuzzlePDB::Load(const char* prefix)
{
	std::string fileName = GetFileName(prefix);
	FILE *f = fopen(fileName.c_str(), "rb");
	if (f == 0)
	{
		std::cout << "Could not load PDB: " << fileName << "\n";
		return false;
	}
	//if (fread(&type, sizeof(type), 1, f) != 1)
	//	return false;
	//if (fread(&goalState, sizeof(goalState), 1, f) != 1)
	//	return false;
	//TODO add real reaing code here
	bool result = true;
	fclose(f);
	if (result)
		std::cout << "Successfully loaded PDB: " << fileName << "\n";
	else
		std::cout << "Could not load PDB: " << fileName << "\n";
	return result;
}