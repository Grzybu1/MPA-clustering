#include <vector>
namespace Distributions
{
	std::vector<double> randLevyVector(int vectorLength, double scale = 0.3, double exponent = 1.8);
	std::vector<double> randBrownianVector(int vectorLength);
	std::vector<double> randUniformVector(int vectorLength);
}