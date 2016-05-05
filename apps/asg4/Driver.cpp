
#include <iostream>
#include <vector>
#include <time.h>
#include <cstring>
#include "MyInefficientAStar.h"
#include "MyAStar.h"
#include "SlidingTilePuzzle.h"


void GetSildingTileInstance(int index, SlidingTilePuzzleState& s);

int main(int argc,char** argv)
{
	if (argc > 1 && strcmp(argv[1], "-test") == 0)
	{
		SlidingTilePuzzle puzzle(4, 4);
		SlidingTilePuzzleState start(4, 4);
		SlidingTilePuzzleState goal(4, 4);
		goal.Reset();
		GetSildingTileInstance(atoi(argv[2]), start);

		uint64_t rank;
		std::cout << "origin state: " << start<<"\n";
		puzzle.GetRankFromState(start, rank);
		std::cout << "rank: " << rank << "\n";
		puzzle.GetStateFromRank(start, rank);
		std::cout << "retrieved state: " << start << "\n";
	}

	else if (argc > 1 && strcmp(argv[1], "-i") == 0)
	{
		int low = 1;
		int high = 100;
		if (argc > 3)
		{
			low = atoi(argv[2]);
			high = atoi(argv[3]);
		}


		SlidingTilePuzzle puzzle(4,4);
		SlidingTilePuzzleState start(4, 4);
		SlidingTilePuzzleState goal(4, 4);
		goal.Reset();

		ManhattanDistanceHeuristic* mh = new ManhattanDistanceHeuristic();
		mh->SetGoal(goal);

		MyInefficientAStar<SlidingTilePuzzleState, SlidingTilePuzzleAction, SlidingTilePuzzle, ManhattanDistanceHeuristic>
			*astar1;

		clock_t startTime;
		clock_t endTime;
		clock_t clockTicksTaken;
		double timeInSeconds;

		SlidingTilePuzzleState startcp(4, 4);
		SlidingTilePuzzleState goalcp(4, 4);
		for (int i = low - 1; i < high; i++)
		{
			
			GetSildingTileInstance(i, start);

			astar1 = new MyInefficientAStar<SlidingTilePuzzleState, SlidingTilePuzzleAction, SlidingTilePuzzle, ManhattanDistanceHeuristic>
				(*mh);

			std::cout << "********************************\n"
				<< "Puzzle " << i + 1 << " of 100\n"
				<< "start: " << start
				<< "goal: " << goal << "\n";
			std::vector<SlidingTilePuzzleAction> acs;

			startTime = clock();
			acs.resize(0);
			puzzle = SlidingTilePuzzle(4, 4);
			startcp = SlidingTilePuzzleState(start);
			goalcp = SlidingTilePuzzleState(goal);
			if (astar1->GetPath(puzzle, startcp, goalcp))
			{
				std::cout << "Inefficient A* w/ MD found a path!\n";
				std::cout << "nodes expanded:\tPath length:\ttime spent(s)\n";
				std::cout<< astar1->GetNodesExpanded() << "\t";
				std::cout << astar1->GetSolutionCost() << "\t";
			}	
			delete astar1;
			endTime = clock();
			clockTicksTaken = endTime - startTime;
			timeInSeconds = clockTicksTaken / (double)CLOCKS_PER_SEC;
			std::cout << timeInSeconds << "\n";
		}
	}

	else if (argc > 1 && strcmp(argv[1], "-a") == 0)
	{
		int low = 1;
		int high = 100;
		if (argc > 3)
		{
			low = atoi(argv[2]);
			high = atoi(argv[3]);
		}


		SlidingTilePuzzle puzzle(4, 4);
		SlidingTilePuzzleState start(4, 4);
		SlidingTilePuzzleState goal(4, 4);
		goal.Reset();

		ManhattanDistanceHeuristic* mh = new ManhattanDistanceHeuristic();
		mh->SetGoal(goal);

		MyAStar<SlidingTilePuzzleState, SlidingTilePuzzleAction, SlidingTilePuzzle, ManhattanDistanceHeuristic>
			*astar1;

		clock_t startTime;
		clock_t endTime;
		clock_t clockTicksTaken;
		double timeInSeconds;

		SlidingTilePuzzleState startcp(4, 4);
		SlidingTilePuzzleState goalcp(4, 4);
		for (int i = low - 1; i < high; i++)
		{

			GetSildingTileInstance(i, start);

			astar1 = new MyAStar<SlidingTilePuzzleState, SlidingTilePuzzleAction, SlidingTilePuzzle, ManhattanDistanceHeuristic>
				(*mh);

			std::cout << "********************************\n"
				<< "Puzzle " << i + 1 << " of 100\n"
				<< "start: " << start
				<< "goal: " << goal << "\n";
			std::vector<SlidingTilePuzzleAction> acs;

			startTime = clock();
			acs.resize(0);
			puzzle = SlidingTilePuzzle(4, 4);
			startcp = SlidingTilePuzzleState(start);
			goalcp = SlidingTilePuzzleState(goal);
			if (astar1->GetPath(puzzle, startcp, goalcp))
			{
				std::cout << "A* w/ MD found a path!\n";
				std::cout << "nodes expanded:\tPath length:\ttime spent(s)\n";
				std::cout << astar1->GetNodesExpanded() << "\t";
				std::cout << astar1->GetSolutionCost() << "\t";
			}
			delete astar1;
			endTime = clock();
			clockTicksTaken = endTime - startTime;
			timeInSeconds = clockTicksTaken / (double)CLOCKS_PER_SEC;
			std::cout << timeInSeconds << "\n";
		}
	}


	else
	{
		std::cout << "Usage: " << argv[0] << " -i [low] [high] \n";
	}


	return 0;
}
