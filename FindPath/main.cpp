#include <iostream>
#include <queue>
#include <cmath>
#include <map>
#include <functional>

int FindPath(	const int nStartX, const int nStartY, 
				const int nTargetX, const int nTargetY, 
				const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
				int* pOutBuffer, const int nOutBufferSize);

int Heuristic(	int nextCoordinate, const int nTargetX, const int nTargetY, const int nMapWidth);

void GetNeighbors(int currentCoord, std::queue<int>& neighborQueue, const unsigned char* pMap, const int nMapWidth, const int nMapHeight);

int main()
{
	unsigned char pMap[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
							 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
							 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0,
							 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0,
							 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0,
							 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
	int pOutBuffer[66];
	int pathLength = FindPath(2, 3, 10, 0, pMap, 11, 6, pOutBuffer, 66);
	std::cout << pathLength << std::endl;
	for (int i = 0; i < pathLength; ++i)
	{
		std::cout << pOutBuffer[i] << std::endl;
	}
	std::cout << std::endl;
	getchar();
	return 0;
}

int FindPath(	const int nStartX, const int nStartY,
				const int nTargetX, const int nTargetY,
				const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
				int* pOutBuffer, const int nOutBufferSize)
{
	typedef std::pair <int, int> Coordinate;
	std::priority_queue<Coordinate, std::vector<Coordinate>, std::greater<Coordinate>> openSet;
	std::map<int, int> cameFrom, costMap;
	int length = 0, currentCoord = (nStartY * nMapWidth + nStartX);
	openSet.push(Coordinate(0, currentCoord));
	cameFrom[currentCoord] = currentCoord;
	costMap[currentCoord] = 0;

	while (!openSet.empty())
	{
		currentCoord = openSet.top().second;
		openSet.pop();
		if (currentCoord == (nTargetY * nMapWidth + nTargetX))
		{
			while (currentCoord != nStartY * nMapWidth + nStartX)
			{
				if (length > nOutBufferSize)
				{
					return -1;
				}
				pOutBuffer[length] = currentCoord;
				currentCoord = cameFrom[currentCoord];
				length++;
			}
			
				
			int end = length - 1;
			for (int i = 0; i < length; ++i)
			{
				if (i < end)
					std::swap(pOutBuffer[i], pOutBuffer[end]);
				--end;
			}
			return length;
		}
		{
			std::queue<int> neighborSet;
			GetNeighbors(currentCoord, neighborSet, pMap, nMapWidth, nMapHeight);
			while (!neighborSet.empty())
			{	
				int nextCost = costMap[currentCoord] + 1;
				if (!cameFrom.count(neighborSet.front()) || nextCost < costMap[neighborSet.front()])
				{
					int priority = nextCost + Heuristic(neighborSet.front(), nTargetX, nTargetY, nMapWidth);
					openSet.push(Coordinate(priority, neighborSet.front()));
					cameFrom[neighborSet.front()] = currentCoord;
					costMap[neighborSet.front()] = nextCost;
				}
				neighborSet.pop();
			}
		}
	}
	return -1;
}

int Heuristic(	int nextCoordinate, const int nTargetX, const int nTargetY, const int nMapWidth)
{
	int nextX = nextCoordinate % nMapWidth, nextY = (nextCoordinate - nextX)/nMapWidth;
	return std::abs(nextX - nTargetX) + std::abs(nextY - nTargetY);
}

void GetNeighbors(int currentCoord, std::queue<int>& neighborQueue, const unsigned char* pMap, const int nMapWidth, const int nMapHeight)
{
	if (currentCoord > (nMapWidth-1) && pMap[currentCoord - nMapWidth] != 0)
	{
		neighborQueue.push(currentCoord - nMapWidth);
	}
	if (currentCoord%nMapWidth != 0 && pMap[currentCoord - 1] != 0)
	{
		neighborQueue.push(currentCoord - 1);
	}
	if (currentCoord < ((nMapHeight - 1)*nMapWidth) && pMap[currentCoord + nMapWidth] != 0)
	{
		neighborQueue.push(currentCoord + nMapWidth);
	}
	if ((currentCoord + 1) % nMapWidth != 0 && pMap[currentCoord + 1] != 0)
	{
		neighborQueue.push(currentCoord + 1);
	}
}