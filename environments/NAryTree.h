
#ifndef N_ARY_TREE_H
#define N_ARY_TREE_H

#include <iostream>
#include "MyEnvironment.h"

//state is just an index
typedef uint64_t NAryTreeState;

//action is the branch to take
//i.e. NAryTreeAction ranges from 0 to n-1
typedef unsigned int NAryTreeAction;

class NAryTree :public MyEnvironment<NAryTreeState, NAryTreeAction>
{
public:
	NAryTree() {}
	NAryTree(unsigned int branchingFactor, unsigned int depth) :bf(branchingFactor), dep(depth) {}
	void GetActions(NAryTreeState &nodeID, std::vector<NAryTreeAction> &actions);
	void ApplyAction(NAryTreeState &s, NAryTreeAction a);
	void UndoAction(NAryTreeState &s, NAryTreeAction a);
	unsigned int GetDepth(NAryTreeState& s);
	unsigned int GetSolutionDepthUpperBound(NAryTreeState& s, NAryTreeState& g);
private:

	unsigned int bf;
	unsigned int dep;
};

class NAryTreeHeuristic
{
public:
	NAryTreeHeuristic() { }
	~NAryTreeHeuristic() {}
	void SetGoal(NAryTreeState& s) { goal = s; }
	int GetHCost(NAryTreeState& s) { return 0; }
	NAryTreeState goal;
};


#endif // !N_ARY_TREE
