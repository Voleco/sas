
#include <iostream>
#include <vector>
#include <time.h>
#include <cstring>
#include <fstream>
#include "MyInefficientAStar.h"
#include "MyAStar.h"
#include "SlidingTilePuzzle.h"
#include "Map2D.h"


int main(int argc,char** argv)
{
	if (argc > 1 && strcmp(argv[1], "-testrank") == 0)
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
	else if (argc > 1 && strcmp(argv[1], "-testmap") == 0)
	{
		Map2D map;
		map.LoadMap("../../resources/maps/orz301d.map");
	}

	else if (argc > 1 && strcmp(argv[1], "-amap") == 0)
	{
		Map2D map;
		if (!map.LoadMap("../../resources/maps/orz301d.map"))
		{
			std::cout << "fail to load map: " << "../../resources/maps/orz301d.map" << "\n";
			return 1;
		}

		Map2DState start(atoi(argv[2]), atoi(argv[3]));
		Map2DState goal(atoi(argv[4]), atoi(argv[5]));

		OctileDistanceHeuristic* odh = new OctileDistanceHeuristic();
		odh->SetGoal(goal);

		MyAStar<Map2DState, Map2DAction, Map2D, OctileDistanceHeuristic>
			*astar1;

		clock_t startTime;
		clock_t endTime;
		clock_t clockTicksTaken;
		double timeInSeconds;

		int low = 1;
		int high = 1;
		for (int i = low - 1; i < high; i++)
		{
			astar1 = new MyAStar<Map2DState, Map2DAction, Map2D, OctileDistanceHeuristic>
				(*odh);

			std::cout << "********************************\n"
				<< "test case" << i + 1 << "\n"
				<< "start: " << start
				<< "goal: " << goal << "\n";

			startTime = clock();
			if (astar1->GetPath(map, start, goal))
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
