
#include <iostream>
#include <vector>
#include <time.h>
#include <cstring>
#include "MyAStar.h"
#include "SlidingTilePuzzle.h"
#include "Map2D.h"

bool LoadBenchmark(std::vector<int>& startx, std::vector<int>& starty, std::vector<int>& goalx, std::vector<int>& goaly,
	std::vector<double>& expectedCost, std::string fileName);

int main(int argc,char** argv)
{
	if (argc > 1 && strcmp(argv[1], "-oct") == 0)
	{

		Map2D map;
		map.LoadMap("../../resources/maps/lak303d.map");

		OctileDistanceHeuristic odh;


		//load the benchmark
		std::vector<int> startx, starty, goalx, goaly;
		std::vector<double> expectedCost;
		if (!LoadBenchmark(startx, starty, goalx, goaly, expectedCost, "../../resources/benchmarks/lak303d.map.scen"))
			return 1;


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

			odh.SetGoal(goal);

			astar1 = new MyAStar<Map2DState, Map2DAction, Map2D, OctileDistanceHeuristic>
				(odh);

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


	else if (argc > 1 && strcmp(argv[1], "-far") == 0)
	{
		int k = 10;
		if (argc > 2)
			k = atoi(argv[2]);
		Map2D map;

		//initialize differential heuristic
		Map2DDifferentialHeuristic diffHeur(map);
		if (!diffHeur.LoadMap("../../resources/maps/lak303d.map"))
		{
			std::cout << "fail to load map: " << "../../resources/maps/lak303d.map" << "\n";
			return 1;
		}

		//choose a random pivot and get its farthest point
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
		diffHeur.AddPivot(s);

		Map2DState next;
		next = diffHeur.GetFarthestPivot();
		diffHeur.ChangePivot(next,0);
		//add k-1 more pivots that are farthest from current pivots
		for (int i = 0; i < k-1; i++)
		{
			next = diffHeur.GetFarthestPivot();
			diffHeur.AddPivot(next);
		}
		//build the PDB heuristic and save the visualization images 
		diffHeur.BuildPDBs();
		for (int i = -1; i < k; i++)
			diffHeur.SaveAsBMP("../../resources/maps/lak303d_far/", i);

		OctileDistanceHeuristic odh;


		//load the benchmark
		std::vector<int> startx, starty, goalx, goaly;
		std::vector<double> expectedCost;
		if (!LoadBenchmark(startx, starty, goalx, goaly, expectedCost, "../../resources/benchmarks/lak303d.map.scen"))
			return 1;


		MyAStar<Map2DState, Map2DAction, Map2D, MaxHeuristic<Map2DState>>
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

			diffHeur.SetGoal(goal);
			odh.SetGoal(goal);
			MaxHeuristic<Map2DState> heur;
			heur.heurs.push_back(&diffHeur);
			heur.heurs.push_back(&odh);

			astar1 = new MyAStar<Map2DState, Map2DAction, Map2D, MaxHeuristic<Map2DState>>
				(heur);

			std::cout << "********************************\n"
				<< "test case" << i + 1 << "\n"
				<< "start: " << start
				<< "goal: " << goal << "\n";

			startTime = clock();
			if (astar1->GetPath(map, start, goal))
			{
				std::cout << "A* w/ Farthest Diff Heur & ODH found a path!\n";
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

	else if (argc > 1 && strcmp(argv[1], "-ran") == 0)
	{
		int k = 10;
		if (argc > 2)
			k = atoi(argv[2]);
		Map2D map;

		//initialize differential heuristic
		Map2DDifferentialHeuristic diffHeur(map);
		if (!diffHeur.LoadMap("../../resources/maps/lak303d.map"))
		{
			std::cout << "fail to load map: " << "../../resources/maps/lak303d.map" << "\n";
			return 1;
		}

		//choose 10 random pivot
		Map2DState s;
		std::vector<Map2DAction> actions;
		int x, y;
		srand(time(NULL));
		map.LoadMap("../../resources/maps/lak303d.map");

		//add k pivots that are randomly chosen
		for (int i = 0; i < k; i++)
		{
			actions.resize(0);
			while (actions.size() == 0)
			{
				x = rand() % map.width;
				y = rand() % map.height;
				s = Map2DState(x, y);
				map.GetActions(s, actions);
			}
			diffHeur.AddPivot(s);
		}
		//build the PDB heuristic and save the visualization images 
		diffHeur.BuildPDBs();
		for (int i = -1; i < k; i++)
			diffHeur.SaveAsBMP("../../resources/maps/lak303d_ran/", i);

		OctileDistanceHeuristic odh;


		//load the benchmark
		std::vector<int> startx, starty, goalx, goaly;
		std::vector<double> expectedCost;
		if (!LoadBenchmark(startx, starty, goalx, goaly, expectedCost, "../../resources/benchmarks/lak303d.map.scen"))
			return 1;


		MyAStar<Map2DState, Map2DAction, Map2D, MaxHeuristic<Map2DState>>
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

			diffHeur.SetGoal(goal);
			odh.SetGoal(goal);
			MaxHeuristic<Map2DState> heur;
			heur.heurs.push_back(&diffHeur);
			heur.heurs.push_back(&odh);

			astar1 = new MyAStar<Map2DState, Map2DAction, Map2D, MaxHeuristic<Map2DState>>
				(heur);

			std::cout << "********************************\n"
				<< "test case" << i + 1 << "\n"
				<< "start: " << start
				<< "goal: " << goal << "\n";

			startTime = clock();
			if (astar1->GetPath(map, start, goal))
			{
				std::cout << "A* w/ Random Diff Heur & ODH found a path!\n";
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
		std::cout << "Usage: " << argv[0] << " -oct \n"
			<< argv[0] << " -far [k] \n"
			<< argv[0] << " -ran [k] \n";
	}


	return 0;
}


bool LoadBenchmark(std::vector<int>& startx, std::vector<int>& starty, std::vector<int>& goalx, std::vector<int>& goaly,
	std::vector<double>& expectedCost, std::string fileName)
{
	std::ifstream fin;
	fin.open(fileName);
	if (!fin.is_open())
	{
		std::cout << "fail to load benchmark file: " << fileName << "\n";
		return false;
	}

	startx.resize(0);
	starty.resize(0);
	goalx.resize(0);
	goaly.resize(0);
	expectedCost.resize(0);
	std::string str;
	//get rid of "version 1"
	fin >> str;
	fin >> str;
	while (!fin.eof())
	{
		//first 4 should be "group", "mapname", "map width", "map height"
		//which are not helpful for this assignment
		fin >> str;
		fin >> str;
		fin >> str;
		fin >> str;

		if (fin.eof())
			break;

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
	return true;
}