
#include <iostream>
#include <cstring>
#include "GridBasedMap.h"
#include "NAryTree.h"
#include "MyBFS.h"
#include "MyDFS.h"

int main(int argc, char** argv)
{
	std::cout << "*******grid-based map*******\n";
	std::cout << "i\t bfs\t bfswdd\t dfid\t dfidwdd\t\n";
	for (int i = 0; i < 10; i++)
	{
		GridBasedMap map(i, i);
		GridBasedMapState start(0, 0);
		GridBasedMapState goal(i, i);

		MyBFS<GridBasedMapState, GridBasedMapAction, GridBasedMap> bfs(map, start, goal, false);
		bfs.GetPath(map, start, goal);


		MyBFS<GridBasedMapState, GridBasedMapAction, GridBasedMap> bfs2(map, start, goal, true);
		bfs2.GetPath(map, start, goal);

		MyDFID<GridBasedMapState, GridBasedMapAction, GridBasedMap> dfid(map, start, goal, false);
		dfid.GetPath(map, start, goal);

		MyDFID<GridBasedMapState, GridBasedMapAction, GridBasedMap> dfid2(map, start, goal, true);
		dfid2.GetPath(map, start, goal);

		std::cout << i << "\t" << bfs.GetNodesExpanded() << "\t"
			<< bfs2.GetNodesExpanded() << "\t"
			<< dfid.GetNodesExpanded() << "\t"
			<< dfid2.GetNodesExpanded() << "\n";
	}

	std::cout << "\n*******n-ary tree  ********\n";
	std::cout << "depth: 10\n";
	std::cout << "braching factor\t bfs\t bfswdd\t dfid\t dfidwdd\t\n";
	for (int i = 2; i < 6; i++)
	{
		NAryTree tree(i, 10);
		NAryTreeState start(0);
		uint64_t tmp = i;
		for (int j = 0; j < 10; j++)
			tmp = tmp* i;
		tmp = (tmp - 1) / (i - 1);
		NAryTreeState goal(tmp);

		MyBFS<NAryTreeState, NAryTreeAction, NAryTree> bfs(tree, start, goal, false);
		bfs.GetPath(tree, start, goal);

		MyBFS<NAryTreeState, NAryTreeAction, NAryTree> bfs2(tree, start, goal, true);
		bfs2.GetPath(tree, start, goal);

		MyDFID<NAryTreeState, NAryTreeAction, NAryTree> dfid(tree, start, goal, false);
		dfid.GetPath(tree, start, goal);

		MyDFID<NAryTreeState, NAryTreeAction, NAryTree> dfid2(tree, start, goal, true);
		dfid2.GetPath(tree, start, goal);
		
		std::cout << i << "\t" << bfs.GetNodesExpanded() << "\t"
			<< bfs2.GetNodesExpanded() << "\t"
			<< dfid.GetNodesExpanded() << "\t"
			<< dfid2.GetNodesExpanded() << "\n";
	}

	return 0;
}