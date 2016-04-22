
#include <iostream>
#include <vector>
#include <time.h>
#include <cstring>
#include "MyIDAStar.h"
#include "SlidingTilePuzzle.h"
#include "NAryTree.h"
#include "GridBasedMap.h"



int main(int argc,char** argv)
{
	if (argc > 1 && strcmp(argv[1], "-stp") == 0)
	{
		bool showPath = false;
		int low = 1;
		int high = 100;
		if (argc > 3)
		{
			low = atoi(argv[2]);
			high = atoi(argv[3]);
		}
		if (argc > 4 && strcmp(argv[4], "-showpath") == 0)
			showPath = true;

		SlidingTilePuzzle puzzle(4,4);
		SlidingTilePuzzleState start(4, 4);
		SlidingTilePuzzleState goal(4, 4);
		ManhattanDistanceHeuristic heur;
		heur.SetGoal(goal);


		MyIDAStar<SlidingTilePuzzleState, SlidingTilePuzzleAction, SlidingTilePuzzle, ManhattanDistanceHeuristic>
			*ida;

		clock_t startTime;
		clock_t endTime;
		clock_t clockTicksTaken;
		double timeInSeconds;
		//int i = atoi(argv[1]);
		for (int i = low - 1; i < high; i++)
		{
			startTime = clock();
			GetSildingTileInstance(i, start);
			ida = new MyIDAStar<SlidingTilePuzzleState, SlidingTilePuzzleAction, SlidingTilePuzzle, ManhattanDistanceHeuristic>(heur, 500);
			std::cout << "********************************\n"
				<< "Puzzle " << i + 1 << " of 100\n"
				<< "start: " << start
				<< "goal: " << goal << "\n";
			std::vector<SlidingTilePuzzleAction> acs;
			puzzle = SlidingTilePuzzle(4, 4);
			if (ida->GetPath(puzzle, start, goal))
			{
				std::cout << "IDA* found a path!\n";
				acs = ida->GetActionSequence();
				std::cout << "Path length: " << acs.size() << "\n";
				if (showPath)
				{
					std::cout << "Path: ";
					for (int i = 0; i < acs.size(); i++)
						std::cout << acs[i] << " ";
					std::cout << "\n";
				}
			}

			else
				std::cout << "IDA* did not find a path.\n";
			std::cout << "nodes expanded: " << ida->GetNodesExpanded() << "\n";

			delete ida;

			endTime = clock();
			clockTicksTaken = endTime - startTime;
			timeInSeconds = clockTicksTaken / (double)CLOCKS_PER_SEC;
			std::cout << "time spent: " << timeInSeconds << " seconds\n";
		}

	}
	else if (argc > 4 && strcmp(argv[1], "-nat") == 0)
	{
		bool showPath = false;
		NAryTree tree(atoi(argv[2]), atoi(argv[3]));
		NAryTreeState start = 0;
		NAryTreeState goal = atoi(argv[4]);
		NAryTreeHeuristic heur;
		heur.SetGoal(goal);
		
		if (argc > 5 && strcmp(argv[5], "-showpath") == 0)
			showPath = true;

		MyIDAStar<NAryTreeState, NAryTreeAction, NAryTree, NAryTreeHeuristic>
			*ida;

		ida = new MyIDAStar<NAryTreeState, NAryTreeAction, NAryTree, NAryTreeHeuristic>(heur, 1+tree.GetSolutionDepthUpperBound(start, goal));
		std::cout << "********************************\n"
			<< "start: " << start
			<< "goal: " << goal << "\n";
		std::vector<NAryTreeAction> acs;
		if (ida->GetPath(tree, start, goal))
		{
			std::cout << "IDA* found a path!\n";
			acs = ida->GetActionSequence();
			std::cout << "Path length: " << acs.size() << "\n";
			if (showPath)
			{
				std::cout << "Path: ";
				for (int i = 0; i < acs.size(); i++)
					std::cout << acs[i] << " ";
				std::cout << "\n";
			}
		}

		else
			std::cout << "IDA* did not find a path.\n";
		std::cout << "nodes expanded: " << ida->GetNodesExpanded() << "\n";

		delete ida;

	}
	else if (argc > 5 && strcmp(argv[1], "-gbm") == 0)
	{
		bool showPath = false;
		GridBasedMap map(atoi(argv[2]), atoi(argv[3]));
		GridBasedMapState start(0, 0);
		GridBasedMapState goal(atoi(argv[4]), atoi(argv[5]));
		GridBasedMapHeuristic heur;
		heur.SetGoal(goal);

		if (argc > 6 && strcmp(argv[6], "-showpath") == 0)
			showPath = true;

		MyIDAStar<GridBasedMapState, GridBasedMapAction, GridBasedMap, GridBasedMapHeuristic>
			*ida;

		//std::cout << "\nestimated upperbound: " << map.GetSolutionDepthUpperBound(start, goal) << "\n";
		ida = new MyIDAStar<GridBasedMapState, GridBasedMapAction, GridBasedMap, GridBasedMapHeuristic>( heur, 1+map.GetSolutionDepthUpperBound(start, goal));
		std::cout << "********************************\n"
			<< "start: " << start
			<< "goal: " << goal << "\n";
		std::vector<GridBasedMapAction> acs;
		if (ida->GetPath(map, start, goal))
		{
			std::cout << "IDA* found a path!\n";
			acs = ida->GetActionSequence();
			std::cout << "Path length: " << acs.size() << "\n";
			if (showPath)
			{
				std::cout << "Path: ";
				for (int i = 0; i < acs.size(); i++)
					std::cout << acs[i] << " ";
				std::cout << "\n";
			}
		}

		else
			std::cout << "IDA* did not find a path.\n";
		std::cout << "nodes expanded: " << ida->GetNodesExpanded() << "\n";

		delete ida;
	}
	else
	{
		std::cout << "Usage: " << argv[0] << " -stp [low] [high] [-showpath]\n"
			<< argv[0] << " -nat <branching factor> <depth> <goal> [-showpath]\n"
			<< argv[0] << " -gbm <map X size> <map Y size> <goal X> <goal Y> [-showpath]\n";
	}


	return 0;
}
