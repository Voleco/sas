
#ifndef MY_ASTAR_H
#define MY_ASTAR_H

#include <vector>
#include <stack>
#include <queue>
#include "MySearchAlgorithm.h"

#define MINUMUM_F 10000

//this algorithm assume unit edge cost

template<typename state>
struct StateWithInfo {
	state s;
	int gcost;
	int hcost;
};

template<typename state>
struct comparator {
	bool operator()(const StateWithInfo& i, const StateWithInfo& j) {
		if (i.gcost + i.hcost > j.gcost + j.hcost)
			return true;
		else if (i.gcost + i.hcost < j.gcost + j.hcost)
			return false;
		else//i.gcost + i.hcost == j.gcost + j.hcost
		{
			return i.gcost > j.gcost;
		}
	}
};


template <typename state, typename action, typename environment, typename heuristic>
class MyAStar :public MySearchAlgorithm<state, action, environment>
{
public:
	MyAStar(heuristic& h)
		:MySearchAlgorithm<state, action, environment>(), nodesExpanded(0), heur(h), solutionCost(MINUMUM_F)
	{
	}
	virtual bool GetPath(environment& e, state& start, state& goal);
	virtual uint64_t GetNodesExpanded() { return nodesExpanded; }
	int GetSolutionCost() { return solutionCost; }
	//std::vector<action> GetActionSequence();
protected:
	uint64_t nodesExpanded;
	heuristic heur;
	//openList is acctually a min heap
	std::priority_queue<StateWithInfo<state>, std::vector<StateWithInfo<state>>, comparator> openList;
	std::vector<StateWithInfo<state>> closedList;
	int solutionCost;
};


template <typename state, typename action, typename environment, typename heuristic>
bool MyAStar<state, action, environment, heuristic>::GetPath(environment& e, state& start, state& goal)
{
	StateWithInfo<state> info;
	info.gcost = 0;
	info.hcost = heur.GetHCost(start);
	info.s = start;
	openList.push(info);
	//openList.
	StateWithInfo<state> next;
	//typename std::map<state, StateInfo>::iterator it;
	while (!openList.empty())
	{
		//choose best node from openlist;
		next = openList.top();
		//remove it from open, add it to closed
		openList.pop();
		closedList.insert(next);

		nodesExpanded++;
		//we can do solution detection here, as DSD

		std::vector<action> actions;
		e.GetActions(next.s, actions);
		for (int i = 0; i < actions.size(); i++)
		{
			e.ApplyAction(next.s, actions[i]);
			//we can do solution here, as ISD
			if (next.s == goal)
			{
				solutionCost = info.gcost + 1;
				return true;
			}


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

#endif // ! MY_ASTAR_H
