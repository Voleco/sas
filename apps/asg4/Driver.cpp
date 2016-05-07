
#include <iostream>
#include <vector>
#include <time.h>
#include <cstring>
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
			low = atoi(argv[3]);
			high = atoi(argv[4]);
		}

		SlidingTilePuzzle puzzle(4, 4);
		SlidingTilePuzzleState start(4, 4);
		SlidingTilePuzzleState goal(4, 4);
		goal.Reset();

		//heurs2 contains ManhattanDistance, pdb a-d
		std::vector<MyHeuristic<SlidingTilePuzzleState>*> heurs2;
		//heurs3 contains ManhattanDistance, pdb c-e
		std::vector<MyHeuristic<SlidingTilePuzzleState>*> heurs3;

		ManhattanDistanceHeuristic* mh = new ManhattanDistanceHeuristic();
		mh->SetGoal(goal);

		heurs2.push_back(mh);
		heurs3.push_back(mh);

		std::vector<std::vector<int>> patterns;
		patterns.push_back(std::vector<int>({ 0,1,4,5 }));
		patterns.push_back(std::vector<int>({ 0,2,3,6,7 }));
		patterns.push_back(std::vector<int>({ 0,8,9,12,13 }));
		patterns.push_back(std::vector<int>({ 0,10,11,14,15 }));
		patterns.push_back(std::vector<int>({ 0,1,2,3,4,5,6,7 }));
		//build PDB a to e
		for (int i = 0; i < 5; i++)
		{
			SlidingTilePuzzlePDB* pdb = new SlidingTilePuzzlePDB(puzzle, goal, patterns[i]);
			if (!pdb->Load(argv[2]))
			{
				pdb->BuildPDB();
				pdb->Save(argv[2]);
			}
			if (i < 4)
				heurs2.push_back(pdb);
			if (i > 1)
				heurs3.push_back(pdb);
		}

		MaxHeuristic<SlidingTilePuzzleState> maxH2(heurs2);
		MaxHeuristic<SlidingTilePuzzleState> maxH3(heurs3);

		MyAStar<SlidingTilePuzzleState, SlidingTilePuzzleAction, SlidingTilePuzzle, ManhattanDistanceHeuristic>
			*astar1;
		MyAStar<SlidingTilePuzzleState, SlidingTilePuzzleAction, SlidingTilePuzzle, MaxHeuristic<SlidingTilePuzzleState>>
			*astar2;
		MyAStar<SlidingTilePuzzleState, SlidingTilePuzzleAction, SlidingTilePuzzle, MaxHeuristic<SlidingTilePuzzleState>>
			*astar3;

		clock_t startTime;
		clock_t endTime;
		clock_t clockTicksTaken;
		double timeInSeconds;
		//int i = atoi(argv[1]);
		SlidingTilePuzzleState startcp(4, 4);
		SlidingTilePuzzleState goalcp(4, 4);
		for (int i = low - 1; i < high; i++)
		{

			GetSildingTileInstance(i, start);

			astar1 = new MyAStar<SlidingTilePuzzleState, SlidingTilePuzzleAction, SlidingTilePuzzle, ManhattanDistanceHeuristic>
				(*mh);
			astar2 = new MyAStar<SlidingTilePuzzleState, SlidingTilePuzzleAction, SlidingTilePuzzle, MaxHeuristic<SlidingTilePuzzleState>>
				(maxH2);
			astar3 = new MyAStar<SlidingTilePuzzleState, SlidingTilePuzzleAction, SlidingTilePuzzle, MaxHeuristic<SlidingTilePuzzleState>>
				(maxH3);

			std::cout << "********************************\n"
				<< "Puzzle " << i + 1 << " of 100\n"
				<< "start: " << start
				<< "goal: " << goal << "\n";

			startTime = clock();
			puzzle = SlidingTilePuzzle(4, 4);
			startcp = SlidingTilePuzzleState(start);
			goalcp = SlidingTilePuzzleState(goal);
			if (astar1->GetPath(puzzle, startcp, goalcp))
			{
				std::cout << "A* w/ only MD found a path!\n";
				std::cout << "nodes expanded:\tSolution cost:\ttime spent(s)\n";
				std::cout << astar1->GetNodesExpanded() << "\t";
				std::cout << astar1->GetSolutionCost() << "\t";
			}
			delete astar1;
			endTime = clock();
			clockTicksTaken = endTime - startTime;
			timeInSeconds = clockTicksTaken / (double)CLOCKS_PER_SEC;
			std::cout << timeInSeconds << "\n";

			startTime = clock();
			puzzle = SlidingTilePuzzle(4, 4);
			startcp = SlidingTilePuzzleState(start);
			goalcp = SlidingTilePuzzleState(goal);
			if (astar2->GetPath(puzzle, startcp, goalcp))
			{
				std::cout << "A* w/ max{MD, PDBa-d} found a path!\n";
				std::cout << "nodes expanded:\tPath length:\ttime spent(s)\n";
				std::cout << astar2->GetNodesExpanded() << "\t";
				std::cout << astar2->GetSolutionCost() << "\t";
			}
			delete astar2;
			endTime = clock();
			clockTicksTaken = endTime - startTime;
			timeInSeconds = clockTicksTaken / (double)CLOCKS_PER_SEC;
			std::cout << timeInSeconds << "\n";

			startTime = clock();
			puzzle = SlidingTilePuzzle(4, 4);
			startcp = SlidingTilePuzzleState(start);
			goalcp = SlidingTilePuzzleState(goal);
			if (astar3->GetPath(puzzle, startcp, goalcp))
			{
				std::cout << "A* w/  max{MD, PDBc-e} found a path!\n";
				std::cout << "nodes expanded:\tPath length:\ttime spent(s)\n";
				std::cout << astar3->GetNodesExpanded() << "\t";
				std::cout << astar3->GetSolutionCost() << "\t";
			}
			delete astar3;
			endTime = clock();
			clockTicksTaken = endTime - startTime;
			timeInSeconds = clockTicksTaken / (double)CLOCKS_PER_SEC;
			std::cout << timeInSeconds << "\n";

		}
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
		std::cout << "Usage: " << argv[0] << " -i [low] [high] \n"
			<< argv[0] << " -a <heuristic directory> [low] [high] \n"
			<< argv[0] << " -amap <start x> <start y> <goal x> <goal y> \n";
	}


	return 0;
}
