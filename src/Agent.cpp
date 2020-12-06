#include "Agent.h"
#include <random>
#include <iostream>
#include <math.h>
#include "Distributions.h"

Agent::Agent(int dimensionNumber)
{
	location.assign(dimensionNumber, 0);
}

void Agent::initialize(std::vector<Dimension> dimensionsRanges)
{
	for (int i = 0; i < (int)location.size(); i++)
	{
		int dimRange = dimensionsRanges[i].maxValue - dimensionsRanges[i].minValue;
		location[i] = rand() % dimRange + dimensionsRanges[i].minValue;
	}
}

void Agent::writeLocation() const
{
	for(auto dimension : location)
	{
		std::cout << dimension << " ";
	}
}

std::vector<int> Agent::getLocation() const
{
	return location;
}

double Agent::getFitting() const
{
	return fitting;
}

Agent Agent::operator*(const std::vector<double>& rhs) const
{
	Agent result(this->location.size());
	for(int i = 0; i < (int)this->location.size(); i++)
	{
		result.location[i] = round(this->location[i] * rhs[i]);
	}
	return result;
}

Agent Agent::operator*(const Agent& rhs) const
{
	Agent result(this->location.size());
	for(int i = 0; i < (int)this->location.size(); i++)
	{
		result.location[i] = this->location[i] * rhs.getLocation()[i];
	}
	return result;
}

Agent Agent::operator-(const Agent& rhs) const
{
	Agent result(this->location.size());
	for(int i = 0; i < (int)this->location.size(); i++)
	{
		result.location[i] = this->location[i] - rhs.getLocation()[i];
	}
	return result;
}

Agent Agent::operator+(const Agent& rhs) const
{
	Agent result(this->location.size());
	for(int i = 0; i < (int)this->location.size(); i++)
	{
		result.location[i] = this->location[i] + rhs.getLocation()[i];
	}
	return result;
}

Agent Agent::operator+(const std::vector<int>& rhs) const
{
	Agent result(this->location.size());
	for(int i = 0; i < (int)this->location.size(); i++)
	{
		result.location[i] = this->location[i] + rhs[i];
	}
	return result;
}

void Agent::setLocation(std::vector<int> newLocation)
{
	location = newLocation;	
}

std::vector<int> Agent::calculateMove(Phases phase, Agent elitePredator)
{
	Agent result(location.size());
	switch(phase)
	{
		case PHASE_1:
		{
			std::vector<double> RB = Distributions::randBrownianVector(result.getLocation().size());
			result = (elitePredator - (*this) * RB) * RB;
			break;
		}
		case PHASE_2_A:
		{
			std::vector<double> RL = Distributions::randLevyVector(result.getLocation().size());
			result = (elitePredator - (*this) * RL) * RL;
			break;
		}
		case PHASE_2_B:
		{
			std::vector<double> RB = Distributions::randBrownianVector(result.getLocation().size());
			result = (elitePredator * RB - (*this)) * RB;
			break;
		}
		case PHASE_3:
		{
			std::vector<double> RL = Distributions::randLevyVector(result.getLocation().size());
			result = (elitePredator * RL - (*this)) * RL;
			break;
		}
	}
	return result.getLocation();
}

void Agent::calculateFitting(double(*fittingFunction)(std::vector<int>))
{
	fitting = fittingFunction(location);
}

void Agent::makeMove(Phases phase, Agent elitePredator, double(*fittingFunction)(std::vector<int>), double CF)
{
	std::vector<int> calculatedStep = calculateMove(phase, elitePredator);
	int actualStep;

	std::vector<int> newLocation;
	Agent newAgent(location.size());

	if(phase == PHASE_1 || phase == PHASE_2_A)
	{
		std::vector<double> R = Distributions::randUniformVector(location.size());
		for(int i = 0; i < (int)location.size(); i++)
		{
			actualStep = round(R[i] * 0.5 * (double)calculatedStep[i]);
			newLocation.push_back(location[i] + actualStep);
		}
		newAgent.setLocation(newLocation);
	}
	else
	{
		for(int i = 0; i < (int)location.size(); i++)
		{
			actualStep = round(CF * 0.5 * (double)calculatedStep[i]);
			newLocation.push_back(elitePredator.getLocation()[i] + actualStep);
		}
		newAgent.setLocation(newLocation);
	}
	newAgent.calculateFitting(fittingFunction);
	if(newAgent.getFitting() > fitting)
	{
		location = newLocation;
		fitting = newAgent.getFitting();
	}
}