#pragma once
#include <vector>
#include <utility>
#include "Agent.h"

class MPA
{
public:
	MPA(int populationSize, std::vector<Dimension> dimensionsRanges, double(*fittingFunction)(std::vector<int>), int stepsNumber);
	~MPA() = default;
	void initializePopulation();
	void writePopulationWithFitting() const;
	std::vector<Agent> getPopulation();
	void runSimulation();
private:
	double (*fittingFunction)(std::vector<int>);
	std::vector<Agent> population;
	std::vector<Dimension> dimensionsRanges;
	Agent *elitePredator;
	int stepsNumber;
	
	void calculatePopulationFitting();
	void findElitePredator();
	double calculateAdaptiveParameter(int currentStep);
	void applyFADs(int currentStep, double FADs = 0.2);
};