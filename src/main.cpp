#include <iostream>
#include <fstream>
#include <time.h>
#include "functions.h"
#include "MPA.h"
#include "Dataset.h"
#include <math.h>
#include "Logger.h"
#include "FittingFunctionSimple.h"
#include "CelinskiHarabaszFittingFunction.h"


int main()
{
	srand(time(NULL));
	Dataset data("dataFiles/iris.data");
	double avgSimilarity = 0;
	for(int i = 0; i < 10; i++)
	{
		data.shuffle();
		CelinskiHarabaszFittingFunction fittingFunction(data);
		std::vector<Dimension> dimensions = functions::createDimensionsRanges({{4,8}, {2,5},{1,7},{0,3}}, 2, 3);
		MPA testObj(20, dimensions, 3, 5000, data, 2, fittingFunction);
		// testObj.writePopulationWithFitting();
		testObj.runSimulation();
		std::vector<std::vector<double>> centroids = functions::separateCoordinates(testObj.getBestEver().getLocation(), 3, 2);
		Dataset resultSet = data;
		resultSet.clearLabels();
		resultSet.setPossibleLabels(data.getPossibleLabels());
		resultSet.assignPointsToClusters(centroids);
		for(auto& num : resultSet.getLabelsDistribution())
		{
			std::cout << num << "\t";
		}
		std::cout << "\nSimilarity: " << functions::compareLabeling(resultSet, data)*100 << "%\n";
		avgSimilarity += functions::compareLabeling(resultSet, data)*100;
	}
	avgSimilarity /= 10;
	std::cout << "Avg result: " << avgSimilarity << std::endl;
}