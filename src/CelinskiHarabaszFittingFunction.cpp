#include "CelinskiHarabaszFittingFunction.h"
#include "Dataset.h"
#include "functions.h"

CelinskiHarabaszFittingFunction::CelinskiHarabaszFittingFunction(const Dataset& pointsToCluster)
:FittingFunctionBase(pointsToCluster)
{
	int dimensionAmount = pointsToCluster.getData()[0].actualData.size();
	datasetCenter.resize(dimensionAmount, 0);
	for(const auto& entry : pointsToCluster.getData())
	{
		for(int i = 0; i < dimensionAmount; i++)
		{
			datasetCenter[i] += entry.actualData[i];
		}
	}
	for(auto& dimension : datasetCenter)
	{
		dimension /= (double)pointsToCluster.getData().size();
	}
}

double CelinskiHarabaszFittingFunction::calculateFitting(std::vector<int> coordinates, int clusterAmount, int precision)const
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
	double indexValue = calculateIndex(coordinates, clusterAmount, precision);
	return indexValue - emptyClusters * pow(10, precision);
}

double CelinskiHarabaszFittingFunction::calculateBetweenClusterVariance(std::vector<int> coordinates, int clusterAmount, int precision)const
{
	double result = 0;
	Dataset clusteredData = pointsToCluster;
	std::vector<std::vector<double>> centroids = functions::separateCoordinates(coordinates, clusterAmount, precision);
	clusteredData.assignPointsToClusters(centroids);
	for(int cluster = 0; cluster < clusterAmount; cluster++)
	{
		result += functions::calculateEuclideanDistanceBetweenVectors(centroids[cluster], datasetCenter) * clusteredData.getLabelsDistribution()[cluster];
	}
	return result;
}

double CelinskiHarabaszFittingFunction::calculateWithinClusterVariance(std::vector<int> coordinates, int clusterAmount, int precision)const
{
	double result = 0;
	Dataset clusteredData = pointsToCluster;
	std::vector<std::vector<double>> centroids = functions::separateCoordinates(coordinates, clusterAmount, precision);
	clusteredData.assignPointsToClusters(centroids);
	for(const auto& entry : clusteredData.getData())
	{
		result += functions::calculateEuclideanDistanceBetweenVectors(entry.actualData, centroids[entry.label]);
	}
	return result;
}

double CelinskiHarabaszFittingFunction::calculateIndex(std::vector<int> coordinates, int clusterAmount, int precision)const
{
	double betweenClusterVariance = calculateBetweenClusterVariance(coordinates, clusterAmount, precision);
	double withinClusterVariance = calculateWithinClusterVariance(coordinates, clusterAmount, precision);
	
	int pointsAmount = pointsToCluster.getData().size();
	double indexValue = (betweenClusterVariance / withinClusterVariance) * (pointsAmount - clusterAmount)/(clusterAmount - 1);

	return indexValue;
}