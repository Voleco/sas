
#ifndef MY_ENVIRONMENT_H
#define MY_ENVIRONMENT_H

#include <vector>

template <class state, class action>
class MyEnvironment {
public:
	virtual void GetActions(state &nodeID, std::vector<action> &actions) = 0;
	virtual void ApplyAction(state &s, action a) = 0;
	virtual void UndoAction(state &s, action a) = 0;
	virtual action GetInvertAction(const action& a) = 0;
};



#endif // !MY_ENVIRONMENT_H


