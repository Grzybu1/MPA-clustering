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
#include "DaviesBouldinIndex.h"
#include "SilhoutteIndex.h"
#include "Kmeans.h"
#include "Cmeans.h"
int main()
{
	srand(time(NULL));
	int precision = 3;
	int iterations = 3000; 
	std::vector<std::pair<std::string, int>> fileNames = {	
															{"wdbc", 0}, //good https://archive.ics.uci.edu/ml/machine-learning-databases/breast-cancer-wisconsin/
															{"iris", 4}, //good
															{"glass", 9}, //bad
															{"fertility_Diagnosis", 9}, //bad
															{"cevical_cancer", 19}, //50/50 https://archive.ics.uci.edu/ml/datasets/Cervical+Cancer+Behavior+Risk
															{"parkinsons", 16}, //bad
															{"forests", 0}, //50/50
															{"divorce", 54}, //good https://archive.ics.uci.edu/ml/datasets/Divorce+Predictors+data+set
															{"wine", 0} //good	
															};
	
	for(auto& datasetInfo : fileNames)
	{
		std::string inputFileName = "dataFiles/" + datasetInfo.first + ".data";
		Dataset data(inputFileName, datasetInfo.second);

		int clusterAmount = data.getPossibleLabels().size();
		data.normalize();

		for(int i = 0; i < 10; i++)
		{
			std::cout << "=========== " << i << " iteration of " + datasetInfo.first + " ===========\n";
			
			data.shuffle();

			std::string stepFileName = datasetInfo.first + std::to_string(i);;
			Logger::instance().setDataInfo(data, stepFileName, clusterAmount, precision);

			CelinskiHarabaszFittingFunction fittingFunction(data);
			std::vector<Dimension> dimensions = functions::createDimensionsRanges(precision, data.getData()[0].actualData.size(), clusterAmount);
			
			Kmeans kmeans(clusterAmount, data, precision, iterations, fittingFunction);
			std::vector<int> kmeansBestCoordinates = kmeans.clusterData();

			Cmeans cmeans(clusterAmount, data, precision, iterations, fittingFunction);
			std::vector<int> cmeansBestCoordinates = cmeans.clusterData();

			MPA mpa(20, dimensions, clusterAmount, iterations, data, precision, fittingFunction);
			mpa.runSimulation();
			std::vector<int> mpaBestCoordinates = mpa.getBestEver().getLocation();
			Logger::instance().setFileName(datasetInfo.first);
			Logger::instance().saveIndexesToFilesAtTheEnd({mpaBestCoordinates, kmeansBestCoordinates, cmeansBestCoordinates});
		}
	}
}