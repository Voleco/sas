
#include <iostream>
#include <vector>
#include <time.h>
#include <string>
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

		std::ifstream fin;
		fin.open("../../resources/benchmarks/orz301d.map.scen");
		if (!fin.is_open())
		{
			std::cout << "fail to load benchmark file: " << "../../resources/benchmarks/orz301d.map.scen" << "\n";
			return 1;
		}

		std::string str;
		fin >> str;
		fin >> str;
		std::vector<int> startx, starty, goalx, goaly;
		std::vector<double> expectedCost;
		while (!fin.eof())
		{
			fin >> str;
			fin >> str;
			fin >> str;
			fin >> str;

			fin >> str;
			startx.push_back(std::stoi(str));
			fin >> str;
			starty.push_back(std::stoi(str));
			fin >> str;
			goalx.push_back(std::stoi(str));
			fin >> str;
			goaly.push_back(std::stoi(str));
			fin >> str;
			expectedCost.push_back(std::stod(str));
		}

		OctileDistanceHeuristic* odh = new OctileDistanceHeuristic();

		MyAStar<Map2DState, Map2DAction, Map2D, OctileDistanceHeuristic>
			*astar1;

		clock_t startTime;
		clock_t endTime;
		clock_t clockTicksTaken;
		double timeInSeconds;

		double solutionCost;
		for (int i = 0; i < startx.size(); i++)
		{
			Map2DState start(startx[i], starty[i]);
			Map2DState goal(goalx[i], goaly[i]);

			odh->SetGoal(goal);

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
				solutionCost = astar1->GetSolutionCost();
				std::cout << solutionCost << "\t";
			}

			delete astar1;

			endTime = clock();
			clockTicksTaken = endTime - startTime;
			timeInSeconds = clockTicksTaken / (double)CLOCKS_PER_SEC;
			std::cout << timeInSeconds << "\n";

			if (solutionCost - expectedCost[i] > 0.01 || solutionCost - expectedCost[i] < -0.01)
			{
				std::cout << "error solution cost:\t expected cost\n";
				std::cout << solutionCost<<"\t" <<expectedCost[i] <<"\n";
				return 1;
			}
		}
	}


	else
	{
		std::cout << "Usage: " << argv[0] << " -i [low] [high] \n";
	}


	return 0;
}
