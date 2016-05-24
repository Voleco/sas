#include "Map2D.h"

Map2D::Map2D(const Map2D & m)
	:width(m.width),height(m.height),mapType(m.mapType)
{
	if (m.map != NULL)
	{
		map = new char[width*height];
		for (int i = 0; i < width*height; i++)
			map[i] = m.map[i];
	}
	else
		map = NULL;
}

//accoring to http://web.cs.du.edu/~sturtevant/papers/benchmarks.pdf
//diagonal moves are allowed only if both related cardinal moves are allowed
void Map2D::GetActions(Map2DState &nodeID, std::vector<Map2DAction> &actions)
{
	actions.resize(0);
	if (nodeID.x < 0 || nodeID.x >= width || nodeID.y < 0 || nodeID.y >= height)
		return;
	if (map[nodeID.y * width + nodeID.x] != '.')
		return;

	bool l = false;
	bool r = false;
	bool u = false;
	bool d = false;
	if (mapType == OCTILE)
	{
		if (nodeID.x + 1 < width && map[nodeID.y * width + nodeID.x + 1] == '.')
		{
			actions.push_back(tR);
			r = true;
		}
		if (nodeID.x - 1 >= 0 && map[nodeID.y * width + nodeID.x - 1 ] == '.')
		{
			actions.push_back(tL);
			l = true;
		}
		if (nodeID.y + 1 < height && map[(nodeID.y + 1)*width + nodeID.x] == '.')
		{
			actions.push_back(tD);
			d = true;
		}
		if (nodeID.y - 1 >= 0 && map[(nodeID.y - 1)*width + nodeID.x] == '.')
		{
			actions.push_back(tU);
			u = true;
		}			
		if ( d && r
			 && map[(nodeID.y + 1)*width + nodeID.x + 1] == '.')
			actions.push_back(tDR);
		if ( u && r
			 && map[(nodeID.y - 1)*width + nodeID.x + 1] == '.')
			actions.push_back(tUR);
		if ( d && l
			 && map[(nodeID.y + 1)*width + nodeID.x - 1] == '.')
			actions.push_back(tDL);
		if ( u && l
			 && map[(nodeID.y - 1)*width + nodeID.x - 1] == '.')
			actions.push_back(tUL);
	}
	//The following code should not be used in this experiment
	if (mapType == FOUR_WAY)
	{
		if (nodeID.x + 1 < width && map[nodeID.y * width + nodeID.x + 1] == '.')
			actions.push_back(tR);
		if (nodeID.x - 1 >= 0 && map[nodeID.y * width + nodeID.x - 1] == '.')
			actions.push_back(tL);
		if (nodeID.y + 1 < height && map[(nodeID.y + 1)*width + nodeID.x] == '.')
			actions.push_back(tD);
		if (nodeID.y - 1 >= 0 && map[(nodeID.y - 1)*width + nodeID.x] == '.')
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
			map[j*width + i] = line[i];
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

double OctileDistanceHeuristic::GetHCost(Map2DState& s1, Map2DState& s2)
{
	int xdiff = abs(s1.x - s2.x);
	int ydiff = abs(s1.y - s2.y);

	if (xdiff > ydiff)
	{
		return ydiff*SQUARE_ROOT_OF2 + (xdiff - ydiff);
	}
	else
	{
		return xdiff*SQUARE_ROOT_OF2 + (ydiff - xdiff);
	}
}

bool Map2DDifferentialHeuristic::LoadMap(std::string fileName)
{
	if (!env.LoadMap(fileName))
		return false;
	width = env.width;
	height = env.height;

	if (img)
		delete img;
	img = new unsigned char [3 * width*height];
	memset(img, 0, sizeof(img));

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			//std::cout << env.map[j*width + i];
			if (env.map[(height-1-j)*width + i] == '.')
			{
				img[(i + j*width) * 3 + 2] = 255;
				img[(i + j*width) * 3 + 1] = 255;
				img[(i + j*width) * 3 + 0] = 255;
			}
			else
			{
				img[(i + j*width) * 3 + 2] = 0;
				img[(i + j*width) * 3 + 1] = 0;
				img[(i + j*width) * 3 + 0] = 0;
			}
		}
		//std::cout << "\n";
	}

	return true;
}

