
#include <iostream>
#include <vector>
#include <time.h>
#include <cstring>
#include "MyAStar.h"
#include "SlidingTilePuzzle.h"
#include "Map2D.h"


int main(int argc,char** argv)
{
	if (argc > 1 && strcmp(argv[1], "-testsave") == 0)
	{
		Map2D map;
		Map2DDifferentialHeuristic heur(map);
		heur.LoadMap("../../resources/maps/lak303d.map");
		heur.SaveAsBMP("../../resources/maps/lak303d.bmp",-1);
	}

	else if (argc > 1 && strcmp(argv[1], "-test2") == 0)
	{
		Map2D map;
		Map2DDifferentialHeuristic heur(map);
		heur.LoadMap("../../resources/maps/lak303d.map");
		Map2DState s(60,60);
		heur.AddPivot(s);
		heur.BuildPDBs();
		//heur.SaveAsBMP("../../resources/maps/lak303d.bmp", -1);
		//heur.SaveAsBMP("../../resources/maps/lak303d.bmp",0);
	}

	else if (argc > 1 && strcmp(argv[1], "-test3") == 0)
	{
		Map2D map;
		Map2DDifferentialHeuristic heur(map);
		heur.LoadMap("../../resources/maps/lak303d.map");
		std::cout << "map.width"<< heur.width<<"\n";
		std::cout << "map.height" << heur.height << "\n";

		Map2DState s;
		std::vector<Map2DAction> actions;
		int x, y;
		srand(time(NULL));
		map.LoadMap("../../resources/maps/lak303d.map");
		while (actions.size() == 0)
		{
			x = rand() % map.width;
			y = rand() % map.height;
			std::cout << "x, y" << x << " ," << y << "\n";
			s = Map2DState(x, y);
			map.GetActions(s, actions);
		}

		heur.AddPivot(s);
		Map2DState next;
		for (int i = 0; i < 9; i++)
		{
			next = heur.GetFarthestPivot();
			heur.AddPivot(next);
		}
		heur.BuildPDBs();
		for (int i = -1; i < 10;i++)
			heur.SaveAsBMP("../../resources/maps/lak303d", i);
		heur.SaveAsBMP("../../resources/maps/lak303d", 0);
	}


	else if (argc > 1 && strcmp(argv[1], "-amaptest") == 0)
	{
		Map2D map;

		Map2DDifferentialHeuristic heur(map);

		if (!heur.LoadMap("../../resources/maps/lak303d.map"))
		{
			std::cout << "fail to load map: " << "../../resources/maps/lak303d.map" << "\n";
			return 1;
		}

		Map2DState s;
		std::vector<Map2DAction> actions;
		int x, y;
		srand(time(NULL));
		map.LoadMap("../../resources/maps/lak303d.map");
		while (actions.size() == 0)
		{
			x = rand() % map.width;
			y = rand() % map.height;
			s = Map2DState(x, y);
			map.GetActions(s, actions);
		}

		heur.AddPivot(s);
		Map2DState next;
		for (int i = 0; i < 9; i++)
		{
			next = heur.GetFarthestPivot();
			heur.AddPivot(next);
		}
		heur.BuildPDBs();
		for (int i = -1; i < 10; i++)
			heur.SaveAsBMP("../../resources/maps/lak303d/", i);


		std::ifstream fin;
		fin.open("../../resources/benchmarks/lak303d.map.scen");
		if (!fin.is_open())
		{
			std::cout << "fail to load benchmark file: " << "../../resources/benchmarks/lak303d.map.scen" << "\n";
			return 1;
		}

		std::string str;
		//get rid of "version 1"
		fin >> str;
		fin >> str;
		std::vector<int> startx, starty, goalx, goaly;
		std::vector<double> expectedCost;
		while (!fin.eof())
		{
			//first 4 should be "group", "mapname", "map width", "map height"
			//which are not helpful for this assignment
			fin >> str;
			fin >> str;
			fin >> str;
			if (std::stoi(str) != heur.width)
				break;
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


		MyAStar<Map2DState, Map2DAction, Map2D, Map2DDifferentialHeuristic>
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

			heur.SetGoal(goal);

			astar1 = new MyAStar<Map2DState, Map2DAction, Map2D, Map2DDifferentialHeuristic>
				(heur);

			std::cout << "********************************\n"
				<< "test case" << i + 1 << "\n"
				<< "start: " << start
				<< "goal: " << goal << "\n";

			startTime = clock();
			if (astar1->GetPath(map, start, goal))
			{
				std::cout << "A* w/ ODH found a path!\n";
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
				std::cout << solutionCost << "\t" << expectedCost[i] << "\n";
				return 1;
			}
			else
				std::cout << "passed!\n";
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
