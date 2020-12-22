#pragma once
#include <vector>
#include "functions.h"
#include "Dataset.h"
#include "FittingFunctionBase.h"
class FittingFunctionSimple : public FittingFunctionBase
{
public:
	FittingFunctionSimple(const Dataset& pointsToCluster);
	~FittingFunctionSimple() = default;
	double calculateFitting(std::vector<int> coordinates, int clusterAmount, int precision)const override;
private:
	
};