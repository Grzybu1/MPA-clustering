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

double functions::calculateRandIndex(const Dataset& firstSet, const Dataset& secondSet )
{
	int numOfPairsClusteredTogether = 0;
	int numOfPairsClusteredSeparate = 0;
	std::vector<DataEntry> firstSetData = firstSet.getData();
	std::vector<DataEntry> secondSetData = secondSet.getData();
	int numOfElements = firstSetData.size();
	for(int firstElement = 0; firstElement < numOfElements; firstElement++)
	{
		for(int secondElement = firstElement + 1; secondElement < numOfElements; secondElement++)
		{
			if(		firstSetData[firstElement].label == firstSetData[secondElement].label
				&& 	secondSetData[firstElement].label == secondSetData[secondElement].label)
			{
				numOfPairsClusteredTogether++;
			}
			else if(	firstSetData[firstElement].label != firstSetData[secondElement].label
					&& 	secondSetData[firstElement].label != secondSetData[secondElement].label)
			{
				numOfPairsClusteredSeparate++;
			}
		}
	}
	double binominalCoefiicient = numOfElements * (numOfElements - 1) / 2;
	return (double)(numOfPairsClusteredTogether + numOfPairsClusteredSeparate) / binominalCoefiicient;
}

/*double functions::calculateGiniIndexForDatasets(const Dataset& correctSet, const Dataset& clusteredData )
{
	double numOfElements = correctSet.getData().size();
	
	std::vector<int> originalSplit = correctSet.getLabelsDistribution();
	double originalGini = calculateGiniIndexForSingleSplit(originalSplit, numOfElements);
	
	std::vector<int> correctLabelsVector = getCorrectLabelsNumber(correctSet, clusteredData);
	std::vector<int> missedLabelsVector;

	int correctAmount = 0;
	int missedAmount = 0;

	for(int i = 0; i < (int)originalSplit.size(); i++)
	{
		missedLabelsVector.push_back(originalSplit[i] - correctLabelsVector[i]);
		correctAmount += correctLabelsVector[i];
		missedAmount += missedLabelsVector[i];
	}

	double missedGini = calculateGiniIndexForSingleSplit(missedLabelsVector, numOfElements);
	double correctGini = calculateGiniIndexForSingleSplit(correctLabelsVector, numOfElements);

	return ((double)correctAmount / (double)numOfElements) * correctGini + ((double)missedAmount / (double)numOfElements) * missedGini;
}*/

double functions::calculateGiniIndexForSingleSplit(const std::vector<int> split, const int& numOfElements)
{
	double result = 1;
	for(const auto& numInCluster : split)
	{
		result -= pow(double(numInCluster) / double(numOfElements), 2);
	}
	return result;
}

/*std::vector<int> getCorrectLabelsNumber(const Dataset& first, const Dataset& second)
{
	int numOfLabels = (int)first.getPossibleLabels().size();
	double bestMatch = 0;
	std::vector<int> result;
	std::vector<int> bestResult;
	int elementsAmount = first.getData().size();
	for(int firstLabelToSwap = 0; firstLabelToSwap < numOfLabels; firstLabelToSwap++)
	{
		for(int secondLabelToSwap = firstLabelToSwap+1; secondLabelToSwap < numOfLabels; secondLabelToSwap++)
		{
			result.clear();
			result.resize(numOfLabels, 0);

			double matchNum = 0;

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
					result[label]++;
			}
			for(auto& cluster : result)
				matchNum += cluster;

			if(matchNum > bestMatch)
			{
				bestResult = result;
				bestMatch = matchNum;
			}
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
				result[label]++;
		}

		for(auto& cluster : result)
				matchNum += cluster;

		if(matchNum > bestMatch)
		{
			bestResult = result;
			bestMatch = matchNum;
		}
	}
	return bestResult;
}*/