void Map2DDifferentialHeuristic::SaveAsBMP(std::string fileName, int index)
{
	//the file format info referred from
	//https://en.wikipedia.org/wiki/BMP_file_format
	unsigned char fileHeader[14] = {
		'B','M', // BMP file specifiers
		0,0,0,0, // size in bytes
		0,0, // app data
		0,0, // app data
		40 + 14,0,0,0 // start of data offset
	};
	unsigned char DIBHeader[40] = {
		40,0,0,0, // info hd size
		0,0,0,0, // width
		0,0,0,0, // heigth
		1,0, // number color planes
		24,0, // bits per pixel
		0,0,0,0, // compression is none
		0,0,0,0, // image bits size
		0xC3,0x03,0,0, // horz resoluition in pixel / m
		0xC3,0x03,0,0, // vert resolutions (0x03C3 = 96 dpi, 0x0B13 = 72 dpi)
		0,0,0,0, // #colors in pallete
		0,0,0,0, // #important colors
	};

	int w = width * AMP_FACTOR;
	int h = height * AMP_FACTOR;

	int padSize = (4 - (w * 3) % 4) % 4;
	int sizeData = w*h * 3 + h * padSize;

	int sizeAll = sizeData + sizeof(fileHeader) + sizeof(DIBHeader);

	fileHeader[2] = (unsigned char)(sizeAll);
	fileHeader[3] = (unsigned char)(sizeAll >> 8);
	fileHeader[4] = (unsigned char)(sizeAll >> 16);
	fileHeader[5] = (unsigned char)(sizeAll >> 24);

	DIBHeader[4] = (unsigned char)(w);
	DIBHeader[5] = (unsigned char)(w >> 8);
	DIBHeader[6] = (unsigned char)(w >> 16);
	DIBHeader[7] = (unsigned char)(w >> 24);

	DIBHeader[8] = (unsigned char)(h);
	DIBHeader[9] = (unsigned char)(h >> 8);
	DIBHeader[10] = (unsigned char)(h >> 16);
	DIBHeader[11] = (unsigned char)(h >> 24);


	DIBHeader[20] = (unsigned char)(sizeData);
	DIBHeader[21] = (unsigned char)(sizeData >> 8);
	DIBHeader[22] = (unsigned char)(sizeData >> 16);
	DIBHeader[23] = (unsigned char)(sizeData >> 24);

	fileName += "_pivot_" + std::to_string(index) + ".bmp";
	FILE *f = fopen(fileName.c_str(), "w+b");
	fwrite(fileHeader, 1, 14, f);
	fwrite(DIBHeader, 1, 40, f);

	unsigned char bmppad[3] = { 0,0,0 };

	for (int j = 0; j<height; j++)
	{
		for (int outer = 0; outer < AMP_FACTOR; outer++)
		{
			for (int i = 0; i < width; i++)
			{
				for (int inner = 0; inner < AMP_FACTOR; inner++)
				{
					if(index == -1)
						fwrite(img + ((j*width + i) * 3), 3, 1, f);
					else
						fwrite(imgs[index] + ((j*width + i) * 3), 3, 1, f);
				}
					
			}
			fwrite(bmppad, 1, padSize, f);
		}
	}
	fclose(f);
}

double Map2DDifferentialHeuristic::GetHCost(Map2DState& s)
{
	double value = 0;
	uint64_t stateRank;
	uint64_t goalRank;
	env.GetRankFromState(s, stateRank);
	env.GetRankFromState(goal, goalRank);
	for (int i = 0; i < heurTable.size(); i++)
	{
		if (built[i])
		{
			if (fabs(heurTable[i][stateRank] - heurTable[i][goalRank])>value)
				value = fabs(heurTable[i][stateRank] - heurTable[i][goalRank]);
		}
	}
	return value;
}

