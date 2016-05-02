
#ifndef MY_INEFFICIENT_ASTAR_H
#define MY_INEFFICIENT_ASTAR_H

#include <vector>
#include <stack>
#include <map>
#include <unordered_set>
#include "MySearchAlgorithm.h"

#define MINUMUM_F 10000

struct StateInfo {
	int gcost;
	int hcost;
};

template <typename state, typename action, typename environment, typename heuristic>
class MyInefficientAStar :public MySearchAlgorithm<state, action, environment>
{
public:
	MyInefficientAStar(heuristic& h)
		:MySearchAlgorithm<state, action, environment>(), nodesExpanded(0), heur(h)
	{
	}
	virtual bool GetPath(environment& e, state& start, state& goal);
	virtual uint64_t GetNodesExpanded() { return nodesExpanded; }
	std::vector<action> GetActionSequence();
protected:
	uint64_t nodesExpanded;
	heuristic heur;
	std::map<state,StateInfo> openList;
	std::map<state,StateInfo> closedList;
};


template <typename state, typename action, typename environment, typename heuristic>
bool MyInefficientAStar<state, action, environment, heuristic>::GetPath(environment& e, state& start, state& goal)
{
	StateInfo info;
	info.gcost = 0;
	info.hcost = heur.GetHCost(start);
	openList[start] = info;
	state next;
	while (!openList.empty())
	{
		//choose best node from openlist;
		int minF = MINUMUM_F;
		std::map<state, StateInfo>::iterator it;
		for (it = openList.begin(); it != openList.end();it++)
		{
			if (it->second.gcost + it->second.hcost < minF)
			{
				minF = it->second.gcost + it->second.hcost;
				next = it->first;
				info = it->second;
			}
		}
		//remove it from open, add it to closed
		it = openList.find(next);
		closedList[it->first] = it->second;
		openList.erase(openList.find(next));

		nodeExpanded++;
		//we can do solution detection here, as DSD

		std::vector<action> actions;
		e.GetActions(start, actions);
		for (int i = 0; i < actions.size(); i++)
		{
			e.ApplyAction(next, actions[i]);
			//we can do solution here, as ISD
			if (next == goal)
				return true;

			//this state is ungenerated (or on closed, if inconsistent heuristic)
			//in this case, we need add it to open
			if (openList.find(next) == openList.end())
			{
				StateInfo nextInfo;
				nextInfo.hcost = heur.GetHCost(next);
				//assume unit edge cost for now
				nextInfo.gcost = info.gcost + 1;
				openList[next] = nextInfo;
			}
			//in this case, this state is already on open. We may need to update its gcost 
			else
			{
				if (openList[next].gcost > info.gcost + 1)
					openList[next].gcost = info.gcost+1;
			}

			e.UndoAction(next, actions[i]);
		}
	}


	return false;
}

#endif // ! MY_INEFFICIENT_ASTAR_H
