
#ifndef MY_IDASTAR_H
#define MY_IDASTAR_H

#include <vector>
#include <stack>
#include "MySearchAlgorithm.h"



template <typename state, typename action, typename environment, typename heuristic>
class MyIDAStar :public MySearchAlgorithm<state, action, environment>
{
public:
	MyIDAStar(heuristic& h, double md)
		:MySearchAlgorithm<state, action, environment>(), nodesExpanded(0), maxFAllowed(md), heur(h),nextF(-1)
	{
	}
	virtual bool GetPath(environment& e, state& start, state& goal);
	bool GetPathWithinF(environment& e, state& start, state& goal, double f,double gcost);
	virtual uint64_t GetNodesExpanded() { return nodesExpanded; }
	double GetSolutionCost() { return solutionCost; }
	std::vector<action> GetActionSequence();
protected:
	uint64_t nodesExpanded;
	double maxFAllowed;
	heuristic heur;
	double nextF;
	std::stack<action> actionSequence;
	double solutionCost;
};


template <typename state, typename action, typename environment, typename heuristic>
bool MyIDAStar<state, action, environment, heuristic>::GetPath(environment& e, state& start, state& goal)
{
	solutionCost = maxFAllowed;
	nextF = -1;
	while(!actionSequence.empty())
		actionSequence.pop();
	nodesExpanded = 0;
	while (nextF < maxFAllowed)
	{
		//std::cout << "nextF" << nextF << "\n";
		if (GetPathWithinF(e, start, goal, nextF, 0))
			return true;
	}
	return false;
}

template <typename state, typename action, typename environment, typename heuristic>
bool MyIDAStar<state, action, environment, heuristic>::GetPathWithinF(environment& e, state& start, state& goal, double f,double gcost)
{
	double hcost = heur.GetHCost(start);
	double fcost = hcost + gcost;

	if (fcost > f)
	{
		if (nextF == f)
			nextF = fcost;
		else if (nextF > f&&fcost < nextF)
				nextF = fcost;

		return false;
	}
	nodesExpanded++;
	if (start == goal)
	{
		solutionCost = gcost;
		return true;
	}
		
	std::vector<action> actions;
	e.GetActions(start, actions);
	for (int i = 0; i < actions.size(); i++)
	{
		if (!actionSequence.empty() && 
			e.GetInvertAction(actions[i]) == actionSequence.top())
			continue;
		e.ApplyAction(start, actions[i]);
		actionSequence.push(actions[i]);
		

		if (GetPathWithinF(e, start, goal, f, gcost + e.GetActionCost(actions[i])))
			return true;

		e.UndoAction(start, actions[i]);
		actionSequence.pop();
	}

	return false;
}

template <typename state, typename action, typename environment, typename heuristic>
std::vector<action> MyIDAStar<state, action, environment, heuristic>::GetActionSequence()
{
	std::vector<action> result;
	std::stack<action> tmp;
	//use 2 stacks as 1 queue
	while (!actionSequence.empty())
	{
		tmp.push(actionSequence.top());
		actionSequence.pop();
	}
	while (!tmp.empty())
	{
		result.push_back(tmp.top());
		tmp.pop();
	}
	return result;
}
#endif // ! MY_IDASTAR_H
