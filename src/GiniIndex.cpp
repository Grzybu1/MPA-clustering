#include "GiniIndex.h"
#include "functions.h"
#include <iostream> 
GiniIndex::GiniIndex(const Dataset& pointsToCluster)
:FittingFunctionBase(pointsToCluster)
{}

double GiniIndex::calculateFitting(std::vector<int> coordinates, int clusterAmount, int precision)const
{
	Dataset clusteredData = pointsToCluster;
	std::vector<std::vector<double>> centroids = functions::separateCoordinates(coordinates, clusterAmount, precision);
	clusteredData.assignPointsToClusters(centroids);

	int emptyClusters = 0;
	for(auto& pointsAmount : clusteredData.getLabelsDistribution())
	{
		if(pointsAmount == 0)
			emptyClusters++;
	}
	double index = calculateIndex(clusteredData.getLabelsDistribution(), clusteredData.getData().size());
	return index - emptyClusters * clusterAmount;
}


double GiniIndex::calculateIndex(const std::vector<int> split, const int& numOfElements)const
{
	double result = 1;
	for(const auto& numInCluster : split)
	{
		result -= pow(double(numInCluster) / double(numOfElements), 2);
	}
	return result;
}