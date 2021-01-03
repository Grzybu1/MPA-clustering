#pragma once
#include "FittingFunctionBase.h"
#include <vector>

class DaviesBouldinIndex : public FittingFunctionBase
{
public:
	DaviesBouldinIndex(const Dataset& pointsToCluster);
	~DaviesBouldinIndex() = default;
	virtual double calculateFitting(std::vector<int> coordinates, int clusterAmount, int precision)const override;
	double calculateIndex(std::vector<int> coordinates, int clusterAmount, int precision)const;

private:
	std::vector<double> calculateAverageDistancesWithinClusters(const Dataset& clusteredData, const std::vector<std::vector<double>>& centroids)const;
};