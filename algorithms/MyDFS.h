
#ifndef MY_DFS_H
#define MY_DFS_H

#include <stack>
#include <unordered_set>
#include "MySearchAlgorithm.h"

template <typename state, typename action, typename environment>
class MyDFS :public MySearchAlgorithm<state, action, environment>
{
public:
	MyDFS(unsigned int md,bool dd=false)
		:MySearchAlgorithm<state, action, environment>(),nodesExpanded(0),maxDepthAllowed(md),duplicateDetection(dd)
	{
	}
	virtual bool GetPath(environment& e, state& start, state& goal);
	virtual uint64_t GetNodesExpanded() { return nodesExpanded; }
protected:
	uint64_t nodesExpanded;
	unsigned int maxDepthAllowed;
	bool duplicateDetection;
};


template <typename state, typename action, typename environment>
class MyDFID
{
public:
	MyDFID(bool dd = false,int _maxDepth=0) :totalNodesExpanded(0),maxDepth(_maxDepth),duplicateDetection(dd){}
	bool GetPath(environment& e, state& start, state& goal);
	uint64_t GetNodesExpanded() { return totalNodesExpanded; }
private:
	uint64_t totalNodesExpanded;
	unsigned int maxDepth;
	bool duplicateDetection;
};

template <typename state, typename action, typename environment>
bool MyDFS<state, action, environment>::GetPath(environment& e, state& start, state& goal)
{
	bool found = false;

	std::stack<state> openStack;
	std::unordered_set<state> openList;
	std::unordered_set<state> closedList;

	openStack.push(start);
	if(duplicateDetection)
		openList.insert(start);
	nodesExpanded = 0;
	std::vector<action> actions;
	state next;
	while (!openStack.empty())
	{
		next = openStack.top();
		openStack.pop();
		if (duplicateDetection)
			openList.erase(openList.find(next));
		nodesExpanded++;
		//std::cout << "next: "<<next<<" \n";
		//we do solution detection here, as delayed solution detection
		if (next == goal)
		{
			found = true;
			break;
		}

		if (e.GetDepth(next) >= maxDepthAllowed)
			continue;

		e.GetActions(next, actions);
		for (unsigned int i = 0; i < actions.size(); i++)
		{
			e.ApplyAction(next, actions[i]);
			//we can do solution dectection here, as immediate solution detection
			
			if (duplicateDetection)
			{
				if (closedList.find(next) == closedList.end() && openList.find(next) == openList.end())
				{
					openStack.push(next);
					openList.insert(next);
				}
			}
			else
				openStack.push(next);

			//openStack.push(next);
			e.UndoAction(next, actions[i]);
		}
		//put the node to closed
		if (duplicateDetection)
			closedList.insert(next);
	}

	return found;
}


template <typename state, typename action, typename environment>
bool MyDFID<state, action, environment>::GetPath(environment& e, state& start, state& goal)
{
	if(maxDepth==0)
		maxDepth = 1 + e.GetSolutionDepthUpperBound(start, goal);
	MyDFS<state, action, environment>* dfs;
	bool pathFound = false;
	for (unsigned int i = 0; i < maxDepth; i++)
	{
		dfs = new MyDFS<state, action, environment>(i,duplicateDetection);
		pathFound = dfs->GetPath(e,start,goal);
		//std::cout << "nodes expanded at iteration "<<i<<": " << dfs->GetNodesExpanded()<< "\n";
		totalNodesExpanded += dfs->GetNodesExpanded();
		delete dfs;
		if (pathFound)
			break;
	}
	return pathFound;
}
#endif // !MY_BFS_H
