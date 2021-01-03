#pragma once
#include "FittingFunctionBase.h"

class GiniIndex : public FittingFunctionBase
{
public:
	GiniIndex(const Dataset& pointsToCluster);
	~GiniIndex()=default;
	virtual double calculateFitting(std::vector<int> coordinates, int clusterAmount, int precision)const override;
	double calculateIndex(const std::vector<int> split, const int& numOfElements)const;
	
};