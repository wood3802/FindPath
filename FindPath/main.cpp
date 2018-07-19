#include <iostream>
#include <queue>
#include <cmath>
#include <map>
#include <functional>

int FindPath(	const int nStartX, const int nStartY, 
				const int nTargetX, const int nTargetY, 
				const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
				int* pOutBuffer, const int nOutBufferSize);

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

//Dijkstra's pathfinding algorithm 
int FindPath(	const int nStartX, const int nStartY,
				const int nTargetX, const int nTargetY,
				const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
				int* pOutBuffer, const int nOutBufferSize)
{
	//Declare the open list as a min priority queue
	typedef std::pair <int, int> Coordinate;
	std::priority_queue<Coordinate, std::vector<Coordinate>, std::greater<Coordinate>> openSet;
	//Declare the costMap, which is the cost from the start node to the node key
	//Declare the cameFrom map, which has the node previous to this as the node's key
	std::map<int, int> cameFrom, costMap;
	//Initialize the length of the path as 0, and the current coordinate as the starting node
	int length = 0, currentCoord = (nStartY * nMapWidth + nStartX);
	//Push the first node with a priority of 0, place the first node as itself in the cameFrom map, and set the cost of the first node to zero
	openSet.push(Coordinate(0, currentCoord));
	cameFrom[currentCoord] = currentCoord;
	costMap[currentCoord] = 0;

	//Begin pathfinding loop
	while (!openSet.empty())
	{
		//set the current node as the one with the lowest priority (cost)
		currentCoord = openSet.top().second;
		openSet.pop();

		//We've found the target node and now reconstruct the path backwards
		if (currentCoord == (nTargetY * nMapWidth + nTargetX))
		{
			while (currentCoord != nStartY * nMapWidth + nStartX)
			{
				//If the length of the path is longer than the buffer size, return -1 
				if (length > nOutBufferSize)
				{
					return -1;
				}
				pOutBuffer[length] = currentCoord;
				currentCoord = cameFrom[currentCoord];
				length++;
			}
			
			//Reverse the path in the buffer
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
			//Find the neighbors of the current node, find their cost and place them in the priority queue
			std::queue<int> neighborSet;
			GetNeighbors(currentCoord, neighborSet, pMap, nMapWidth, nMapHeight);
			while (!neighborSet.empty())
			{	
				//Every node is one unit away from each other
				int nextCost = costMap[currentCoord] + 1;
				if (!cameFrom.count(neighborSet.front()) || nextCost < costMap[neighborSet.front()])
				{
					int priority = nextCost;
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

//Determines which neighbors can be put into the neighbor queue
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