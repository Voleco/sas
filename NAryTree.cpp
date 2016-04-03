#include "NAryTree.h"

void NAryTree::GetActions(NAryTreeState &nodeID, std::vector<NAryTreeAction> &actions)
{
	actions.clear();
	if (GetDepth(nodeID) >= dep)
		return;
	for (unsigned int i = 1; i <= bf; i++)
		actions.push_back(i);
}

void NAryTree::ApplyAction(NAryTreeState &s, NAryTreeAction a)
{
	s = s * bf + a;
}

void NAryTree::UndoAction(NAryTreeState &s, NAryTreeAction a)
{
	s = (s - a) / bf;
}

unsigned int NAryTree::GetDepth(NAryTreeState& s)
{
	unsigned int depth = 0;
	uint64_t cum = 1;
	while (s >= cum)
	{
		depth++;
		cum = cum*bf+1;
	}

	return depth;
}

unsigned int NAryTree::GetSolutionDepthUpperBound(NAryTreeState& s, NAryTreeState& g)
{
	return GetDepth(g) > dep ? dep : GetDepth(g);
}