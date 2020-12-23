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
	int precision = 3;
	// Dataset data("dataFiles/glass.data", 9);//bad
	Dataset data("dataFiles/wdbc.data", 0);//Good Anakin, good.
	// Dataset data("dataFiles/parkinsons.data", 16); //bad
	// Dataset data("dataFiles/forests.data", 0); //bad
	// Dataset data("dataFiles/divorce.data", 54); //good
	// Dataset data("dataFiles/iris.data", 4); //good
	// Dataset data("dataFiles/wine.data", 0); //good
	int clusterAmount = data.getPossibleLabels().size();
	data.normalize();
	// data.printWithLabels();
	double avgSimilarity = 0;
	for(int i = 0; i < 10; i++)
	{
		data.shuffle();
		CelinskiHarabaszFittingFunction fittingFunction(data);
		std::vector<Dimension> dimensions = functions::createDimensionsRanges(precision, data.getData()[0].actualData.size(), clusterAmount);
		MPA testObj(20, dimensions, clusterAmount, 3000, data, precision, fittingFunction);
		// testObj.writePopulationWithFitting();
		testObj.runSimulation();
		std::vector<std::vector<double>> centroids = functions::separateCoordinates(testObj.getBestEver().getLocation(), clusterAmount, precision);
		Dataset resultSet = data;
		resultSet.clearLabels();
		resultSet.setPossibleLabels(data.getPossibleLabels());
		resultSet.assignPointsToClusters(centroids);
		// for(auto& [label, value] : resultSet.getPossibleLabels())
		// {
		// 	std::cout << label << " is " << value << "\n";
		// }
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