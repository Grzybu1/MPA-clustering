#pragma once
#include <vector>
#include <Dataset.h>
struct Dimension
{
	int minValue;
	int maxValue;
};

enum Phases
{
	PHASE_1,
	PHASE_2_A,
	PHASE_2_B,
	PHASE_3
};

class Agent
{
public:
	Agent(std::vector<Dimension> dimensionsRanges);
	~Agent() = default;

	void calculateFitting(int clusterAmount, Dataset pointsToCluster, int precision);
	double getFitting()const;

	void writeLocation() const;
	std::vector<int> getLocation()const;
	void setLocation(std::vector<int> newLocation);
	
	void makeMove(Phases phase, Agent elitePredator, Dataset pointsToCluster, int clusterAmount, int precision, double CF = 0);

	Agent operator*(const std::vector<double>& rhs) const;
	Agent operator*(const Agent& rhs) const;
	Agent operator-(const Agent& rhs) const;
	Agent operator+(const Agent& rhs) const;
	Agent operator+(const std::vector<int>& rhs) const;
private:
	std::vector<int> location;
	double fitting;
	std::vector<int> calculateMove(Phases phase, Agent elitePredator);
	std::vector<Dimension> dimensionsRanges;
};