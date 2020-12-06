#include <Distributions.h>
#include <algorithm>
#include <vector>
#include <random>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

namespace Distributions
{
		std::vector<double> randBrownianVector(int vectorLength)
	{
		std::vector<double> result;
		gsl_rng * generator =  gsl_rng_alloc(gsl_rng_default);
		gsl_rng_set(generator, rand());
		for(int i = 0; i < vectorLength; i++)
		{
			result.push_back(gsl_ran_gaussian(generator ,1.0));
		}
		gsl_rng_free(generator);
		return result;
	}

	std::vector<double> randLevyVector(int vectorLength, double scale, double exponent)
	{
		std::vector<double> result;
		gsl_rng * generator =  gsl_rng_alloc(gsl_rng_default);
		gsl_rng_set(generator, rand());
		for(int i = 0; i < vectorLength; i++)
		{
			result.push_back(gsl_ran_levy(generator, scale, exponent));
		}
		gsl_rng_free(generator);
		return result;
	}

	std::vector<double> randUniformVector(int vectorLength)
	{
		std::vector<double> result;
		for(int i = 0; i < vectorLength; i++)
		{
			result.push_back((double)rand()/RAND_MAX);
		}
		return result;
	}
}