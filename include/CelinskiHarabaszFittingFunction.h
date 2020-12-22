#pragma once
#include "FittingFunctionBase.h"
#include <vector>
class CelinskiHarabaszFittingFunction : public FittingFunctionBase
{
public:
	CelinskiHarabaszFittingFunction(const Dataset& pointsToCluster);
	~CelinskiHarabaszFittingFunction() = default;
	virtual double calculateFitting(std::vector<int> coordinates, int clusterAmount, int precision)const override;

private:
	std::vector<double> datasetCenter;
	double calculateBetweenClusterVariance(std::vector<int> coordinates, int clusterAmount, int precision)const;
	double calculateWithinClusterVariance(std::vector<int> coordinates, int clusterAmount, int precision)const;
};