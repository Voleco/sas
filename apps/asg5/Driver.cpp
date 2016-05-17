
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
		heur.LoadMap("../../resources/maps/orz301d.map");
		heur.SaveAsBMP("../../resources/maps/orz301d.bmp");
	}


	else
	{
		std::cout << "Usage: " << argv[0] << " -i [low] [high] \n"
			<< argv[0] << " -a <heuristic directory> [low] [high] \n"
			<< argv[0] << " -amap <start x> <start y> <goal x> <goal y> \n";
	}


	return 0;
}
