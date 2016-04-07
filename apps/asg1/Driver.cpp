
#include <iostream>
#include <cstring>
#include "GridBasedMap.h"
#include "NAryTree.h"
#include "MyBFS.h"
#include "MyDFS.h"

int main(int argc, char** argv)
{
	if (argc >=6 && strcmp(argv[1], "-gridmap") == 0)
	{
		GridBasedMap map(atoi(argv[2]), atoi(argv[3]));
		GridBasedMapState start(0, 0);
		GridBasedMapState goal(atoi(argv[4]), atoi(argv[5]));

		std::cout << "start state: " << start << "\n" << "goal state: " << goal << "\n";
		MyBFS<GridBasedMapState, GridBasedMapAction, GridBasedMap> bfs(map, start, goal,false);
		if (bfs.GetPath(map, start, goal))
			std::cout << "\nBFS(without duplicate detection) found a path!\n";
		else
			std::cout << "\nBFS(without duplicate detection) did not find a path.\n";
		std::cout << "nodes expanded: " << bfs.GetNodesExpanded() << "\n";

		MyBFS<GridBasedMapState, GridBasedMapAction, GridBasedMap> bfs2(map, start, goal,true);
		if (bfs2.GetPath(map, start, goal))
			std::cout << "\nBFS(with duplicate detection) found a path!\n";
		else
			std::cout << "\nBFS(with duplicate detection) did not find a path.\n";
		std::cout << "nodes expanded: " << bfs2.GetNodesExpanded() << "\n";

		MyDFID<GridBasedMapState, GridBasedMapAction, GridBasedMap> dfid(map, start, goal,false);
		if (dfid.GetPath(map, start, goal))
			std::cout << "\nDFID(without duplicate detection) found a path!\n";
		else
			std::cout << "\nDFID(without duplicate detection) did not find a path.\n";
		std::cout << "nodes expanded: " << dfid.GetNodesExpanded() << "\n";

		MyDFID<GridBasedMapState, GridBasedMapAction, GridBasedMap> dfid2(map, start, goal,true);
		if (dfid2.GetPath(map, start, goal))
			std::cout << "\nDFID(with duplicate detection) found a path!\n";
		else
			std::cout << "\nDFID(with duplicate detection) did not find a path.\n";
		std::cout << "nodes expanded: " << dfid2.GetNodesExpanded() << "\n";
	}
	else if (argc >= 5 && strcmp(argv[1], "-narytree") == 0)
	{
		NAryTree tree(atoi(argv[2]), atoi(argv[3]));
		NAryTreeState start = 0;
		NAryTreeState goal = atoi(argv[4]);

		std::cout << "start state: " << start << "\n" << "goal state: " << goal << "\n";
		MyBFS<NAryTreeState, NAryTreeAction, NAryTree> bfs(tree, start, goal, false);
		if (bfs.GetPath(tree, start, goal))
			std::cout << "\nBFS(without duplicate detection) found a path!\n";
		else
			std::cout << "\nBFS(without duplicate detection) did not find a path.\n";
		std::cout << "nodes expanded: " << bfs.GetNodesExpanded() << "\n";

		MyBFS<NAryTreeState, NAryTreeAction, NAryTree> bfs2(tree, start, goal, true);
		if (bfs2.GetPath(tree, start, goal))
			std::cout << "\nBFS(with duplicate detection) found a path!\n";
		else
			std::cout << "\nBFS(with duplicate detection) did not find a path.\n";
		std::cout << "nodes expanded: " << bfs2.GetNodesExpanded() << "\n";

		MyDFID<NAryTreeState, NAryTreeAction, NAryTree> dfid(tree, start, goal,false);
		if (dfid.GetPath(tree, start, goal))
			std::cout << "\nDFID(without duplicate detection) found a path!\n";
		else
			std::cout << "\nDFID(without duplicate detection) did not find a path.\n";
		std::cout << "nodes expanded: " << dfid.GetNodesExpanded() << "\n";

		MyDFID<NAryTreeState, NAryTreeAction, NAryTree> dfid2(tree, start, goal,true);
		if (dfid2.GetPath(tree, start, goal))
			std::cout << "\nDFID(with duplicate detection) found a path!\n";
		else
			std::cout << "\nDFID(with duplicate detection) did not find a path.\n";
		std::cout << "nodes expanded: " << dfid2.GetNodesExpanded() << "\n";
	}
	else
	{
		std::cout << "usage: \n" << argv[0] << " -gridmap <map size X> <map size Y> <goal X coordinate> <goal Y coordinate>\n"
			<< argv[0] << " -narytree <branching factor> <depth> <goal index>\n";
	}

	return 0;
}