
#ifndef MY_HEURISTIC_H
#define MY_HEURISTIC_H

#include <vector>

template <class state>
class MyHeuristic {
public:
	virtual int GetHCost(state &s) = 0;
};

template <class state>
class MaxHeuristic
{
public:
	MaxHeuristic(std::vector<MyHeuristic<state>*> h):heurs(h){}
	int GetHCost(state &s);

protected:
	std::vector<MyHeuristic<state>*> heurs;
};

template<class state>
int MaxHeuristic<state>::GetHCost(state & s)
{
	int max = 0;
	for (int i = 0; i < heurs.size(); i++)
		if (heurs[i]->GetHCost(s)>max)
			max = heurs[i]->GetHCost(s);
	return max;
}

#endif // !MY_HEURISTIC_H
