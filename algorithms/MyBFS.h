
#ifndef MY_BFS_H
#define MY_BFS_H

#include <queue>
#include <unordered_set>
#include <set>
#include "MySearchAlgorithm.h"

template <typename state, typename action, typename environment>
class MyBFS :public MySearchAlgorithm<state, action, environment>
{
public:
	MyBFS(bool dd=false)
		:MySearchAlgorithm<state, action, environment>(),nodesExpanded(0),duplicateDetection(dd)
	{
	}
	virtual bool GetPath(environment& e, state& start, state& goal);
	virtual uint64_t GetNodesExpanded() { return nodesExpanded; }
protected:
	uint64_t nodesExpanded;
	bool duplicateDetection;
};

template <typename state, typename action, typename environment>
bool MyBFS<state, action, environment>::GetPath(environment& e, state& start, state& goal)
{
	//std::cout << "start: " << start << "\n"<< "goal: " << goal << "\n";
	bool found = false;

	std::queue<state> openQueue;

	std::unordered_set<state> openList;
	std::unordered_set<state> closedList;

	openQueue.push(start);
	if(duplicateDetection)
		openList.insert(start);
	nodesExpanded = 0;
	std::vector<action> actions;
	state next;
	while (!openQueue.empty()) 
	{
		//take a node from open
		next = openQueue.front();
		openQueue.pop();
		if (duplicateDetection)
			openList.erase(openList.find(next));
		nodesExpanded++;
		//std::cout << "next: " << next << "\n";
		//we do solution detection here, as delayed solution detection
		if (next == goal)
		{
			found = true;
			break;
		}
		e.GetActions(next, actions);
		//generate its successors
		for (unsigned int i = 0; i < actions.size();i++)
		{
			e.ApplyAction(next, actions[i]);
			//we can do solution dectection here, as immediate solution detection

			if (duplicateDetection) 
			{
				if (closedList.find(next) == closedList.end() && openList.find(next) == openList.end())
				{
					openQueue.push(next);
					openList.insert(next);
				}
			}
			else
				openQueue.push(next);

			//TODO duplicate detection might needed here
			//openQueue.push(next);
			e.UndoAction(next, actions[i]);
		}
		//put the node to closed
		if (duplicateDetection)
			closedList.insert(next);
	}

	return found;
}


#endif // !MY_BFS_H
