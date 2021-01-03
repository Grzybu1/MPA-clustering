#include "Hierarchical.h"
#include <string>
#include <math.h>
#include "functions.h"
#include <iostream>
Hierarchical::Hierarchical(const int& clusterAmount, const Dataset& dataToCluster)
:clusterAmount(clusterAmount), dataToCluster(dataToCluster)
{
	int pointNum = dataToCluster.getData().size();
	distanceTable = std::vector<std::vector<double>> (pointNum, std::vector<double>(pointNum, 0));

}

Dataset Hierarchical::clusterData()
{
	prepareDataset();
	std::vector<DataEntry> data = dataToCluster.getData();
	while((int)dataToCluster.getPossibleLabels().size() > clusterAmount)
	{
		auto [firstLabel, secondLabel] = findClosestClusters();
		for(int point = 0; point < (int)data.size(); point++)
		{
			if(data[point].label == secondLabel)
			{
				dataToCluster.setLabel(point, firstLabel);
				data[point].label = firstLabel;
			}
		}
		std::vector<std::pair<std::string, int>> newLabels = dataToCluster.getPossibleLabels();
		for (auto i = newLabels.begin(); i != newLabels.end(); i++)
		{
			if((*i).second == secondLabel)
			{
				newLabels.erase(i);
				break;
			}
		}
		dataToCluster.setPossibleLabels(newLabels);
		updateTable(secondLabel, firstLabel);
	}
	return dataToCluster;
}

void Hierarchical::prepareDataset()
{
	std::vector<std::pair<std::string, int>> newLabels;
	std::vector<DataEntry> data = dataToCluster.getData();
	for(int pointIndex = 0; pointIndex < (int) data.size(); pointIndex++)
	{
		dataToCluster.setLabel(pointIndex, pointIndex);
		newLabels.push_back({std::to_string(pointIndex), pointIndex});
	}
	dataToCluster.setPossibleLabels(newLabels);

	int pointNum = dataToCluster.getData().size();
	for(int i = 0; i < pointNum; i++)
	{
		for(int j = i + 1; j < pointNum; j++)
		{
			double distance = calculateDistanceBetweenClusters(i, j);
			distanceTable[i][j] = distanceTable[j][i] = distance;
		}
	}

}

std::pair<int, int> Hierarchical::findClosestClusters()
{
	int firstLabel = 0;
	int secondLabel = 1;
	double minimumDistance = distanceTable[0][1];
	int startIndex = 0;
	while(minimumDistance <= 0)
	{
		for(int i = 0; i < (int)distanceTable.size(); i++)
		{
			minimumDistance = distanceTable[startIndex][i];
			if(minimumDistance > 0)
			{
				firstLabel = startIndex;
				secondLabel = i;
				break;
			}
		}
		startIndex++;
	}
	for(int i = 0; i < (int)distanceTable.size(); i++)
		for(int j = 0; j < (int)distanceTable.size(); j++)
		{
			double distance = distanceTable[i][j];
			if(distance < minimumDistance && distance > 0)
			{
				minimumDistance = distance;
				firstLabel = i;
				secondLabel = j;
			}
		}
	return {firstLabel, secondLabel};
}

double Hierarchical::calculateDistanceBetweenClusters(int firstLabel, int secondLabel)
{
	std::vector<DataEntry> data = dataToCluster.getData();
	std::vector<std::vector<double>> firstClusterPoints;
	std::vector<std::vector<double>> secondClusterPoints;
	for(auto& point : data)
	{
		if(point.label == firstLabel)
			firstClusterPoints.push_back(point.actualData);
		if(point.label == secondLabel)
			secondClusterPoints.push_back(point.actualData);
	}

	double sum = 0;
	for(auto& pointInFirstCluster : firstClusterPoints)
	{
		for(auto& pointInSecondCluster : secondClusterPoints)
		{
			sum += pow(functions::calculateEuclideanDistanceBetweenVectors(pointInFirstCluster, pointInSecondCluster), 2);
		}
	}
	return sum / (double)(firstClusterPoints.size() * secondClusterPoints.size());
}

void Hierarchical::updateTable(int deletedLabel, int mergedLabel)
{
	int pointNum = dataToCluster.getData().size();
	for(int i = 0; i < pointNum; i++)
		for(int j = i + 1; j < pointNum; j++)
		{
			if(i == deletedLabel || j == deletedLabel)
			{
				distanceTable[i][j] = distanceTable[j][i] = -1;
				continue;
			}
			if((i == mergedLabel || j == mergedLabel) && distanceTable[i][j] > 0)
				distanceTable[i][j] = distanceTable[j][i] = calculateDistanceBetweenClusters(i, j);
		}

}