#pragma once
#include <vector>

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
	Agent(int dimensionNumber);
	~Agent() = default;
	void initialize(std::vector<Dimension> dimensionsRanges);

	void calculateFitting(double(*fittingFunction)(std::vector<int>));
	double getFitting()const;

	void writeLocation() const;
	std::vector<int> getLocation()const;
	void setLocation(std::vector<int> newLocation);
	
	void makeMove(Phases phase, Agent elitePredator, double(*fittingFunction)(std::vector<int>), double CF = 0);

	Agent operator*(const std::vector<double>& rhs) const;
	Agent operator*(const Agent& rhs) const;
	Agent operator-(const Agent& rhs) const;
	Agent operator+(const Agent& rhs) const;
	Agent operator+(const std::vector<int>& rhs) const;
private:
	std::vector<int> location;
	double fitting;
	std::vector<int> calculateMove(Phases phase, Agent elitePredator);
};