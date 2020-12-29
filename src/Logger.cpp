#include "Logger.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "functions.h"
#include "DaviesBouldinIndex.h"
#include "SilhoutteIndex.h"
#include "CelinskiHarabaszFittingFunction.h"
void Logger::setDataInfo(Dataset& data, std::string name, int amount, int prec)
{
	dataToCluster = data;
	dataName = name;
	clusterAmount = amount;
	precision = prec;
}

void Logger::saveIndexesToFilesAtStep(std::vector<int> coordinates, int step)
{
	std::vector<std::vector<double>> centroids = functions::separateCoordinates(coordinates, clusterAmount, precision);
	Dataset resultSet = dataToCluster;
	resultSet.clearLabels();
	resultSet.setPossibleLabels(dataToCluster.getPossibleLabels());
	resultSet.assignPointsToClusters(centroids);

	std::fstream file;

	std::string outputFileName = "outputFiles/indexesByStep/" + dataName + "RandIndex.csv";
	file.open(outputFileName, std::ios::app);
	double rand = functions::calculateRandIndex(dataToCluster, resultSet);
	file << step << "," << rand << std::endl;
	file.close();

	outputFileName = "outputFiles/indexesByStep/" + dataName + "GiniIndex.csv";
	file.open(outputFileName, std::ios::app);
	double GIndex = functions::calculateGiniIndexForSingleSplit(resultSet.getLabelsDistribution(), resultSet.getData().size());
	file << step << "," << GIndex << std::endl;
	file.close();

	outputFileName = "outputFiles/indexesByStep/" + dataName + "DaviesBouldinIndex.csv";
	file.open(outputFileName, std::ios::app);
	DaviesBouldinIndex davies(dataToCluster);
	double DBIndex = davies.calculateFitting(coordinates, clusterAmount, precision);
	file << step << "," << DBIndex << std::endl;
	file.close();

	outputFileName = "outputFiles/indexesByStep/" + dataName + "SilhoutteIndex.csv";
	file.open(outputFileName, std::ios::app);
	SilhoutteIndex silhoutte(dataToCluster);
	double SIndex = silhoutte.calculateFitting(coordinates, clusterAmount, precision);
	file << step << "," << SIndex << std::endl;
	file.close();

	outputFileName = "outputFiles/indexesByStep/" + dataName + "CelinskiHarabaszIndex.csv";
	file.open(outputFileName, std::ios::app);
	CelinskiHarabaszFittingFunction celinski(dataToCluster);
	double CHIndex = celinski.calculateIndex(coordinates, clusterAmount, precision);
	file << step << "," << CHIndex << std::endl;
	file.close();
}

void Logger::saveIndexesToFilesAtTheEnd(std::vector<std::vector<int>> multipleCoordinates)
{

	DaviesBouldinIndex davies(dataToCluster);
	SilhoutteIndex silhoutte(dataToCluster);
	CelinskiHarabaszFittingFunction celinski(dataToCluster);
	std::vector<double> daviesResults;
	std::vector<double> silhoutteResults;
	std::vector<double> celinskiResults;
	std::vector<double> randResults;
	std::vector<double> giniResults;
	for(auto& coordinates : multipleCoordinates)
	{
		std::vector<std::vector<double>> centroids = functions::separateCoordinates(coordinates, clusterAmount, precision);
		Dataset resultSet = dataToCluster;
		resultSet.clearLabels();
		resultSet.setPossibleLabels(dataToCluster.getPossibleLabels());
		resultSet.assignPointsToClusters(centroids);

		randResults.push_back(functions::calculateRandIndex(dataToCluster, resultSet));
		giniResults.push_back(functions::calculateGiniIndexForSingleSplit(resultSet.getLabelsDistribution(), resultSet.getData().size()));
		daviesResults.push_back(davies.calculateFitting(coordinates, clusterAmount, precision));
		silhoutteResults.push_back(silhoutte.calculateFitting(coordinates, clusterAmount, precision));
		celinskiResults.push_back(celinski.calculateIndex(coordinates, clusterAmount, precision));
	// Rand Index: More -> better
	// Davis Bouldin Index: Less -> better
	// Silhoutte Index: More -> better
	// Gini Index: More -> better
	// Celinski Harabasz Index: More -> better
	}
	std::fstream file;

	std::string outputFileName = "outputFiles/overallResults/" + dataName + "RandIndex.csv";
	file.open(outputFileName, std::ios::app);
	for(int i = 0; i < (int) multipleCoordinates.size()-1; i++)
	{
		file << randResults[i] << ",";
	}
	file << randResults[multipleCoordinates.size()-1] << std::endl;
	file.close();

	outputFileName = "outputFiles/overallResults/" + dataName + "GiniIndex.csv";
	file.open(outputFileName, std::ios::app);
	for(int i = 0; i < (int) multipleCoordinates.size()-1; i++)
	{
		file << giniResults[i] << ",";
	}
	file << giniResults[multipleCoordinates.size()-1] << std::endl;
	file.close();

	outputFileName = "outputFiles/overallResults/" + dataName + "DaviesBouldinIndex.csv";
	file.open(outputFileName, std::ios::app);
	for(int i = 0; i < (int) multipleCoordinates.size()-1; i++)
	{
		file << daviesResults[i] << ",";
	}
	file << daviesResults[multipleCoordinates.size()-1] << std::endl;
	file.close();

	outputFileName = "outputFiles/overallResults/" + dataName + "SilhoutteIndex.csv";
	file.open(outputFileName, std::ios::app);
	for(int i = 0; i < (int) multipleCoordinates.size()-1; i++)
	{
		file << silhoutteResults[i] << ",";
	}
	file << silhoutteResults[multipleCoordinates.size()-1] << std::endl;
	file.close();

	outputFileName = "outputFiles/overallResults/" + dataName + "CelinskiHarabaszIndex.csv";
	file.open(outputFileName, std::ios::app);
	for(int i = 0; i < (int) multipleCoordinates.size()-1; i++)
	{
		file << celinskiResults[i] << ",";
	}
	file << celinskiResults[multipleCoordinates.size()-1] << std::endl;
	file.close();
}

Logger::Logger()
:dataToCluster(), dataName(""), clusterAmount(0), precision(0)
{}

void Logger::setFileName(std::string newName)
{
	dataName = newName;
}