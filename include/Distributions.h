#include <vector>
namespace Distributions
{
	std::vector<double> randLevyVector(int vectorLength, double scale = 1, double exponent = 1.5);
	std::vector<double> randBrownianVector(int vectorLength);
	std::vector<double> randUniformVector(int vectorLength);
}