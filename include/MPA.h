#pragma once
#include <vector>
#include <utility>
#include "Agent.h"
#include "Dataset.h"
#include "FittingFunctionSimple.h"
class MPA
{
public:
	MPA(int populationSize, std::vector<Dimension> dimensionsRanges, int clusterAmount, int stepsNumber, Dataset pointsToCluster, int precision, const FittingFunctionBase& fittingFunction);
	~MPA() = default;
	void writePopulationWithFitting() const;
	std::vector<Agent> getPopulation();
	void runSimulation();
	Agent* getElitePredator();
	Agent& getBestEver();
private:
	std::vector<Agent> population;
	std::vector<Dimension> dimensionsRanges;
	Agent *elitePredator;
	int clusterAmount;
	int stepsNumber;
	Dataset pointsToCluster;
	int precision;
	Agent bestEver;
	const FittingFunctionBase& fittingFunction;
	void calculatePopulationFitting();
	void findElitePredator();
	double calculateAdaptiveParameter(int currentStep);
	void applyFADs(int currentStep, double FADs = 0.2);
};