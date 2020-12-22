#include <FittingFunction.h>
#include <math.h>
#include <iostream>
FittingFunction::FittingFunction(Dataset pointsToCluster)
:pointsToCluster(pointsToCluster)
{}

double FittingFunction::calculateFitting(std::vector<int> coordinates, int clusterAmount, int precision)
{
	double result = 0;
	std::vector<std::vector<double>> centroids = functions::separateCoordinates(coordinates, clusterAmount, precision);
	std::vector<DataEntry> points = pointsToCluster.getData();
	std::vector<double> sumOfClusterDistances;
	std::vector<int> numberOfPointsInCluster;
	numberOfPointsInCluster.resize(centroids.size(), 0);
	sumOfClusterDistances.resize(centroids.size(), 0);
	for(auto& point : points)
	{
		int bestLabel = -1;
		double bestLabelDistance = -1;
		for(int centroid = 0; centroid < (int)centroids.size(); centroid++)
		{
			double distance = functions::calculateEuclideanDistanceBetweenVectors(centroids[centroid], point.actualData);
			if(bestLabel == -1 || bestLabelDistance > distance)
			{
				bestLabel = centroid;
				bestLabelDistance = distance;
			}
		}
		numberOfPointsInCluster[bestLabel]++;
		sumOfClusterDistances[bestLabel] += bestLabelDistance;
	}
	for(int cluster = 0; cluster < (int)numberOfPointsInCluster.size(); cluster++)
	{
		if(numberOfPointsInCluster[cluster] != 0)
		{
			result += sumOfClusterDistances[cluster];	
		}
		else
		{
			result += 1000;
		}
	}
	result /= (double)points.size();
	return (-1) * result;
}