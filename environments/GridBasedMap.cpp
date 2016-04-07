#include "GridBasedMap.h"

void GridBasedMap::GetActions(GridBasedMapState &nodeID, std::vector<GridBasedMapAction> &actions)
{
	actions.clear();
	//we are allowed to move right
	if (nodeID.coorX < maxX)
		actions.push_back(0);
	//we are allowed to move up
	if (nodeID.coorY < maxY)
		actions.push_back(1);

/*
	//we are allowed to move left
	if (nodeID.coorX > 0)
		actions.push_back(2);
	//we are allowed to move down
	if (nodeID.coorY > 0)
		actions.push_back(3);
*/
}

void GridBasedMap::ApplyAction(GridBasedMapState &s, GridBasedMapAction a)
{
	switch (a)
	{
		//moving right
	case 0:
		s.coorX++;
		break;
		//moving up
	case 1:
		s.coorY++;
		break;
/*
		//moveing left
	case 2:
		s.coorX--;
		break;
		//moving down
	case 3:
		s.coorY--;
		break;
*/
	default:
		break;
	}
}


void GridBasedMap::UndoAction(GridBasedMapState &s, GridBasedMapAction a)
{
	switch (a)
	{
		//moved right, need to move left
	case 0:
		s.coorX--;
		break;
		//moved up
	case 1:
		s.coorY--;
		break;
/*
		//moved left
		case 2:
		s.coorX++;
		break;
		//moved down
		case 3:
		s.coorY++;
		break;
*/
	default:
		break;
	}
}

unsigned int GridBasedMap::GetDepth(GridBasedMapState & s)
{
	return s.coorX + s.coorY;
}

unsigned int GridBasedMap::GetSolutionDepthUpperBound(GridBasedMapState & s, GridBasedMapState & g)
{
	return (g.coorX + g.coorY)>(maxX+maxY)?(maxX+maxY):(g.coorX+g.coorY);
}