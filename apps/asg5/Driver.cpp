
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
		heur.SaveAsBMP("../../resources/maps/lak303d.bmp",0);
	}

	else
	{
		std::cout << "Usage: " << argv[0] << " -i [low] [high] \n"
			<< argv[0] << " -a <heuristic directory> [low] [high] \n"
			<< argv[0] << " -amap <start x> <start y> <goal x> <goal y> \n";
	}


	return 0;
}
