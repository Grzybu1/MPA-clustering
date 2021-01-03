#pragma once
#include <vector>
#include "Dataset.h"

enum DbscanPointStatus
{
	Unvisited,
	Noise,
	Core,
	Border,
	Checking
};

class Dbscan
{
public:
	Dbscan(const double& epsilon, const int& minPoints, const Dataset& dataToCluster);
	~Dbscan() = default;
	Dataset clusterData();
private:
	bool isEveryPointVisited();
	int getStartingPointIndex();
	bool formClusterIfPossible(const int& startingPointIndex, const int& labelForCluster);
	std::vector<int> findNeighbours(const int& pointIndex);
	std::vector<DbscanPointStatus> pointsStatus;
	double epsilon;
	int minPointsInCluster;
	Dataset dataToCluster;
};