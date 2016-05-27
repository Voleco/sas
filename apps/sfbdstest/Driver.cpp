
#include <iostream>
#include <vector>
#include <time.h>
#include <cstring>
#include "MyIDAStar.h"
#include "MyAStar.h"
#include "SlidingTilePuzzle.h"
#include "Map2D.h"
#include "MySFBDS.h"


bool LoadBenchmark(std::vector<int>& startx, std::vector<int>& starty, std::vector<int>& goalx, std::vector<int>& goaly,
	std::vector<double>& expectedCost, std::string fileName);

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

		SlidingTilePuzzle puzzle(4, 4);
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

	else if (argc > 1 && strcmp(argv[1], "-bf") == 0)
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

		SlidingTilePuzzle puzzle(4, 4);
		SlidingTilePuzzleState start(4, 4);
		SlidingTilePuzzleState goal(4, 4);
		ManhattanDistanceHeuristic heur;
		//heur.SetGoal(goal);


		MySFBDS<SlidingTilePuzzleState, SlidingTilePuzzleAction, SlidingTilePuzzle, ManhattanDistanceHeuristic>
			*sfbds;

		clock_t startTime;
		clock_t endTime;
		clock_t clockTicksTaken;
		double timeInSeconds;
		//int i = atoi(argv[1]);
		for (int i = low - 1; i < high; i++)
		{
			startTime = clock();
			GetSildingTileInstance(i, start);
			goal.Reset();
			sfbds = new MySFBDS<SlidingTilePuzzleState, SlidingTilePuzzleAction, SlidingTilePuzzle, ManhattanDistanceHeuristic>(heur,heur,BF, 500.0);
			std::cout << "********************************\n"
				<< "Puzzle " << i + 1 << " of 100\n"
				<< "start: " << start
				<< "goal: " << goal << "\n";
			std::vector<SlidingTilePuzzleAction> acs;
			puzzle = SlidingTilePuzzle(4, 4);
			if (sfbds->GetPath(puzzle, start, goal))
			{
				std::cout << "SFBDS with BF Jumping Policy found a path!\n";
				acs = sfbds->GetActionSequence();
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
				std::cout << "SFBDS with BF Jumping Policy did not find a path.\n";
			std::cout << "nodes expanded: " << sfbds->GetNodesExpanded() << "\n";
			std::cout << "number of jumps: " << sfbds->GetNumOfJumps() << "\n";
			delete sfbds;

			endTime = clock();
			clockTicksTaken = endTime - startTime;
			timeInSeconds = clockTicksTaken / (double)CLOCKS_PER_SEC;
			std::cout << "time spent: " << timeInSeconds << " seconds\n";
		}

	}
	
	else if (argc > 1 && strcmp(argv[1], "-jil1") == 0)
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

		SlidingTilePuzzle puzzle(4, 4);
		SlidingTilePuzzleState start(4, 4);
		SlidingTilePuzzleState goal(4, 4);
		ManhattanDistanceHeuristic heur;
		//heur.SetGoal(goal);


		MySFBDS<SlidingTilePuzzleState, SlidingTilePuzzleAction, SlidingTilePuzzle, ManhattanDistanceHeuristic>
			*sfbds;

		clock_t startTime;
		clock_t endTime;
		clock_t clockTicksTaken;
		double timeInSeconds;
		//int i = atoi(argv[1]);
		for (int i = low - 1; i < high; i++)
		{
			startTime = clock();
			GetSildingTileInstance(i, start);
			goal.Reset();
			sfbds = new MySFBDS<SlidingTilePuzzleState, SlidingTilePuzzleAction, SlidingTilePuzzle, ManhattanDistanceHeuristic>(heur, heur, JIL1, 500.0);
			std::cout << "********************************\n"
				<< "Puzzle " << i + 1 << " of 100\n"
				<< "start: " << start
				<< "goal: " << goal << "\n";
			std::vector<SlidingTilePuzzleAction> acs;
			puzzle = SlidingTilePuzzle(4, 4);
			if (sfbds->GetPath(puzzle, start, goal))
			{
				std::cout << "SFBDS with JIL1 Jumping Policy found a path!\n";
				acs = sfbds->GetActionSequence();
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
				std::cout << "SFBDS with JIL1 Jumping Policy did not find a path.\n";
			std::cout << "nodes expanded: " << sfbds->GetNodesExpanded() << "\n";
			std::cout << "number of jumps: " << sfbds->GetNumOfJumps() << "\n";
			delete sfbds;

			endTime = clock();
			clockTicksTaken = endTime - startTime;
			timeInSeconds = clockTicksTaken / (double)CLOCKS_PER_SEC;
			std::cout << "time spent: " << timeInSeconds << " seconds\n";
		}

	}


	else if (argc > 1 && strcmp(argv[1], "-oct") == 0)
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


		MyIDAStar<Map2DState, Map2DAction, Map2D, MaxHeuristic<Map2DState>>
			*ida;

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

			ida = new MyIDAStar<Map2DState, Map2DAction, Map2D, MaxHeuristic<Map2DState>>
				(heur,100000);

			std::cout << "********************************\n"
				<< "test case" << i + 1 << "\n"
				<< "start: " << start
				<< "goal: " << goal << "\n";

			startTime = clock();
			if (ida->GetPath(map, start, goal))
			{
				std::cout << "A* w/ Farthest Diff Heur & ODH found a path!\n";
				std::cout << "nodes expanded:\tPath length:\ttime spent(s)\n";
				std::cout << ida->GetNodesExpanded() << "\t";
				solutionCost = ida->GetSolutionCost();
				std::cout << solutionCost << "\t";
			}

			delete ida;

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
			<< argv[0] << " -ran [k] \n"
			<< argv[0] << " -stp \n"
			<< argv[0] << " -bf \n"
			<< argv[0] << " -jil1 \n";
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