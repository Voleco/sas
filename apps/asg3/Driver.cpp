
#include <iostream>
#include <vector>
#include <time.h>
#include <cstring>
#include "MyIDAStar.h"
#include "SlidingTilePuzzle.h"
#include "NAryTree.h"
#include "GridBasedMap.h"

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

		std::vector<std::vector<int>> patterns;
		patterns.push_back(std::vector<int>({ 0,1,4,5 }));
		patterns.push_back(std::vector<int>({ 0,2,3,6,7 }));
		patterns.push_back(std::vector<int>({ 0,8,9,12,13 }));
		patterns.push_back(std::vector<int>({ 0,10,11,14,15 }));
		patterns.push_back(std::vector<int>({ 0,1,2,3,4,5,6,7 }));
		SlidingTilePuzzlePDB* pdb = new SlidingTilePuzzlePDB(puzzle, goal, patterns[atoi(argv[3])]);

		uint64_t rank;
		std::cout << "origin state: " << start<<"\n";
		pdb->GetPDBRankFromState(start, rank);
		std::cout << "rank: " << rank << "\n";
		pdb->GetStateFromPDBRank(start, rank);
		std::cout << "retrieved state: " << start << "\n";
	}

	else if (argc > 2 && strcmp(argv[1], "-buildPDB") == 0)
	{
		uint64_t rank;
		SlidingTilePuzzleState goal(4, 4);
		goal.Reset();

		SlidingTilePuzzle* puzzle;
		std::vector<int>* pattern;

		std::cout << "building PDB(a): 0 1 4 5\n";
		puzzle = new SlidingTilePuzzle(4, 4);
		pattern = new std::vector<int>({ 0,1,4,5 });
		SlidingTilePuzzlePDB* pdbA = new SlidingTilePuzzlePDB(*puzzle, goal, *pattern);
		if (!pdbA->Load(argv[2]))
		{
			pdbA->BuildPDB();
			pdbA->Save(argv[2]);
		}
		delete pattern;
		delete puzzle;
		std::cout << "building PDB(b): 0 2 3 6 7\n";
		puzzle = new SlidingTilePuzzle(4, 4);
		pattern = new std::vector<int>({ 0,2,3,6,7 });
		SlidingTilePuzzlePDB* pdbB = new SlidingTilePuzzlePDB(*puzzle, goal, *pattern);
		if (!pdbB->Load(argv[2]))
		{
			pdbB->BuildPDB();
			pdbB->Save(argv[2]);
		}
		delete pattern;
		delete puzzle;
		std::cout << "building PDB(c): 0 8 9 12 13\n";
		puzzle = new SlidingTilePuzzle(4, 4);
		pattern = new std::vector<int>({ 0,8,9,12,13 });
		SlidingTilePuzzlePDB* pdbC = new SlidingTilePuzzlePDB(*puzzle, goal, *pattern);
		if (!pdbC->Load(argv[2]))
		{
			pdbC->BuildPDB();
			pdbC->Save(argv[2]);
		}
		delete pattern;
		delete puzzle;
		std::cout << "building PDB(d): 0 10 11 14 15\n";
		puzzle = new SlidingTilePuzzle(4, 4);
		pattern = new std::vector<int>({ 0,10,11,14,15 });
		SlidingTilePuzzlePDB* pdbD = new SlidingTilePuzzlePDB(*puzzle, goal, *pattern);
		if (!pdbD->Load(argv[2]))
		{
			pdbD->BuildPDB();
			pdbD->Save(argv[2]);
		}
		delete pattern;
		delete puzzle;
		std::cout << "building PDB(e): 0 1 2 3 4 5 6 7\n";
		puzzle = new SlidingTilePuzzle(4, 4);
		pattern = new std::vector<int>({ 0,1,2,3,4,5,6,7 });
		SlidingTilePuzzlePDB* pdbE = new SlidingTilePuzzlePDB(*puzzle, goal, *pattern);
		if (!pdbE->Load(argv[2]))
		{
			pdbE->BuildPDB();
			pdbE->Save(argv[2]);
		}
		delete pattern;
		delete puzzle;

	}

	else if (argc > 2 && strcmp(argv[1], "-run") == 0)
	{
		bool showPath = false;
		int low = 1;
		int high = 100;
		if (argc > 3)
		{
			low = atoi(argv[3]);
			high = atoi(argv[4]);
		}
		if (argc > 5 && strcmp(argv[5], "-showpath") == 0)
			showPath = true;

		SlidingTilePuzzle puzzle(4,4);
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

		MyIDAStar<SlidingTilePuzzleState, SlidingTilePuzzleAction, SlidingTilePuzzle, ManhattanDistanceHeuristic>
			*ida1;
		MyIDAStar<SlidingTilePuzzleState, SlidingTilePuzzleAction, SlidingTilePuzzle, MaxHeuristic<SlidingTilePuzzleState>>
			*ida2;
		MyIDAStar<SlidingTilePuzzleState, SlidingTilePuzzleAction, SlidingTilePuzzle, MaxHeuristic<SlidingTilePuzzleState>>
			*ida3;

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

			ida1 = new MyIDAStar<SlidingTilePuzzleState, SlidingTilePuzzleAction, SlidingTilePuzzle, ManhattanDistanceHeuristic>
				(*mh, 500);
			ida2 = new MyIDAStar<SlidingTilePuzzleState, SlidingTilePuzzleAction, SlidingTilePuzzle, MaxHeuristic<SlidingTilePuzzleState>>
				(maxH2, 500);
			ida3 = new MyIDAStar<SlidingTilePuzzleState, SlidingTilePuzzleAction, SlidingTilePuzzle, MaxHeuristic<SlidingTilePuzzleState>>
				(maxH3, 500);

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
			if (ida1->GetPath(puzzle, startcp, goalcp))
			{
				std::cout << "IDA* w/ only MD found a path!\n";
				std::cout << "nodes expanded:\tPath length:\ttime spent(s)\n";
				std::cout<< ida1->GetNodesExpanded() << "\t";
				acs = ida1->GetActionSequence();
				std::cout << acs.size() << "\t";
			}	
			delete ida1;
			endTime = clock();
			clockTicksTaken = endTime - startTime;
			timeInSeconds = clockTicksTaken / (double)CLOCKS_PER_SEC;
			std::cout << timeInSeconds << "\n";
			if (showPath)
			{
				std::cout << "Path: ";
				for (int i = 0; i < acs.size(); i++)
					std::cout << acs[i] << " ";
				std::cout << "\n";
			}

			startTime = clock();
			acs.resize(0);
			puzzle = SlidingTilePuzzle(4, 4);
			startcp = SlidingTilePuzzleState(start);
			goalcp = SlidingTilePuzzleState(goal);
			if (ida2->GetPath(puzzle, startcp, goalcp))
			{
				std::cout << "IDA* w/ max{MD, PDBa-d} found a path!\n";
				std::cout << "nodes expanded:\tPath length:\ttime spent(s)\n";
				std::cout << ida2->GetNodesExpanded() << "\t";
				acs = ida2->GetActionSequence();
				std::cout << acs.size() << "\t";
			}
			delete ida2;
			endTime = clock();
			clockTicksTaken = endTime - startTime;
			timeInSeconds = clockTicksTaken / (double)CLOCKS_PER_SEC;
			std::cout << timeInSeconds << "\n";
			if (showPath)
			{
				std::cout << "Path: ";
				for (int i = 0; i < acs.size(); i++)
					std::cout << acs[i] << " ";
				std::cout << "\n";
			}

			startTime = clock();
			acs.resize(0);
			puzzle = SlidingTilePuzzle(4, 4);
			startcp = SlidingTilePuzzleState(start);
			goalcp = SlidingTilePuzzleState(goal);
			if (ida3->GetPath(puzzle, startcp, goalcp))
			{
				std::cout << "IDA* w/  max{MD, PDBc-e} found a path!\n";
				std::cout << "nodes expanded:\tPath length:\ttime spent(s)\n";
				std::cout << ida3->GetNodesExpanded() << "\t";
				acs = ida3->GetActionSequence();
				std::cout << acs.size() << "\t";
			}
			delete ida3;
			endTime = clock();
			clockTicksTaken = endTime - startTime;
			timeInSeconds = clockTicksTaken / (double)CLOCKS_PER_SEC;
			std::cout << timeInSeconds << "\n";
			if (showPath)
			{
				std::cout << "Path: ";
				for (int i = 0; i < acs.size(); i++)
					std::cout << acs[i] << " ";
				std::cout << "\n";
			}
		}
	}

	else
	{
		std::cout << "Usage: " << argv[0] << " -buildPDB <heuristic directory>\n"
			<< argv[0] << " -run <heuristic directory> [low] [high] [-showpath]\n";
	}


	return 0;
}
