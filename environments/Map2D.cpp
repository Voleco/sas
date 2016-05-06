#include "Map2D.h"

Map2D::Map2D(const Map2D & m)
	:width(m.width),height(m.height),mapType(m.mapType)
{
	if (m.map != 0)
	{
		map = new char[width*height];
		for (int i = 0; i < width*height; i++)
			map[i] = m.map[i];
	}
}

void Map2D::GetActions(Map2DState &nodeID, std::vector<Map2DAction> &actions)
{
	actions.resize(0);
	if (nodeID.x < 0 || nodeID.x >= width || nodeID.y < 0 || nodeID.y >= height)
		return;
	if (map[nodeID.x * height + nodeID.y] != '.')
		return;
	if (mapType == OCTILE)
	{
		if (nodeID.x + 1 < width && map[(nodeID.x + 1)*height + nodeID.y] == '.')
			actions.push_back(tR);
		if (nodeID.x - 1 >= 0 && map[(nodeID.x - 1)*height + nodeID.y] == '.')
			actions.push_back(tL);
		if (nodeID.y + 1 < height && map[nodeID.x*height + nodeID.y + 1] == '.')
			actions.push_back(tD);
		if (nodeID.y - 1 >= 0 && map[nodeID.x*height + nodeID.y - 1] == '.')
			actions.push_back(tU);
		if (nodeID.x + 1 < width && nodeID.y + 1 < width && map[(nodeID.x + 1)*height + nodeID.y + 1] == '.')
			actions.push_back(tDR);
		if (nodeID.x + 1 < width && nodeID.y - 1 >= 0 && map[(nodeID.x + 1)*height + nodeID.y - 1] == '.')
			actions.push_back(tUR);
		if (nodeID.x - 1 >= 0 && nodeID.y + 1 < height && map[(nodeID.x - 1)*height + nodeID.y + 1] == '.')
			actions.push_back(tDL);
		if (nodeID.x - 1 >= 0 && nodeID.y - 1 >= 0 && map[(nodeID.x - 1)*height + nodeID.y - 1] == '.')
			actions.push_back(tUL);
	}
	if (mapType == FOUR_WAY)
	{
		if (nodeID.x + 1 < width && map[(nodeID.x + 1)*height + nodeID.y] == '.')
			actions.push_back(tR);
		if (nodeID.x - 1 >= 0 && map[(nodeID.x - 1)*height + nodeID.y] == '.')
			actions.push_back(tL);
		if (nodeID.y + 1 < height && map[nodeID.x*height + nodeID.y + 1] == '.')
			actions.push_back(tD);
		if (nodeID.y - 1 >= 0 && map[nodeID.x*height + nodeID.y - 1] == '.')
			actions.push_back(tU);
	}
}

void Map2D::ApplyAction(Map2DState &s, Map2DAction a)
{
	switch (a)
	{
	case tR:
		s.x++;
		break;
	case tU:
		s.y--;
		break;
	case tL:
		s.x--;
		break;
	case tD:
		s.y++;
		break;
	case tUR:
		s.x++;
		s.y--;
		break;
	case tUL:
		s.x--;
		s.y--;
		break;
	case tDR:
		s.x++;
		s.y++;
		break;
	case tDL:
		s.x--;
		s.y++;
		break;
	case tUNKNOWN:
	default:
		break;
	}
}

void Map2D::UndoAction(Map2DState &s, Map2DAction a)
{
	ApplyAction(s, GetInvertAction(a));
}

double Map2D::GetActionCost(const Map2DAction& a)
{
	switch (a)
	{
	case tR:
	case tU:
	case tL:
	case tD:
		return 1.0;
		break;
	case tUR:
	case tUL:
	case tDR:
	case tDL:
		return SQUARE_ROOT_OF2;
		break;
	case tUNKNOWN:
	default:
		return 0;
		break;
	}
}

Map2DAction Map2D::GetInvertAction(const Map2DAction& a)
{
	switch (a)
	{
	case tR:
		return tL;
		break;
	case tU:
		return tD;
		break;
	case tL:
		return tR;
		break;
	case tD:
		return tU;
		break;
	case tUR:
		return tDL;
		break;
	case tUL:
		return tDR;
		break;
	case tDR:
		return tUL;
		break;
	case tDL:
		return tUR;
		break;
	case tUNKNOWN:
	default:
		return tUNKNOWN;
		break;
	}
}

void Map2D::GetRankFromState(const Map2DState& state, uint64_t& rank)
{
	rank = state.y*width + state.x;
}

void Map2D::GetStateFromRank(Map2DState& state, const uint64_t& rank)
{
	state.y = rank / width;
	state.x = rank % width;
}

bool Map2D::LoadMap(std::string fileName)
{
	std::ifstream fin;
	fin.open(fileName);
	if (!fin.is_open())
	{
		std::cout << "fail to open" << fileName << "\n";
		return false;
	}
		
	std::string str;
	//supposed to get word "type"
	fin >> str;
	//supposed to get type, "octile"
	fin >> str;
	if (str == "octile")
		mapType = OCTILE;
	//supposed to get word "height"
	fin >> str;
	fin >> height;
	//supposed to get word "width"
	fin >> str;
	fin >> width;
	//supposed to get word "map"
	fin >> str;
	//get rid of the '\n' after word "map"
	fin.get();
	map = new char[width * height];
	char* line = new char[width + 1];
	for (int j = 0; j <height; j++)
	{
		fin.getline(line, width+1);
		for (int i = 0; i < width; i++)
		{
			map[i*height + j] = line[i];
		}
	}
	return true;
}

double OctileDistanceHeuristic::GetHCost(Map2DState& s)
{
	int xdiff = abs(s.x - goal.x);
	int ydiff = abs(s.y - goal.y);

	if (xdiff > ydiff)
	{
		return ydiff*SQUARE_ROOT_OF2 + (xdiff - ydiff);
	}
	else
	{
		return xdiff*SQUARE_ROOT_OF2 + (ydiff - xdiff);
	}
}