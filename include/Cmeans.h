#pragma once
#include <vector>
#include "Agent.h"
#include "Dataset.h"
class Cmeans
{
public:
	Cmeans(const int& clusterAmount, const Dataset& dataToCluster, const int& precision, const int& iterations, const FittingFunctionBase& fittingFunction, const double& m = 2);
	~Cmeans()=default;
	std::vector<int> clusterData();
private:
	std::vector<int> getStartingPoints();
	void calculateNewCentroids(std::vector<std::vector<double>>& centroids);
	std::vector<int> mergeCoordinates(std::vector<std::vector<double>> centroids);
	void updatePointsMembership(const std::vector<std::vector<double>>& centroids);
	std::vector<std::vector<double>> pointsMembershipToClusters;
	int clusterAmount;
	Dataset dataToCluster;
	int precision;
	int iterations;
	int dimNumber;
	const FittingFunctionBase& fittingFunction;
	double m;
};