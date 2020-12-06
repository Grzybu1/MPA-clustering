#include <iostream>
#include <fstream>
#include <time.h>
#include "MPA.h"
#include <math.h>

double testFittingFunction(std::vector<int> location)
{
	double x = (double)location[0] / 1000.0;
	double y = (double)location[1] / 1000.0;
	double r = sqrt(x*x + y*y);
	return sin(r)/r;
}


int main()
{
	srand(time(NULL));
	 MPA testObj(2, {{-10000,10000}, {-10000,10000}}, testFittingFunction, 10);
	testObj.initializePopulation();
	testObj.writePopulationWithFitting();
	testObj.runSimulation();
	
}