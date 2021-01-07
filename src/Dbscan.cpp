#include "Dbscan.h"
#include "functions.h"
#include <iostream>

Dbscan::Dbscan(const double& epsilon, const int& minPoints, const Dataset& dataToCluster)
:epsilon(epsilon), minPointsInCluster(minPoints), dataToCluster(dataToCluster)
{
	pointsStatus = std::vector<DbscanPointStatus> (dataToCluster.getData().size(), Unvisited);
}

Dataset Dbscan::clusterData()
{
	dataToCluster.clearLabels();
	std::vector<std::pair<std::string, int>> newPossibleLabels;
	int clusterIndex = -1;
	while(!isEveryPointVisited())
	{
		clusterIndex++;
		int startingPointIndex = getStartingPointIndex();
		bool isNoise = formClusterIfPossible(startingPointIndex, clusterIndex);
		if(isNoise)
		{
			pointsStatus[startingPointIndex] = Noise;
			int newNoiseIndex = (clusterIndex + 1) * (-1);
			dataToCluster.setLabel(startingPointIndex, newNoiseIndex);
			newPossibleLabels.push_back({"Noise" , newNoiseIndex});
		}
		else
		{
			pointsStatus[startingPointIndex] = Core;
			dataToCluster.setLabel(startingPointIndex, clusterIndex);
			newPossibleLabels.push_back({std::to_string(clusterIndex), clusterIndex});
		}
	}
	dataToCluster.setPossibleLabels(newPossibleLabels);
	return dataToCluster;
}

bool Dbscan::isEveryPointVisited()
{
	for(auto& status : pointsStatus)
	{
		if(status == Unvisited)
		{
			return false;
		}
	}
	return true;
}

int Dbscan::getStartingPointIndex()
{
	int dataSize = dataToCluster.getData().size();
	int startingPointIndex = rand() % dataSize;
	while(pointsStatus[startingPointIndex] != Unvisited)
	{
		startingPointIndex = (startingPointIndex + 1) % dataSize;
	}
	return startingPointIndex;
}

std::vector<int> Dbscan::findNeighbours(const int& pointIndex)
{
	std::vector<int> result;
	std::vector<DataEntry> setData = dataToCluster.getData();
	const std::vector<double>& pointData = setData[pointIndex].actualData;
	for(int index = 0; index < (int)setData.size(); index++)
	{
		if(index == pointIndex)
			continue;
		if(functions::calculateEuclideanDistanceBetweenVectors(setData[index].actualData, pointData) <= epsilon)
			result.push_back(index);
	}
	return result;
}

bool Dbscan::formClusterIfPossible(const int& startingPointIndex, const int& labelForCluster)
{
	std::vector<int> neighboursIndexes = findNeighbours(startingPointIndex);
	if((int)neighboursIndexes.size() < minPointsInCluster)
	{
		return true;
	}

	for(auto& pointIndex : neighboursIndexes)
	{

		if(pointsStatus[pointIndex] == Core || pointsStatus[pointIndex] == Border || pointsStatus[pointIndex] == Checking)
			continue;
		pointsStatus[pointIndex] = Checking;
		bool isBorder = formClusterIfPossible(pointIndex, labelForCluster);
		dataToCluster.setLabel(pointIndex, labelForCluster);
		if(isBorder)
		{
			pointsStatus[pointIndex] = Border;

		}
		else
		{
			pointsStatus[pointIndex] = Core;
		}
	}
	return false;
}