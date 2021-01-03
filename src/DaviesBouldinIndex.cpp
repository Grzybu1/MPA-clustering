#include "DaviesBouldinIndex.h"
#include "functions.h"
#include "iostream"
DaviesBouldinIndex::DaviesBouldinIndex(const Dataset& pointsToCluster)
:FittingFunctionBase(pointsToCluster)
{}

double DaviesBouldinIndex::calculateFitting(std::vector<int> coordinates, int clusterAmount, int precision)const
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
	return (1.0/indexValue) - emptyClusters * clusterAmount;
}

std::vector<double> DaviesBouldinIndex::calculateAverageDistancesWithinClusters(const Dataset& clusteredData, const std::vector<std::vector<double>>& centroids)const
{
	std::vector<double> result;
	int clusterAmount = centroids.size();
	result.resize(clusterAmount, 0);
	std::vector<int> numOfPointsInClusters = clusteredData.getLabelsDistribution();
	for(auto& point : clusteredData.getData())
	{
		int cluster = point.label; 
		result[cluster] += functions::calculateEuclideanDistanceBetweenVectors(point.actualData, centroids[cluster]);
	}
	for(int cluster = 0; cluster < clusterAmount; cluster++)
	{
		result[cluster] /= numOfPointsInClusters[cluster];
	}
	return result;
}

double DaviesBouldinIndex::calculateIndex(std::vector<int> coordinates, int clusterAmount, int precision)const
{
	Dataset clusteredData = pointsToCluster;
	std::vector<std::vector<double>> centroids = functions::separateCoordinates(coordinates, clusterAmount, precision);
	clusteredData.assignPointsToClusters(centroids);
	std::vector<double> avarageInnerDistance = calculateAverageDistancesWithinClusters(clusteredData, centroids);
	double sum = 0;
	for(int firstCluster = 0; firstCluster < clusterAmount; firstCluster++)
	{
		double max = -1;
		for(int secondCluster = 0; secondCluster < clusterAmount; secondCluster++)
		{
			if(firstCluster == secondCluster)
				continue;
			double distanceBetweenCentroids = functions::calculateEuclideanDistanceBetweenVectors(centroids[firstCluster], centroids[secondCluster]);
			double R = (avarageInnerDistance[firstCluster] + avarageInnerDistance[secondCluster]) / distanceBetweenCentroids;
			if(R > max)
				max = R;
		}
		sum += max;
	}
	return sum / (double) clusterAmount;
}