double Map2DDifferentialHeuristic::GetHCost(Map2DState& s1, Map2DState& s2)
{
	double value = 0;
	uint64_t stateRank;
	uint64_t goalRank;
	env.GetRankFromState(s1, stateRank);
	env.GetRankFromState(s2, goalRank);
	for (int i = 0; i < heurTable.size(); i++)
	{
		if (built[i])
		{
			if (fabs(heurTable[i][stateRank] - heurTable[i][goalRank])>value)
				value = fabs(heurTable[i][stateRank] - heurTable[i][goalRank]);
		}
	}
	return value;
}

void Map2DDifferentialHeuristic::AddPivot(Map2DState& s)
{
	pivots.push_back(s);
	built.push_back(false);	
	heurTable.push_back(NULL); 
	imgs.push_back(NULL);	

	img[(s.x + (height - 1 - s.y)*width) * 3 + 2] = 255;
	img[(s.x + (height - 1 - s.y)*width) * 3 + 1] = 0;
	img[(s.x + (height - 1 - s.y)*width) * 3 + 0] = 0;
}

void Map2DDifferentialHeuristic::ChangePivot(Map2DState & s, int index)
{
	img[(pivots[index].x + (height - 1 - pivots[index].y)*width) * 3 + 2] = 255;
	img[(pivots[index].x + (height - 1 - pivots[index].y)*width) * 3 + 1] = 255;
	img[(pivots[index].x + (height - 1 - pivots[index].y)*width) * 3 + 0] = 255;

	pivots[index] = s;
	built[index] = false;

	img[(s.x + (height - 1 - s.y)*width) * 3 + 2] = 255;
	img[(s.x + (height - 1 - s.y)*width) * 3 + 1] = 0;
	img[(s.x + (height - 1 - s.y)*width) * 3 + 0] = 0;
}


void Map2DDifferentialHeuristic::BuildPDBs()
{
	for (int i = 0; i < built.size(); i++)
	{
		if (!built[i])
		{
			BuildPDB(i);
			built[i] = true;
		}
	}
}

