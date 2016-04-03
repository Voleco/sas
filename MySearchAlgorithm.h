
#ifndef MY_SEARCH_ALGORITHM_H
#define MY_SEARCH_ALGORITHM_H

#include <stdint.h>

template <typename state, typename action, typename environment>
class MySearchAlgorithm
{
public:
	MySearchAlgorithm() {}
	MySearchAlgorithm(environment &e, state& start, state& goal) :env(e),s(start),g(goal){}
	// GetPath returns if the goal was found
	virtual bool GetPath(environment &e, state &start, state &goal) = 0;
	// Returns the total nodes expanded by the last GetPath call.
	virtual uint64_t GetNodesExpanded() = 0;
protected:
	state s;
	state g;
	environment env;
	// ...
};


#endif // !MY_SEARCH_ALGORITHM_H


