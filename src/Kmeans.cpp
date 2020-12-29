#include "Kmeans.h"
#include <iostream>
#include <random>
Kmeans::Kmeans(const int& clusterAmount, const Dataset& dataToCluster, const int& precision, const int& iterations, const FittingFunctionBase& fittingFunction)
:clusterAmount(clusterAmount), dataToCluster(dataToCluster), precision(precision), iterations(iterations), fittingFunction(fittingFunction)
{
	dimNumber = dataToCluster.getData()[0].actualData.size();
}

std::vector<int> Kmeans::clusterData()
{
	double bestFit;
	std::vector<std::vector<double>> bestCentroids;
	for(int iter = 0; iter++ < iterations; iter++)
	{
		std::vector<int> startingPoints = getStartingPoints();
		std::vector<std::vector<double>> centroids;
		for(auto & elementIndex : startingPoints)
		{
			centroids.push_back(dataToCluster.getData()[elementIndex].actualData);
		}
		
		dataToCluster.assignPointsToClusters(centroids);
		
		bestFit = fittingFunction.calculateFitting(mergeCoordinates(centroids), clusterAmount, precision);
		bestCentroids = centroids;
		int repeat = 0;
		std::vector<std::vector<double>> newCentroids = calculateNewCentroids();
		while(newCentroids != centroids && repeat < 100)
		{
			repeat++;
			centroids = newCentroids;
			dataToCluster.assignPointsToClusters(centroids);
			newCentroids = calculateNewCentroids();

			double fit = fittingFunction.calculateFitting(mergeCoordinates(centroids), clusterAmount, precision);
			if(fit > bestFit)
			{
				bestCentroids = centroids;
				bestFit = fit;
			}
		}
	}
	return mergeCoordinates(bestCentroids);
}

std::vector<int> Kmeans::getStartingPoints()
{
	int numOfElements = dataToCluster.getData().size();
	std::vector<int> startingPoints;
	while((int)startingPoints.size() < clusterAmount)
	{
		bool alreadyIn = false;
		int index = rand() % numOfElements;
		for(auto& element: startingPoints)
		{
			if(element == index)
				alreadyIn = true;
		}
		if(!alreadyIn)
			startingPoints.push_back(index);
	}
	return startingPoints;
}

std::vector<std::vector<double>> Kmeans::calculateNewCentroids()
{
	std::vector<std::vector<double>> centroids;
	centroids.resize(clusterAmount, std::vector<double> (dimNumber, 0));
	for(auto& point : dataToCluster.getData())
	{
		for(int i = 0; i < dimNumber; i++)
		{
			centroids[point.label][i] += point.actualData[i];
		}
	}
	for(int i = 0; i < clusterAmount; i++)
	{
		for(auto& dimension : centroids[i])
		{
			dimension /= dataToCluster.getLabelsDistribution()[i];
		}
	}
	return centroids;
}

std::vector<int> Kmeans::mergeCoordinates(std::vector<std::vector<double>> centroids)
{
	std::vector<int> coordinates;
	for(int i = 0; i < clusterAmount; i++)
	{
		for(int j = 0; j < dimNumber; j++)
		{
			coordinates.push_back(int(centroids[i][j] * pow(10, precision)));
		}
	}
	return coordinates;
}