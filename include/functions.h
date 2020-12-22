#pragma once
#include "Agent.h"
#include <math.h>
namespace functions
{
	std::vector<Dimension> createDimensionsRanges(std::vector<Dimension> baseDimensions, int precision, int clusterAmount);
	double calculateEuclideanDistanceBetweenVectors(const std::vector<double>& firstVec, const std::vector<double>& secondVec);
	std::vector<std::vector<double>> separateCoordinates(std::vector<int> coordinates, int clusterAmount, int precision);
	double compareLabeling(const Dataset& first, const Dataset& second);
}