
#include "SlidingTilePuzzle.h"

int ManhattanDistanceHeuristic::GetHCost(SlidingTilePuzzleState& s)
{
#ifdef ASGS
	return s.hCost;
#endif
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
	actions.resize(0);
	
	//std::cout << "\n************state geting actions:\n" << nodeID;
	//std::cout << "node blankidx: " << nodeID.blankIdx << "node width" << nodeID.width << "\n";
	if (nodeID.blankIdx % nodeID.width < nodeID.width - 1)
		actions.push_back(Right);
	if (nodeID.blankIdx % nodeID.width > 0)
		actions.push_back(Left);
	if (nodeID.blankIdx >= nodeID.width)
		actions.push_back(Up);
	if (nodeID.blankIdx < nodeID.width*(nodeID.height-1))
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

#ifdef ASGS
	if (s.puzzle[inx] != -1)
	{
		s.hCost -= distances[s.puzzle[inx]][inx];
		s.hCost += distances[s.puzzle[inx]][s.blankIdx];
	}
#endif

	s.puzzle[s.blankIdx] = s.puzzle[inx];
	s.puzzle[inx] = 0;
	s.blankIdx = inx;
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

#ifdef ASGS
	if (s.puzzle[inx] != -1)
	{
		s.hCost -= distances[s.puzzle[inx]][inx];
		s.hCost += distances[s.puzzle[inx]][s.blankIdx];
	}
#endif

	s.puzzle[s.blankIdx] = s.puzzle[inx];
	s.puzzle[inx] = 0;
	s.blankIdx = inx;
	//std::cout << "state undid actions:\n" << s;
}

SlidingTilePuzzleAction SlidingTilePuzzle::GetInvertAction(const SlidingTilePuzzleAction& a)
{
	switch (a)
	{
	case Right:
		return Left;
		break;
	case Up:
		return Down;
		break;
	case Left:
		return Right;
		break;
	case Down:
		return Up;
		break;
	default:
		break;
	}
	return -1;
}

#ifdef Lexicographical_ranking
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
#endif

#ifdef Myrvold_Ruskey_ranking
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
	for (int n = size; n > 0; n--)
	{
		s = Pi.puzzle[n - 1];
		//swap Pi[n-1], Pi[Pi^-1[n-1]]
		std::swap(Pi.puzzle[n - 1], Pi.puzzle[dual[n - 1]]);
		//swap Pi^-1[s], Pi^-1[n-1]
		std::swap(dual[n - 1], dual[s]);

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
		std::swap(state.puzzle[n - 1], state.puzzle[s]);
	}
	for (int i = 0; i < width*height;i++)
		if (state.puzzle[i] == 0)
		{
			state.blankIdx = i;
			break;
		}
}
#endif

SlidingTilePuzzlePDB::SlidingTilePuzzlePDB(SlidingTilePuzzle e, SlidingTilePuzzleState s, std::vector<int> p)
	:env(e),pattern(p),locs(p),pdbSize(0),goalState(s)
{
	uint64_t r;
	GetPDBRankFromState(s, r);
	GetStateFromPDBRank(goalState, r);
	pdbSize = FactorialN_K(s.puzzle.size(), s.puzzle.size() - pattern.size());
	pdbData = std::vector<uint8_t>(pdbSize, 255);
}

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

uint64_t FactorialN_K(int n, int k)
{
	return Factorial(n) / Factorial(k);
}

