#pragma once
#include <vector>
#include "functions.h"
#include "Dataset.h"

class FittingFunction
{
public:
	FittingFunction(Dataset);
	~FittingFunction() = default;
	double calculateFitting(std::vector<int> coordinates, int clusterAmount, int precision);
private:
	Dataset pointsToCluster;
	
};