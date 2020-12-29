#pragma once
#include "Agent.h"
#include <math.h>
namespace functions
{
	std::vector<Dimension> createDimensionsRanges(int precision, int dimensionsAmount, int clusterAmount);
	double calculateEuclideanDistanceBetweenVectors(const std::vector<double>& firstVec, const std::vector<double>& secondVec);
	std::vector<std::vector<double>> separateCoordinates(std::vector<int> coordinates, int clusterAmount, int precision);
	double compareLabeling(const Dataset& first, const Dataset& second);
	double calculateRandIndex(const Dataset& firstSet, const Dataset& secondSet );
	// double calculateGiniIndexForDatasets(const Dataset& correctSet, const Dataset& clusteredData);
	double calculateGiniIndexForSingleSplit(const std::vector<int> split, const int& numOfElements);
	// std::vector<int> getCorrectLabelsNumber(const Dataset& first, const Dataset& second);
}