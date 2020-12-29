#include "Cmeans.h"
#include "functions.h"
#include <iostream>
#include <random>
Cmeans::Cmeans(const int& clusterAmount, const Dataset& dataToCluster, const int& precision, const int& iterations, const FittingFunctionBase& fittingFunction, const double& m)
:clusterAmount(clusterAmount), dataToCluster(dataToCluster), precision(precision), iterations(iterations), fittingFunction(fittingFunction), m(m)
{
	dimNumber = dataToCluster.getData()[0].actualData.size();
	pointsMembershipToClusters.resize(dataToCluster.getData().size(), std::vector<double> (clusterAmount, 0));
}

std::vector<int> Cmeans::clusterData()
{
	double bestFit;
	std::vector<std::vector<double>> bestCentroids;
	std::vector<std::vector<double>> newCentroids;
	for(int iter = 0; iter < iterations; iter++)
	{
		std::vector<int> startingPoints = getStartingPoints();
		std::vector<std::vector<double>> centroids;
		for(auto & elementIndex : startingPoints)
		{
			centroids.push_back(dataToCluster.getData()[elementIndex].actualData);
		}
		dataToCluster.assignPointsToClusters(centroids);
		updatePointsMembership(centroids);

		bestFit = fittingFunction.calculateFitting(mergeCoordinates(centroids), clusterAmount, precision);
		bestCentroids = centroids;
		int repeat = 0;

		calculateNewCentroids(newCentroids);
		while(newCentroids != centroids && repeat < 10)
		{
			repeat++;
			centroids = newCentroids;
			dataToCluster.assignPointsToClusters(centroids);
			updatePointsMembership(centroids);
			calculateNewCentroids(newCentroids);

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

std::vector<int> Cmeans::getStartingPoints()
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

void Cmeans::calculateNewCentroids(std::vector<std::vector<double>>& centroids)
{
	centroids.clear();
	centroids.resize(clusterAmount, std::vector<double> (dimNumber, 0));
	std::vector<double> sumOfMemberships(clusterAmount, 0);
	std::vector<DataEntry> data = dataToCluster.getData();
	for(int point = 0; point < (int)data.size(); point++)
	{
		int label = data[point].label;
		for(int i = 0; i < dimNumber; i++)
		{
			centroids[label][i] += pow(pointsMembershipToClusters[point][label], m) * data[point].actualData[i];
		}
		sumOfMemberships[label] += pow(pointsMembershipToClusters[point][label], m);
	}
	for(int i = 0; i < clusterAmount; i++)
	{
		for(auto& dimension : centroids[i])
		{
			dimension /= sumOfMemberships[i];
		}
	}
}

std::vector<int> Cmeans::mergeCoordinates(std::vector<std::vector<double>> centroids)
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

void Cmeans::updatePointsMembership(const std::vector<std::vector<double>>& centroids)
{
	std::vector<DataEntry> data = dataToCluster.getData();
	for(int i = 0; i < (int)data.size(); i++)
	{
		for(int j = 0; j < clusterAmount; j++)
		{
			double sum = 0;
			double distBetweenPointAndItsCluster = functions::calculateEuclideanDistanceBetweenVectors(data[i].actualData, centroids[data[i].label]);
			for(int cluster = 0; cluster < clusterAmount; cluster++)
			{
				double distBetweenPointAndDiffCluster = functions::calculateEuclideanDistanceBetweenVectors(data[i].actualData, centroids[cluster]);
				if(distBetweenPointAndDiffCluster != 0)
				{
					sum += pow(distBetweenPointAndItsCluster/distBetweenPointAndDiffCluster, 2.0/(m-1));
				}
				else
				{
					sum += cluster == j ? 1 : -1;
				}

			}
			if(sum != -1)
				pointsMembershipToClusters[i][j] = 1.0/sum;
			else
				pointsMembershipToClusters[i][j] = 0;
		}
	}
}