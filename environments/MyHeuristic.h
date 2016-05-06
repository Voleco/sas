
#ifndef MY_HEURISTIC_H
#define MY_HEURISTIC_H

#include <vector>

template <class state>
class MyHeuristic {
public:
	virtual double GetHCost(state &s) = 0;
};

template <class state>
class MaxHeuristic
{
public:
	MaxHeuristic(std::vector<MyHeuristic<state>*> h) :heurs(h) {}
	double GetHCost(state &s);

protected:
	std::vector<MyHeuristic<state>*> heurs;
};

template<class state>
double MaxHeuristic<state>::GetHCost(state & s)
{
	double max = 0;
	double hcost = 0;

	for (int i = 0; i < heurs.size(); i++)
	{
		hcost = heurs[i]->GetHCost(s);
		if (hcost>max)
			max = hcost;
	}	
	return max;
}

#endif // !MY_HEURISTIC_H
