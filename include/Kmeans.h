#pragma once
#include <vector>
#include "Agent.h"
#include "Dataset.h"
class Kmeans
{
public:
	Kmeans(const int& clusterAmount, const Dataset& dataToCluster, const int& precision, const int& iterations, const FittingFunctionBase& fittingFunction);
	~Kmeans()=default;
	std::vector<int> clusterData();
private:
	std::vector<int> getStartingPoints();
	std::vector<std::vector<double>> calculateNewCentroids();
	std::vector<int> mergeCoordinates(std::vector<std::vector<double>> centroids);
	int clusterAmount;
	Dataset dataToCluster;
	int precision;
	int iterations;
	int dimNumber;
	const FittingFunctionBase& fittingFunction;
};