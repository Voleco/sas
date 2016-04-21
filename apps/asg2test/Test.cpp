
#include <iostream>
#include <cstring>
#include "GridBasedMap.h"
#include "NAryTree.h"
#include "MyBFS.h"
#include "MyDFS.h"
#include "MyIDAStar.h"

int main()
{
	std::cout << "*******IDA*test*******\n";
	std::cout << "*******grid-based map*******\n";
	std::cout << "i\t IDA*\t BFSWDD\t DFIDWDD\t\n";
	for (int i = 0; i < 10; i++)
	{
		GridBasedMap map(i, i);
		GridBasedMapState start(0, 0);
		GridBasedMapState goal(i, i);

		GridBasedMapHeuristic heur;
		heur.SetGoal(goal);

		MyBFS<GridBasedMapState, GridBasedMapAction, GridBasedMap> bfs( true);
		bfs.GetPath(map, start, goal);

		MyDFID<GridBasedMapState, GridBasedMapAction, GridBasedMap> dfid( true);
		dfid.GetPath(map, start, goal);

		MyIDAStar<GridBasedMapState, GridBasedMapAction, GridBasedMap, GridBasedMapHeuristic>
			ida( heur, 1 + map.GetSolutionDepthUpperBound(start, goal));
		ida.GetPath(map, start, goal);

		std::cout << i << "\t" << ida.GetNodesExpanded()<<"\t"
			<<bfs.GetNodesExpanded() << "\t"
			<< dfid.GetNodesExpanded() << "\n";
	}

	std::cout << "\n*******n-ary tree  ********\n";
	std::cout << "depth: 10\n";
	std::cout << "braching factor\t ida\t bfs\t \n";
	for (int i = 2; i < 7; i++)
	{
		NAryTree tree(i, 10);
		NAryTreeState start(0);
		uint64_t tmp = i;
		for (int j = 0; j < 10; j++)
			tmp = tmp* i;
		tmp = (tmp - 1) / (i - 1);
		NAryTreeState goal(tmp-1);

		NAryTreeHeuristic heur;
		heur.SetGoal(goal);

		MyBFS<NAryTreeState, NAryTreeAction, NAryTree> bfs( false);
		bfs.GetPath(tree, start, goal);

		MyIDAStar<NAryTreeState, NAryTreeAction, NAryTree, NAryTreeHeuristic>
			ida( heur, 1 + tree.GetSolutionDepthUpperBound(start,goal));
		ida.GetPath(tree, start, goal);

		std::cout << i << "\t" << ida.GetNodesExpanded() << "\t"
			<< bfs.GetNodesExpanded() << "\n";
	}

	return 0;
}