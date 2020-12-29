#pragma once
#include "FittingFunctionBase.h"

class SilhoutteIndex : public FittingFunctionBase
{
public:
	SilhoutteIndex(const Dataset& pointsToCluster);
	~SilhoutteIndex()=default;
	virtual double calculateFitting(std::vector<int> coordinates, int clusterAmount, int precision)const override;
private:
	std::vector<double> calculatePointsInnerDistance(const Dataset& clusteredData)const;
	std::vector<double> calculatePointsOuterDistance(const Dataset& clusteredData, const int& clusterAmount)const;

};