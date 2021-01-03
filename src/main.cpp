#include <iostream>
#include <fstream>
#include <time.h>
#include "functions.h"
#include "Dataset.h"
#include "Logger.h"
#include "CelinskiHarabaszFittingFunction.h"
#include "DaviesBouldinIndex.h"
#include "SilhoutteIndex.h"
#include "GiniIndex.h"
#include "MPA.h"
#include "Kmeans.h"
#include "Cmeans.h"
#include "Dbscan.h"
#include "Hierarchical.h"
struct dataToLoadInfo
{
	std::string name;
	int labelIndex;
	double epsilon;
};

int main()
{
	srand(time(NULL));
	int precision = 3;
	int iterations = 1200; 
	std::vector<dataToLoadInfo> fileNames = {	
															{"wdbc", 0, 0.44}, //.good || db do jednego || https://archive.ics.uci.edu/ml/machine-learning-databases/breast-cancer-wisconsin/
															{"iris", 4, 0.14}, //.good
															{"glass", 9, 0.2}, //.bad
															{"fertility_Diagnosis", 9, 1}, //.bad
															{"cevical_cancer", 19, 1.7}, //50/50 || db do jednego || https://archive.ics.uci.edu/ml/datasets/Cervical+Cancer+Behavior+Risk
															{"parkinsons", 16, 2}, //.bad || dbscan wszystko daje do jednego
															{"forests", 0, 0.7}, //.50/50 || dbscan do jednego
															{"divorce", 54, 3.6}, //good https://archive.ics.uci.edu/ml/datasets/Divorce+Predictors+data+set
															{"wine", 0, 0.6} //.good	
															};
	
	for(auto& datasetInfo : fileNames)
	{
		std::cout << "=========== " <<  datasetInfo.name << " ===========\n";
		std::string inputFileName = "dataFiles/" + datasetInfo.name + ".data";
		Dataset data(inputFileName, datasetInfo.labelIndex);

		int clusterAmount = data.getPossibleLabels().size();
		data.normalize();
		data.shuffle();

		GiniIndex GIndex(data);
		CelinskiHarabaszFittingFunction CHIndex(data);
		DaviesBouldinIndex DBIndex(data);
		SilhoutteIndex SIndex(data);

		std::vector<std::pair<FittingFunctionBase&, std::string>> fittingFunctions = {
																						{CHIndex, "CHIndex"}, 
																						{DBIndex, "DBIndex"}, 
																						{SIndex, "SIndex"},
																						{GIndex, "GIndex"}
																					};

		Logger::instance().setDataInfo(data, datasetInfo.name, clusterAmount, precision);

		std::cout << "=> Hierarchical\n";
		Hierarchical hierarchical(clusterAmount, data);
		Dataset clusteredData = hierarchical.clusterData();
		Logger::instance().saveHierarchicalResultToFile(clusteredData);
		for(int fitting = 0; fitting < (int)fittingFunctions.size(); fitting++)
		{
			for(int i = 0; i < 10; i++)
			{
				std::cout << "====> " << i << " iteration with " <<  fittingFunctions[fitting].second << std::endl;
				
				data.shuffle();

				std::string stepFileName = datasetInfo.name + "_" + fittingFunctions[fitting].second + std::to_string(i);
				Logger::instance().setFileName(stepFileName);

				std::vector<Dimension> dimensions = functions::createDimensionsRanges(precision, data.getData()[0].actualData.size(), clusterAmount);
				
				std::cout << "==> Kmeans\n";
				Kmeans kmeans(clusterAmount, data, precision, iterations, fittingFunctions[fitting].first);
				std::vector<int> kmeansBestCoordinates = kmeans.clusterData();

				std::cout << "==> Cmeans\n";
				Cmeans cmeans(clusterAmount, data, precision, iterations, fittingFunctions[fitting].first);
				std::vector<int> cmeansBestCoordinates = cmeans.clusterData();

				std::cout << "==> MPA\n";
				MPA mpa(20, dimensions, clusterAmount, iterations, data, precision, fittingFunctions[fitting].first);
				mpa.runSimulation();
				std::vector<int> mpaBestCoordinates = mpa.getBestEver().getLocation();
				
				Logger::instance().setFileName(datasetInfo.name + "_" + fittingFunctions[fitting].second);
				Logger::instance().saveIndexesToFilesAtTheEnd({mpaBestCoordinates, kmeansBestCoordinates, cmeansBestCoordinates});
				

				double dimNumber = data.getData()[0].actualData.size();
				int minPoints = 2 * dimNumber;
				
				std::cout << "==> Dbscan\n";
				Dbscan dbscan(datasetInfo.epsilon, minPoints, data);
				clusteredData = dbscan.clusterData();
				Logger::instance().saveDbscanResultToFile(clusteredData);
			}
		}
	}
}