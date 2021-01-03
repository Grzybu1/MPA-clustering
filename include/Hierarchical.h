#pragma once
#include "Dataset.h"

class Hierarchical
{
public:
	Hierarchical(const int& clusterAmount, const Dataset& dataToCluster);
	~Hierarchical() = default;
	Dataset clusterData();

private:
	std::vector<std::vector<double>> distanceTable;
	int clusterAmount;
	Dataset dataToCluster;
	void prepareDataset();
	std::pair<int, int> findClosestClusters();
	double calculateDistanceBetweenClusters(int firstIndex, int secondIndex);
	void updateTable(int deletedIndex, int mergedIndex);
};