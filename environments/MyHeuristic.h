
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
	MaxHeuristic() {}
	MaxHeuristic(std::vector<MyHeuristic<state>*> h) :heurs(h) {}
	~MaxHeuristic() {}
	double GetHCost(state &s);

	std::vector<MyHeuristic<state>*> heurs;
};

template <class state>
class ZeroHeuristic
{
public:
	double GetHCost(state &s) { return 0; }
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
