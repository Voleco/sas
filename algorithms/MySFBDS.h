//my implementation of single frontier bidirectional search
#ifndef MY_SFBDS_H
#define MY_SFBDS_H

#include <vector>
#include <stack>
#include "MySearchAlgorithm.h"

const double DBL_ERROR = 0.0001;

inline bool fless(double a, double b) { return (a < b - DBL_ERROR); }
inline bool fgreater(double a, double b) { return (a > b + DBL_ERROR); }
inline bool fequal(double a, double b){	return (a >= b - DBL_ERROR) && (a <= b + DBL_ERROR);}

enum JumpingPolicy
{
	JIL0 = 0,
	JIL1 = 1,
	JIL2 = 2,
	BF = 3
};

template <typename state, typename action, typename environment, typename heuristic>
class MySFBDS :public MySearchAlgorithm<state, action, environment>
{
public:
	MySFBDS(heuristic& fw, heuristic& bw, JumpingPolicy jp, double maxF=100000)
		:MySearchAlgorithm<state, action, environment>(), nodesExpanded(0),numJumps(0),
		forwardHeur(fw), backwardHeur(bw),maxFAllowed(maxF),nextF(-1),policy(jp)
	{
		std::stack<action> sqs;
		actionSequences.push_back(sqs);
		actionSequences.push_back(sqs);
	}
	virtual bool GetPath(environment& e, state& start, state& goal);
	bool GetPathWithinF(environment& e, state& start, state& goal, double f, double gcost, int direction);
	virtual uint64_t GetNodesExpanded() { return nodesExpanded; }
	uint64_t GetNumOfJumps() { return numJumps; }
	std::vector<action> GetActionSequence();
protected:
	uint64_t nodesExpanded;
	uint64_t numJumps;
	heuristic forwardHeur;
	heuristic backwardHeur;

	std::vector<std::stack<action>> actionSequences;

	JumpingPolicy policy;
	double nextF;
	double maxFAllowed;
};


template <typename state, typename action, typename environment, typename heuristic>
bool MySFBDS<state, action, environment, heuristic>::GetPath(environment& e, state& start, state& goal)
{
	//while(!actionSequence.empty())
	//	actionSequence.pop();
	nextF = -1;
	while (!actionSequences[0].empty())
		actionSequences[0].pop();
	while (!actionSequences[1].empty())
		actionSequences[1].pop();
	nodesExpanded = 0;
	numJumps = 0;
	while (nextF< maxFAllowed)
	{
		//std::cout << "nextF" << nextF << "\n";
		if (GetPathWithinF(e, start, goal, nextF, 0, 0))
			return true;
	}
	return false;
}

template <typename state, typename action, typename environment, typename heuristic>
bool MySFBDS<state, action, environment, heuristic>::GetPathWithinF(environment& e, state& start, state& goal, double f,double gcost, int direction)
{
	double fwhcost = forwardHeur.GetHCost(start,goal);
	double bwhcost = backwardHeur.GetHCost(goal, start);
	
	double fcost = gcost + (fless(fwhcost, bwhcost) ? fwhcost : bwhcost);
	//double fcost = gcost + fwhcost;
	//std::cout << "fcost: " << fcost << "\n";
	if (fgreater(fcost, f))
	//if (fcost > f)
	{
		if (fequal(nextF,f))
		//if (nextF== f)
			nextF = fcost;
		else if (fgreater(nextF,f) && fless(fcost, nextF))
		//else if (nextF> f && fcost< nextF)
			nextF = fcost;

		return false;
	}
	nodesExpanded++;
	if (start == goal)
		return true;
	//determine the direction to expand
	int dir = 0;
	std::vector<action> actions;
	int fbf;
	int bbf;
	switch (policy)
	{
	case JIL0:
		break;
	case JIL1:
		break;
	case JIL2:
		break;
	case BF:
		e.GetActions(start, actions);
		fbf = actions.size();
		e.GetActions(goal, actions);
		bbf = actions.size();
		if (bbf < fbf)
			dir = 1;
		break;
	default:
		break;
	}
	if (dir != direction)
		numJumps++;
	
	if(dir==0)
		e.GetActions(start, actions);
	else//dir == 1
		e.GetActions(goal, actions);
	for (int i = 0; i < actions.size(); i++)
	{
		if (!actionSequences[dir].empty() && 
			e.GetInvertAction(actions[i]) == actionSequences[dir].top())
			continue;
		if(dir==0)
			e.ApplyAction(start, actions[i]);
		else
			e.ApplyAction(goal, actions[i]);
		actionSequences[dir].push(actions[i]);
		

		if (GetPathWithinF(e, start, goal, f, gcost + e.GetActionCost(actions[i]),dir))
			return true;

		if (dir == 0)
			e.UndoAction(start, actions[i]);
		else
			e.UndoAction(goal, actions[i]);
		actionSequences[dir].pop();
	}

	return false;
}

template <typename state, typename action, typename environment, typename heuristic>
std::vector<action> MySFBDS<state, action, environment, heuristic>::GetActionSequence()
{
	std::vector<action> result;
	std::stack<action> tmp;
	//use 2 stacks as 1 queue
	while (!actionSequences[0].empty())
	{
		tmp.push(actionSequences[0].top());
		actionSequences[0].pop();
	}
	while (!tmp.empty())
	{
		result.push_back(tmp.top());
		tmp.pop();
	}
	while (!actionSequences[1].empty())
	{
		result.push_back(actionSequences[1].top());
		actionSequences[1].pop();
	}
	return result;
}
#endif // ! MY_SFBDS_H