void Map2DDifferentialHeuristic::BuildPDB(int index)
{
	Map2DState start = pivots[index];
	if (heurTable[index])
		delete heurTable[index];
	heurTable[index] = new double[width*height];
	for (int i = 0; i < width*height; i++)
		heurTable[index][i] = -1.0;

	MyBinaryHeap<StateInfo, uint64_t, StateInfoLess> openList;
	std::unordered_set<uint64_t> closedList;

	StateInfo info;
	info.gcost = 0;
	info.hcost = 0;
	uint64_t startRank;
	env.GetRankFromState(start, startRank);
	openList.Insert(info, startRank);
	//openList.
	uint64_t next;
	while (!openList.Empty())
	{
		//choose best node from openlist;
		next = openList.ExtractMin();
		info = openList.MinKey();
		//remove it from open, add it to closed
		openList.DeleteMin();
		closedList.insert(next);

		heurTable[index][next] = info.gcost;

		Map2DState successor;
		env.GetStateFromRank(successor, next);


		std::vector<Map2DAction> actions;
		env.GetActions(successor, actions);

		for (int i = 0; i < actions.size(); i++)
		{
			env.ApplyAction(successor, actions[i]);
			double edgeCost = env.GetActionCost(actions[i]);

			StateInfo succinfo;
			succinfo.gcost = info.gcost + edgeCost;
			succinfo.hcost = 0;
			//succinfo.hcost = heur.GetHCost(successor);
			uint64_t succrank;
			env.GetRankFromState(successor, succrank);

			if (!openList.IsExist(succrank))
			{
				//otherwise this node is expanded. as for consistent heuristic, we can ignore it
				if (closedList.find(succrank) != closedList.end())
					;
				//this node is ungenerated
				else
					openList.Insert(succinfo, succrank);
			}
			//in this case, this state is already on open. We may need to update its gcost 
			else
			{
				StateInfo preinfo = openList.GetKey(succrank);
				if (preinfo.gcost > succinfo.gcost)
					openList.DecreaseKey(succinfo, succrank);
			}

			env.UndoAction(successor, actions[i]);
		}
	}

	double sf = 255/heurTable[index][next];
	//std::cout << "sf" << sf;

	if (imgs[index])
		delete imgs[index];
	imgs[index] = new unsigned char[3 * width*height];
	memset(imgs[index], 0, sizeof(imgs[index]));

	

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			if (heurTable[index][i+(height-1-j)*width]==-1)
			{
				imgs[index][(i + j*width) * 3 + 2] = 0;
				imgs[index][(i + j*width) * 3 + 1] = 0;
				imgs[index][(i + j*width) * 3 + 0] = 0;
			}
			else
			{
				int diff = heurTable[index][i + (height-1-j)*width] * sf;
				imgs[index][(i + j*width) * 3 + 2] = 255 - (diff);
				imgs[index][(i + j*width) * 3 + 1] = 0;
				imgs[index][(i + j*width) * 3 + 0] = 0+ (diff);
			}
		}
	}
	//mark the pivot with green color
	int i = startRank % width;
	int j = startRank / width;
	imgs[index][(i + (height - 1 - j)*width) * 3 + 2] = 0;
	imgs[index][(i + (height - 1 - j)*width) * 3 + 1] = 255;
	imgs[index][(i + (height - 1 - j)*width) * 3 + 0] = 0;
	//i = next % width;
	//j = next / width;
	//imgs[index][(i + (height - 1 - j)*width) * 3 + 2] = 255;
	//imgs[index][(i + (height - 1 - j)*width) * 3 + 1] = 255;
	//imgs[index][(i + (height - 1 - j)*width) * 3 + 0] = 0;
}

Map2DState Map2DDifferentialHeuristic::GetFarthestPivot()
{
	Map2DState res(-1, -1);
	if (pivots.size() < 1)
		return res;

	MyBinaryHeap<StateInfo, uint64_t, StateInfoLess> openList;
	std::unordered_set<uint64_t> closedList;

	StateInfo info;
	info.gcost = 0;
	info.hcost = 0;
	uint64_t startRank;
	for (int i = 0; i < pivots.size(); i++)
	{
		env.GetRankFromState(pivots[i], startRank);
		openList.Insert(info, startRank);
	}

	//openList.
	uint64_t next;
	while (!openList.Empty())
	{
		//choose best node from openlist;
		next = openList.ExtractMin();
		info = openList.MinKey();
		//remove it from open, add it to closed
		openList.DeleteMin();
		closedList.insert(next);

		Map2DState successor;
		env.GetStateFromRank(successor, next);

		std::vector<Map2DAction> actions;
		env.GetActions(successor, actions);

		for (int i = 0; i < actions.size(); i++)
		{
			env.ApplyAction(successor, actions[i]);
			double edgeCost = env.GetActionCost(actions[i]);

			StateInfo succinfo;
			succinfo.gcost = info.gcost + edgeCost;
			succinfo.hcost = 0;
			//succinfo.hcost = heur.GetHCost(successor);
			uint64_t succrank;
			env.GetRankFromState(successor, succrank);

			if (!openList.IsExist(succrank))
			{
				//otherwise this node is expanded. as for consistent heuristic, we can ignore it
				if (closedList.find(succrank) != closedList.end())
					;
				//this node is ungenerated
				else
					openList.Insert(succinfo, succrank);
			}
			//in this case, this state is already on open. We may need to update its gcost 
			else
			{
				StateInfo preinfo = openList.GetKey(succrank);
				if (preinfo.gcost > succinfo.gcost)
					openList.DecreaseKey(succinfo, succrank);
			}

			env.UndoAction(successor, actions[i]);
		}
	}

	env.GetStateFromRank(res, next);

	return res;
}