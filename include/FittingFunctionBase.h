#pragma once
#include <vector>
#include "Dataset.h"
class FittingFunctionBase
{
public:
	FittingFunctionBase(const Dataset& pointsToCluster)
	:pointsToCluster(pointsToCluster)
	{}

	virtual ~FittingFunctionBase() = default;
	virtual double calculateFitting(std::vector<int> coordinates, int clusterAmount, int precision)const = 0;
protected:
	const Dataset& pointsToCluster;
};