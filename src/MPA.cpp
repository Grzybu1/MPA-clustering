#include "MPA.h"
#include <vector>
#include <math.h>
#include <iostream>
#include <Distributions.h>
MPA::MPA(int populationSize, std::vector<Dimension> dimensionsRanges, int clusterAmount, int stepsNumber, Dataset pointsToCluster, int precision, const FittingFunctionBase& fittingFunction)
:dimensionsRanges(dimensionsRanges), elitePredator(nullptr), clusterAmount(clusterAmount), stepsNumber(stepsNumber), pointsToCluster(pointsToCluster), precision(precision), bestEver(dimensionsRanges), fittingFunction(fittingFunction) 
{
	for(int i = 0; i < populationSize; i++)
	{
		population.push_back(Agent(dimensionsRanges));
	}
	calculatePopulationFitting();
	findElitePredator();
	bestEver = *elitePredator;
}

void MPA::calculatePopulationFitting()
{
	for(auto &agent : population)
	{
		agent.calculateFitting(clusterAmount, precision, fittingFunction);
	}
}

void MPA::writePopulationWithFitting() const
{
	for(const auto &agent : population)
	{
		if(&agent == elitePredator)
			std::cout << "Agent is elite predator\n";
		std::cout << "Agent coordinates: ";
		agent.writeLocation();
		std::cout << std::endl << "Fitting: " << agent.getFitting() << std::endl << std::endl;
	}
}

void MPA::findElitePredator()
{
	elitePredator = &population[0];
	for(int i = 1; i < (int)population.size(); i++)
	{
		if(population[i].getFitting() > elitePredator->getFitting())
			elitePredator = &population[i];
	}
}

std::vector<Agent> MPA::getPopulation()
{
	return population;
}

double MPA::calculateAdaptiveParameter(int currentStep)
{
	return pow(1 - (currentStep / stepsNumber), 2 * (currentStep / stepsNumber));
}

void MPA::applyFADs(int currentStep, double FADs)
{
	for(auto& agent : population)
	{
		int dimensionsNumber = agent.getLocation().size();
		double r = Distributions::randUniformVector(1)[0];
		std::vector<int> newLocation;
		newLocation.resize(agent.getLocation().size(), 0);
		if(r <= FADs)
		{
			std::vector<int> U;
			for(int i = 0; i < dimensionsNumber; i++)
			{
				U.push_back(rand()/RAND_MAX <= FADs ? 0 : 1);
			}

			std::vector<double> R = Distributions::randUniformVector(dimensionsNumber);
			double CF = calculateAdaptiveParameter(currentStep);
			for(int i = 0; i < dimensionsNumber; i++)
			{
				int dimWidth = dimensionsRanges[i].maxValue - dimensionsRanges[i].minValue;
				double change = CF * (dimensionsRanges[i].minValue + R[i] * dimWidth);
				newLocation[i] = agent.getLocation()[i] + round(change) * U[i];
			}
			agent.setLocation(newLocation);
		}
		else
		{
			int r1 = rand() % population.size();
			int r2 = rand() % population.size();
			while(r1 == r2)
				r2 = rand() % population.size();

			for(int i = 0; i < dimensionsNumber; i++)
			{
				int changeVector = population[r2].getLocation()[i] - population[r1].getLocation()[i];
				int locationChange = (FADs * (1 - r) + r) * changeVector;
				newLocation[i] = agent.getLocation()[i] + locationChange;
			}
			agent.setLocation(newLocation);
		}
	}
}

void MPA::runSimulation()
{
	int firstPhaseMaxStep = stepsNumber / 3;
	int secondPhaseMaxStep = firstPhaseMaxStep * 2;
	for(int step = 0; step < stepsNumber; step++)
	{
		if(step < firstPhaseMaxStep)
		{
			for(auto& agent : population)
			{
				agent.makeMove(PHASE_1, fittingFunction, *elitePredator, clusterAmount, precision);
			}
		}
		else if(step < secondPhaseMaxStep)
		{
			for(int i = 0; i < (int)population.size(); i++)
			{
				double CF = calculateAdaptiveParameter(step);
				if(i % 2)
					population[i].makeMove(PHASE_2_A, fittingFunction, *elitePredator, clusterAmount, precision);
				else
					population[i].makeMove(PHASE_2_B, fittingFunction, *elitePredator, clusterAmount, precision, CF);
			}
		}
		else
		{
			double CF = calculateAdaptiveParameter(step);
			for(auto& agent : population)
			{
				agent.makeMove(PHASE_3, fittingFunction, *elitePredator, clusterAmount, precision, CF);
			}
		}
		findElitePredator();
		applyFADs(step);

		calculatePopulationFitting();
		findElitePredator();
		if(elitePredator -> getFitting() > bestEver.getFitting())
			bestEver = *elitePredator;

		if(step % 100 == 0)
		{
			std::cout << "Step: " << step <<"\nElite coordinates: ";
			elitePredator->writeLocation();
			std::cout << "\nElite fitting: " << elitePredator->getFitting() << std::endl;
			std::cout << "Best ever fitting: " << bestEver.getFitting() << std::endl << std::endl;
		}
	}
}

Agent* MPA::getElitePredator()
{
	return elitePredator;
}

Agent& MPA::getBestEver()
{
	return bestEver;
}