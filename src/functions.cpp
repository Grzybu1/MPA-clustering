#include "functions.h"
#include <iostream>
std::vector<Dimension> functions::createDimensionsRanges(int precision, int dimensionsAmount, int clusterAmount)
{
	std::vector<Dimension> result;
	for(int i = 0; i < clusterAmount; i++)
	{
		for(int j = 0; j < dimensionsAmount; j++)
		{
			Dimension multipliedDimension;
			multipliedDimension.minValue = 0;
			multipliedDimension.maxValue = pow(10, precision);
			result.push_back(multipliedDimension);
		}
	}
	return result;
}

double functions::calculateEuclideanDistanceBetweenVectors(const std::vector<double>& firstVec, const std::vector<double>& secondVec)
{
	double result = 0;
	for(int index = 0; index < (int)firstVec.size(); index++)
	{
		result += pow(firstVec[index] - secondVec[index], 2);
	}
	result = sqrt(result);
	return result;
}

std::vector<std::vector<double>> functions::separateCoordinates(std::vector<int> coordinates, int clusterAmount, int precision)
{
	std::vector<std::vector<double>> centroids;
	centroids.resize(clusterAmount);
	int dimensionsAmount = coordinates.size()/clusterAmount;
	for(int cluster = 0; cluster < clusterAmount; cluster++)
	{
		for(int dimension = 0; dimension < dimensionsAmount; dimension++)
		{
			int index = cluster * dimensionsAmount + dimension;
			centroids[cluster].push_back((double)coordinates[index]/(double)pow(10, precision));
		}
	}
	return centroids;
}

double functions::compareLabeling(const Dataset& first, const Dataset& second)
{
	int numOfLabels = (int)first.getPossibleLabels().size();
	double bestResult = 0;
	double result;
	int elementsAmount = first.getData().size();
	for(int firstLabelToSwap = 0; firstLabelToSwap < numOfLabels; firstLabelToSwap++)
	{
		for(int secondLabelToSwap = firstLabelToSwap+1; secondLabelToSwap < numOfLabels; secondLabelToSwap++)
		{
			result = 0;
			Dataset tmp = second;
			for(int i = 0; i < elementsAmount; i++)
			{
				if(tmp.getData()[i].label == firstLabelToSwap)
					tmp.setLabel(i, secondLabelToSwap);
				else if(tmp.getData()[i].label == secondLabelToSwap)
					tmp.setLabel(i, firstLabelToSwap);
			}
			for(int i = 0; i < elementsAmount; i++)
			{
				if(first.getData()[i].label == tmp.getData()[i].label)
					result++;
			}
			result /= (double)elementsAmount;
			if(result > bestResult)
				bestResult = result;
		}
	}

	for(int reLabel = 0; reLabel < numOfLabels; reLabel++)
	{
		result = 0;
		Dataset tmp = second;
		for(int i = 0; i < elementsAmount; i++)
		{
			tmp.setLabel(i, (tmp.getData()[i].label + reLabel) % numOfLabels);
		}
		for(int i = 0; i < elementsAmount; i++)
		{
			if(first.getData()[i].label == tmp.getData()[i].label)
				result++;
		}
		result /= (double)elementsAmount;
		if(result > bestResult)
			bestResult = result;
	}
	return bestResult;
}