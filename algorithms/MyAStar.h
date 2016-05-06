
#ifndef MY_ASTAR_H
#define MY_ASTAR_H

#include <vector>
#include <stack>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include "MySearchAlgorithm.h"
#include "MyBinaryHeap.h"
#include "MyInefficientAStar.h"

//this algorithm assume unit edge cost

//StateInfo is defined in MyInefficientAStar.h
//TODO use namespace
/*
struct StateInfo {
	int gcost;
	int hcost;
};
*/

struct StateInfoLess {
	bool operator()(const StateInfo& i, const StateInfo& j) {
		if (i.gcost + i.hcost < j.gcost + j.hcost)
			return true;
//		return false;
		else if (i.gcost + i.hcost > j.gcost + j.hcost)
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
	MyBinaryHeap<StateInfo, uint64_t, StateInfoLess> openList;
	std::unordered_set<uint64_t> closedList;
	int solutionCost;
};


template <typename state, typename action, typename environment, typename heuristic>
bool MyAStar<state, action, environment, heuristic>::GetPath(environment& e, state& start, state& goal)
{
	if (start == goal)
	{
		solutionCost = 0;
		nodesExpanded = 1;
		return true;
	}
		
	StateInfo info;
	info.gcost = 0;
	info.hcost = heur.GetHCost(start);
	uint64_t startRank;
	e.GetRankFromState(start, startRank);
	openList.Insert(info, startRank);
	//openList.
	uint64_t next;
	//typename std::map<state, StateInfo>::iterator it;
	while (!openList.Empty())
	{
		//choose best node from openlist;
		next = openList.ExtractMin();
		info = openList.MinKey();
		//remove it from open, add it to closed
		openList.DeleteMin();
		closedList.insert(next);

		nodesExpanded++;
		//we can do solution detection here, as DSD

		//state s;
		state successor;
		e.GetStateFromRank(successor, next);
		//std::cout << "\n***********exbanding*****************\nstate: " << successor;
		//std::cout << "rank" << next << "\n";
		//std::cout << "info: " << info<<"\n";
		std::vector<action> actions;
		e.GetActions(successor, actions);
		for (int i = 0; i < actions.size(); i++)
		{
			//state successor(s);
			e.ApplyAction(successor, actions[i]);
			//we can do solution here, as ISD
			if (successor == goal)
			{
				solutionCost = info.gcost + 1;
				return true;
			}
			//std::cout << "\nsucc: " << successor << "\n";
			StateInfo succinfo;
			succinfo.gcost = info.gcost + 1;
			//succinfo.hcost = heur.GetHCost(successor);
			uint64_t succrank;
			e.GetRankFromState(successor, succrank);
			//std::cout << "succ: " << successor;
			//std::cout << "succinfo " << succinfo << "\n";
			//std::cout << "succrank: " << succrank<<"\n";
			if (!openList.IsExist(succrank))
			{
				//this node is ungenerated
				if (closedList.find(succrank) == closedList.end())
				{
					succinfo.hcost = heur.GetHCost(successor);
					openList.Insert(succinfo, succrank);
				}
				//otherwise this node is expanded. as for consistent heuristic, we can ignore it
			}
			//in this case, this state is already on open. We may need to update its gcost 
			else
			{
				StateInfo preinfo = openList.GetKey(succrank);
				if (preinfo.gcost > succinfo.gcost)
				{
					succinfo.hcost = preinfo.hcost;
					openList.DecreaseKey(succinfo, succrank);
				}
			}
			
			e.UndoAction(successor, actions[i]);
		}
	}


	return false;
}

#endif // ! MY_ASTAR_H