void SlidingTilePuzzlePDB::GetPDBRankFromState(const SlidingTilePuzzleState& state, uint64_t& rank)
{
	int psize = pattern.size();
	int ssize = state.puzzle.size();
	int K = ssize - psize;


	for (int i = 0; i < psize;i++)
		for (int j = 0; j < ssize; j++)
			if (state.puzzle[j] == pattern[i])
				locs[i]=j;
	rank = 0;
	for (int i = 0; i < psize; i++)
	{
		rank += locs[i] * FactorialN_K(ssize-1-i, K);
		for (int j = i+1; j < psize; j++)
			if (locs[j] > locs[i])
				locs[j]--;
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

void SlidingTilePuzzlePDB::BuildPDB()
{
	uint64_t rank;
	GetPDBRankFromState(goalState, rank);
	pdbData[rank] = 0;

	std::queue<uint64_t> openQueue;
	openQueue.push(rank);
	int depth = -1;
	uint64_t numTotal = 0;
	uint64_t numNodesOfCurDepth = 0;
	uint64_t nextRank;
	SlidingTilePuzzleState nextState(goalState.width,goalState.height);
	std::vector<SlidingTilePuzzleAction> actions;
	while (!openQueue.empty())
	{
		nextRank = openQueue.front();
		openQueue.pop();
		//in this case, we have justed finished last depth
		if (pdbData[nextRank] == depth + 1)
		{
			depth++;
			std::cout << "depth: " << depth << " " << 1 + openQueue.size() << " of " << pdbSize << "\n";
			numTotal += 1 + openQueue.size();
		}

		GetStateFromPDBRank(nextState, nextRank);
		env.GetActions(nextState, actions);
		//generate its successors
		for (unsigned int i = 0; i < actions.size(); i++)
		{
			env.ApplyAction(nextState, actions[i]);
			
			GetPDBRankFromState(nextState, nextRank);
			if (pdbData[nextRank] > depth+1)
			{
				pdbData[nextRank] = depth+1;
				openQueue.push(nextRank);
			}		

			env.UndoAction(nextState, actions[i]);
		}
	}
	std::cout<<"total num of nodes: "<<numTotal<<" of " << pdbSize << "\n";
}

std::string SlidingTilePuzzlePDB::GetFileName(const char *prefix)
{
	std::string fileName;
	fileName += prefix;

	if (fileName.back() != '/')
		fileName += "/";
	fileName += std::to_string(env.width)+"x"+ std::to_string(env.height)+"puzzle";
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

	fwrite(pdbData.data(), sizeof(uint8_t), pdbSize, f);
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
	fseek(f, 0, SEEK_END);
	uint64_t fileSize= ftell(f);
	rewind(f);
	if (fileSize != pdbSize)
	{
		std::cout << "Error: unmatched file size: " << fileName << " \n";
		return false;
	}
		
	uint8_t* data = new uint8_t[pdbSize];
	uint64_t bytesRead = fread(data, sizeof(uint8_t), pdbSize, f);
	if ( bytesRead != pdbSize)
	{
		std::cout << "Could not load PDB: " << fileName << "\n";
		return false;
	}
	fclose(f);
	pdbData = std::vector<uint8_t>(data,data+pdbSize);
	std::cout << "Successfully loaded PDB: " << fileName << "\n";

	return true;
}

int SlidingTilePuzzlePDB::GetHCost(SlidingTilePuzzleState& s)
{
	uint64_t index;
	GetPDBRankFromState(s, index);
	return pdbData[index];
}



void GetSildingTileInstance(int index, SlidingTilePuzzleState& s)
{
	int instances[110][16] =
	{
		{ 14, 13, 15, 7, 11, 12, 9, 5, 6, 0, 2, 1, 4, 8, 10, 3 },
		{ 13, 5, 4, 10, 9, 12, 8, 14, 2, 3, 7, 1, 0, 15, 11, 6 },
		{ 14, 7, 8, 2, 13, 11, 10, 4, 9, 12, 5, 0, 3, 6, 1, 15 },
		{ 5, 12, 10, 7, 15, 11, 14, 0, 8, 2, 1, 13, 3, 4, 9, 6 },
		{ 4, 7, 14, 13, 10, 3, 9, 12, 11, 5, 6, 15, 1, 2, 8, 0 },
		{ 14, 7, 1, 9, 12, 3, 6, 15, 8, 11, 2, 5, 10, 0, 4, 13 },
		{ 2, 11, 15, 5, 13, 4, 6, 7, 12, 8, 10, 1, 9, 3, 14, 0 },
		{ 12, 11, 15, 3, 8, 0, 4, 2, 6, 13, 9, 5, 14, 1, 10, 7 },
		{ 3, 14, 9, 11, 5, 4, 8, 2, 13, 12, 6, 7, 10, 1, 15, 0 },
		{ 13, 11, 8, 9, 0, 15, 7, 10, 4, 3, 6, 14, 5, 12, 2, 1 },
		{ 5, 9, 13, 14, 6, 3, 7, 12, 10, 8, 4, 0, 15, 2, 11, 1 },
		{ 14, 1, 9, 6, 4, 8, 12, 5, 7, 2, 3, 0, 10, 11, 13, 15 },
		{ 3, 6, 5, 2, 10, 0, 15, 14, 1, 4, 13, 12, 9, 8, 11, 7 },
		{ 7, 6, 8, 1, 11, 5, 14, 10, 3, 4, 9, 13, 15, 2, 0, 12 },
		{ 13, 11, 4, 12, 1, 8, 9, 15, 6, 5, 14, 2, 7, 3, 10, 0 },
		{ 1, 3, 2, 5, 10, 9, 15, 6, 8, 14, 13, 11, 12, 4, 7, 0 },
		{ 15, 14, 0, 4, 11, 1, 6, 13, 7, 5, 8, 9, 3, 2, 10, 12 },
		{ 6, 0, 14, 12, 1, 15, 9, 10, 11, 4, 7, 2, 8, 3, 5, 13 },
		{ 7, 11, 8, 3, 14, 0, 6, 15, 1, 4, 13, 9, 5, 12, 2, 10 },
		{ 6, 12, 11, 3, 13, 7, 9, 15, 2, 14, 8, 10, 4, 1, 5, 0 },
		{ 12, 8, 14, 6, 11, 4, 7, 0, 5, 1, 10, 15, 3, 13, 9, 2 },
		{ 14, 3, 9, 1, 15, 8, 4, 5, 11, 7, 10, 13, 0, 2, 12, 6 },
		{ 10, 9, 3, 11, 0, 13, 2, 14, 5, 6, 4, 7, 8, 15, 1, 12 },
		{ 7, 3, 14, 13, 4, 1, 10, 8, 5, 12, 9, 11, 2, 15, 6, 0 },
		{ 11, 4, 2, 7, 1, 0, 10, 15, 6, 9, 14, 8, 3, 13, 5, 12 },
		{ 5, 7, 3, 12, 15, 13, 14, 8, 0, 10, 9, 6, 1, 4, 2, 11 },
		{ 14, 1, 8, 15, 2, 6, 0, 3, 9, 12, 10, 13, 4, 7, 5, 11 },
		{ 13, 14, 6, 12, 4, 5, 1, 0, 9, 3, 10, 2, 15, 11, 8, 7 },
		{ 9, 8, 0, 2, 15, 1, 4, 14, 3, 10, 7, 5, 11, 13, 6, 12 },
		{ 12, 15, 2, 6, 1, 14, 4, 8, 5, 3, 7, 0, 10, 13, 9, 11 },
		{ 12, 8, 15, 13, 1, 0, 5, 4, 6, 3, 2, 11, 9, 7, 14, 10 },
		{ 14, 10, 9, 4, 13, 6, 5, 8, 2, 12, 7, 0, 1, 3, 11, 15 },
		{ 14, 3, 5, 15, 11, 6, 13, 9, 0, 10, 2, 12, 4, 1, 7, 8 },
		{ 6, 11, 7, 8, 13, 2, 5, 4, 1, 10, 3, 9, 14, 0, 12, 15 },
		{ 1, 6, 12, 14, 3, 2, 15, 8, 4, 5, 13, 9, 0, 7, 11, 10 },
		{ 12, 6, 0, 4, 7, 3, 15, 1, 13, 9, 8, 11, 2, 14, 5, 10 },
		{ 8, 1, 7, 12, 11, 0, 10, 5, 9, 15, 6, 13, 14, 2, 3, 4 },
		{ 7, 15, 8, 2, 13, 6, 3, 12, 11, 0, 4, 10, 9, 5, 1, 14 },
		{ 9, 0, 4, 10, 1, 14, 15, 3, 12, 6, 5, 7, 11, 13, 8, 2 },
		{ 11, 5, 1, 14, 4, 12, 10, 0, 2, 7, 13, 3, 9, 15, 6, 8 },
		{ 8, 13, 10, 9, 11, 3, 15, 6, 0, 1, 2, 14, 12, 5, 4, 7 },
		{ 4, 5, 7, 2, 9, 14, 12, 13, 0, 3, 6, 11, 8, 1, 15, 10 },
		{ 11, 15, 14, 13, 1, 9, 10, 4, 3, 6, 2, 12, 7, 5, 8, 0 },
		{ 12, 9, 0, 6, 8, 3, 5, 14, 2, 4, 11, 7, 10, 1, 15, 13 },
		{ 3, 14, 9, 7, 12, 15, 0, 4, 1, 8, 5, 6, 11, 10, 2, 13 },
		{ 8, 4, 6, 1, 14, 12, 2, 15, 13, 10, 9, 5, 3, 7, 0, 11 },
		{ 6, 10, 1, 14, 15, 8, 3, 5, 13, 0, 2, 7, 4, 9, 11, 12 },
		{ 8, 11, 4, 6, 7, 3, 10, 9, 2, 12, 15, 13, 0, 1, 5, 14 },
		{ 10, 0, 2, 4, 5, 1, 6, 12, 11, 13, 9, 7, 15, 3, 14, 8 },
		{ 12, 5, 13, 11, 2, 10, 0, 9, 7, 8, 4, 3, 14, 6, 15, 1 },
		{ 10, 2, 8, 4, 15, 0, 1, 14, 11, 13, 3, 6, 9, 7, 5, 12 },
		{ 10, 8, 0, 12, 3, 7, 6, 2, 1, 14, 4, 11, 15, 13, 9, 5 },
		{ 14, 9, 12, 13, 15, 4, 8, 10, 0, 2, 1, 7, 3, 11, 5, 6 },
		{ 12, 11, 0, 8, 10, 2, 13, 15, 5, 4, 7, 3, 6, 9, 14, 1 },
		{ 13, 8, 14, 3, 9, 1, 0, 7, 15, 5, 4, 10, 12, 2, 6, 11 },
		{ 3, 15, 2, 5, 11, 6, 4, 7, 12, 9, 1, 0, 13, 14, 10, 8 },
		{ 5, 11, 6, 9, 4, 13, 12, 0, 8, 2, 15, 10, 1, 7, 3, 14 },
		{ 5, 0, 15, 8, 4, 6, 1, 14, 10, 11, 3, 9, 7, 12, 2, 13 },
		{ 15, 14, 6, 7, 10, 1, 0, 11, 12, 8, 4, 9, 2, 5, 13, 3 },
		{ 11, 14, 13, 1, 2, 3, 12, 4, 15, 7, 9, 5, 10, 6, 8, 0 },
		{ 6, 13, 3, 2, 11, 9, 5, 10, 1, 7, 12, 14, 8, 4, 0, 15 },
		{ 4, 6, 12, 0, 14, 2, 9, 13, 11, 8, 3, 15, 7, 10, 1, 5 },
		{ 8, 10, 9, 11, 14, 1, 7, 15, 13, 4, 0, 12, 6, 2, 5, 3 },
		{ 5, 2, 14, 0, 7, 8, 6, 3, 11, 12, 13, 15, 4, 10, 9, 1 },
		{ 7, 8, 3, 2, 10, 12, 4, 6, 11, 13, 5, 15, 0, 1, 9, 14 },
		{ 11, 6, 14, 12, 3, 5, 1, 15, 8, 0, 10, 13, 9, 7, 4, 2 },
		{ 7, 1, 2, 4, 8, 3, 6, 11, 10, 15, 0, 5, 14, 12, 13, 9 },
		{ 7, 3, 1, 13, 12, 10, 5, 2, 8, 0, 6, 11, 14, 15, 4, 9 },
		{ 6, 0, 5, 15, 1, 14, 4, 9, 2, 13, 8, 10, 11, 12, 7, 3 },
		{ 15, 1, 3, 12, 4, 0, 6, 5, 2, 8, 14, 9, 13, 10, 7, 11 },
		{ 5, 7, 0, 11, 12, 1, 9, 10, 15, 6, 2, 3, 8, 4, 13, 14 },
		{ 12, 15, 11, 10, 4, 5, 14, 0, 13, 7, 1, 2, 9, 8, 3, 6 },
		{ 6, 14, 10, 5, 15, 8, 7, 1, 3, 4, 2, 0, 12, 9, 11, 13 },
		{ 14, 13, 4, 11, 15, 8, 6, 9, 0, 7, 3, 1, 2, 10, 12, 5 },
		{ 14, 4, 0, 10, 6, 5, 1, 3, 9, 2, 13, 15, 12, 7, 8, 11 },
		{ 15, 10, 8, 3, 0, 6, 9, 5, 1, 14, 13, 11, 7, 2, 12, 4 },
		{ 0, 13, 2, 4, 12, 14, 6, 9, 15, 1, 10, 3, 11, 5, 8, 7 },
		{ 3, 14, 13, 6, 4, 15, 8, 9, 5, 12, 10, 0, 2, 7, 1, 11 },
		{ 0, 1, 9, 7, 11, 13, 5, 3, 14, 12, 4, 2, 8, 6, 10, 15 },
		{ 11, 0, 15, 8, 13, 12, 3, 5, 10, 1, 4, 6, 14, 9, 7, 2 },
		{ 13, 0, 9, 12, 11, 6, 3, 5, 15, 8, 1, 10, 4, 14, 2, 7 },
		{ 14, 10, 2, 1, 13, 9, 8, 11, 7, 3, 6, 12, 15, 5, 4, 0 },
		{ 12, 3, 9, 1, 4, 5, 10, 2, 6, 11, 15, 0, 14, 7, 13, 8 },
		{ 15, 8, 10, 7, 0, 12, 14, 1, 5, 9, 6, 3, 13, 11, 4, 2 },
		{ 4, 7, 13, 10, 1, 2, 9, 6, 12, 8, 14, 5, 3, 0, 11, 15 },
		{ 6, 0, 5, 10, 11, 12, 9, 2, 1, 7, 4, 3, 14, 8, 13, 15 },
		{ 9, 5, 11, 10, 13, 0, 2, 1, 8, 6, 14, 12, 4, 7, 3, 15 },
		{ 15, 2, 12, 11, 14, 13, 9, 5, 1, 3, 8, 7, 0, 10, 6, 4 },
		{ 11, 1, 7, 4, 10, 13, 3, 8, 9, 14, 0, 15, 6, 5, 2, 12 },
		{ 5, 4, 7, 1, 11, 12, 14, 15, 10, 13, 8, 6, 2, 0, 9, 3 },
		{ 9, 7, 5, 2, 14, 15, 12, 10, 11, 3, 6, 1, 8, 13, 0, 4 },
		{ 3, 2, 7, 9, 0, 15, 12, 4, 6, 11, 5, 14, 8, 13, 10, 1 },
		{ 13, 9, 14, 6, 12, 8, 1, 2, 3, 4, 0, 7, 5, 10, 11, 15 },
		{ 5, 7, 11, 8, 0, 14, 9, 13, 10, 12, 3, 15, 6, 1, 4, 2 },
		{ 4, 3, 6, 13, 7, 15, 9, 0, 10, 5, 8, 11, 2, 12, 1, 14 },
		{ 1, 7, 15, 14, 2, 6, 4, 9, 12, 11, 13, 3, 0, 8, 5, 10 },
		{ 9, 14, 5, 7, 8, 15, 1, 2, 10, 4, 13, 6, 12, 0, 11, 3 },
		{ 0, 11, 3, 12, 5, 2, 1, 9, 8, 10, 14, 15, 7, 4, 13, 6 },
		{ 7, 15, 4, 0, 10, 9, 2, 5, 12, 11, 13, 6, 1, 3, 14, 8 },
		{ 11, 4, 0, 8, 6, 10, 5, 13, 12, 7, 14, 3, 1, 2, 9, 15 },
			//easy ones
		{ 4, 1, 2, 3, 9, 8, 6, 7, 12, 5, 10, 11, 13, 0, 14, 15 },
		{ 4, 1, 2, 3, 5, 6, 10, 7, 8, 9, 14, 11, 12, 0, 13, 15 },
		{ 4, 1, 2, 3, 8, 0, 6, 7, 9, 5, 10, 11, 12, 13, 14, 15 },
		{ 1, 2, 6, 3, 4, 5, 10, 7, 8, 9, 11, 15, 12, 13, 14, 0 },
		{ 4, 1, 2, 3, 8, 0, 6, 7, 9, 5, 10, 11, 12, 13, 14, 15 },
		{ 5, 4, 2, 3, 1, 0, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
		{ 1, 2, 3, 7, 4, 5, 6, 11, 8, 9, 14, 10, 12, 0, 13, 15 },
		{ 4, 1, 2, 3, 5, 6, 7, 0, 8, 9, 10, 11, 12, 13, 14, 15 },
		{ 4, 1, 2, 3, 5, 6, 7, 0, 8, 9, 10, 11, 12, 13, 14, 15 },
		{ 1, 5, 2, 3, 4, 9, 6, 7, 0, 13, 10, 11, 8, 12, 14, 15 }
 };

	for (int i = 0; i < 16; i++)
	{
		s.puzzle[i] = instances[index][i];
		if (instances[index][i] == 0)
			s.blankIdx = i;
	}
#ifdef ASGS
	SlidingTilePuzzleState goal(4, 4);
	goal.Reset();
	s.hCost = 0;
	int xdiff = 0;
	int ydiff = 0;
	int size = s.puzzle.size();
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (goal.puzzle[j] == s.puzzle[i] && goal.puzzle[j] != 0)
			{
				ydiff = j / s.width > i / s.width ? (j / s.width - i / s.width) : (i / s.width - j / s.width);
				xdiff = j%s.width > i%s.width ? (j%s.width - i%s.width) : (i%s.width - j%s.width);
				s.hCost += xdiff + ydiff;
				break;
			}
		}
	}
#endif
}
