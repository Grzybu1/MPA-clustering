#include "SilhoutteIndex.h"
#include "functions.h"
#include <iostream> 
SilhoutteIndex::SilhoutteIndex(const Dataset& pointsToCluster)
:FittingFunctionBase(pointsToCluster)
{}

double SilhoutteIndex::calculateFitting(std::vector<int> coordinates, int clusterAmount, int precision)const
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
	double index = calculateIndex(coordinates, clusterAmount, precision);
	return index - emptyClusters * clusterAmount;
}

std::vector<double> SilhoutteIndex::calculatePointsInnerDistance(const Dataset& clusteredData)const
{
	std::vector<double> result;
	std::vector<int> numOfPointsInClusters = clusteredData.getLabelsDistribution();
	std::vector<DataEntry> data = clusteredData.getData();
	for(const auto& firstPoint : data)
	{
		double sum = 0;
		for(const auto& secondPoint : data)
		{
			if(&firstPoint == &secondPoint)
				continue;
			if(firstPoint.label == secondPoint.label)
				sum += functions::calculateEuclideanDistanceBetweenVectors(firstPoint.actualData, secondPoint.actualData);
		}
		if(((double)numOfPointsInClusters[firstPoint.label] - 1) == 0)
			result.push_back(-2);
		else
			result.push_back(sum / ((double)numOfPointsInClusters[firstPoint.label] - 1));
	}
	return result;
}

std::vector<double> SilhoutteIndex::calculatePointsOuterDistance(const Dataset& clusteredData, const int& clusterAmount)const
{
	std::vector<double> result;
	std::vector<int> numOfPointsInClusters = clusteredData.getLabelsDistribution();
	std::vector<DataEntry> data = clusteredData.getData();
	for(const auto& firstPoint : data)
	{
		std::vector<double> sum;
		sum.resize(clusterAmount, 0);
		for(const auto& secondPoint : data)
		{
			if(firstPoint.label == secondPoint.label)
				continue;
			sum[secondPoint.label] += functions::calculateEuclideanDistanceBetweenVectors(firstPoint.actualData, secondPoint.actualData);
		}
		double min = sum[(firstPoint.label + 1) % clusterAmount];
		for(int cluster = 0; cluster < clusterAmount; cluster++)
		{
			if(cluster == firstPoint.label)
				continue;

			double val = sum[cluster] / (double)numOfPointsInClusters[cluster];
			if(min > val)
				min = val;
		}
		result.push_back(min);
	}
	return result;
}

double SilhoutteIndex::calculateIndex(std::vector<int> coordinates, int clusterAmount, int precision)const
{
	Dataset clusteredData = pointsToCluster;
	std::vector<std::vector<double>> centroids = functions::separateCoordinates(coordinates, clusterAmount, precision);
	clusteredData.assignPointsToClusters(centroids);

	std::vector<double> pointsInnerDistance = calculatePointsInnerDistance(clusteredData);
	std::vector<double> pointsOuterDistance = calculatePointsOuterDistance(clusteredData, clusterAmount);
	double result = 0;
	int dataSize = clusteredData.getData().size();
	for(int point = 0; point < dataSize; point++)
	{
		double a = pointsInnerDistance[point];
		double b = pointsOuterDistance[point];
		if(a != -2)
			result += (b - a) / (b > a ? b : a);
	}
	return result / dataSize;
}