#include "Logger.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "functions.h"
#include "DaviesBouldinIndex.h"
#include "SilhoutteIndex.h"
#include "GiniIndex.h"
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
	std::string title = "#step,index\n";

	std::string outputFileName = "outputFiles/indexesByStep/" + dataName + "RandIndex.csv";
	file.open(outputFileName, std::ios::app);
	double rand = functions::calculateRandIndex(dataToCluster, resultSet);
	
	if(step == 0)
		file << title;
	
	file << step << "," << rand << std::endl;
	file.close();

	outputFileName = "outputFiles/indexesByStep/" + dataName + "GiniIndex.csv";
	file.open(outputFileName, std::ios::app);
	GiniIndex gini(dataToCluster);
	double GIndex = gini.calculateIndex(resultSet.getLabelsDistribution(), resultSet.getData().size());
	
	if(step == 0)
		file << title;
	
	file << step << "," << GIndex << std::endl;
	file.close();

	outputFileName = "outputFiles/indexesByStep/" + dataName + "DaviesBouldinIndex.csv";
	file.open(outputFileName, std::ios::app);
	DaviesBouldinIndex davies(dataToCluster);
	double DBIndex = davies.calculateIndex(coordinates, clusterAmount, precision);
	
	if(step == 0)
		file << title;
	
	file << step << "," << DBIndex << std::endl;
	file.close();

	outputFileName = "outputFiles/indexesByStep/" + dataName + "SilhoutteIndex.csv";
	file.open(outputFileName, std::ios::app);
	SilhoutteIndex silhoutte(dataToCluster);
	double SIndex = silhoutte.calculateIndex(coordinates, clusterAmount, precision);
	
	if(step == 0)
		file << title;
	
	file << step << "," << SIndex << std::endl;
	file.close();

	outputFileName = "outputFiles/indexesByStep/" + dataName + "CelinskiHarabaszIndex.csv";
	file.open(outputFileName, std::ios::app);
	CelinskiHarabaszFittingFunction celinski(dataToCluster);
	double CHIndex = celinski.calculateIndex(coordinates, clusterAmount, precision);
	
	if(step == 0)
		file << title;
	
	file << step << "," << CHIndex << std::endl;
	file.close();
}

void Logger::saveIndexesToFilesAtTheEnd(std::vector<std::vector<int>> multipleCoordinates)
{
	GiniIndex gini(dataToCluster);
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
		giniResults.push_back(gini.calculateIndex(resultSet.getLabelsDistribution(), resultSet.getData().size()));
		daviesResults.push_back(davies.calculateIndex(coordinates, clusterAmount, precision));
		silhoutteResults.push_back(silhoutte.calculateIndex(coordinates, clusterAmount, precision));
		celinskiResults.push_back(celinski.calculateIndex(coordinates, clusterAmount, precision));
	// Rand Index: More -> better
	// Davis Bouldin Index: Less -> better
	// Silhoutte Index: More -> better
	// Gini Index: More -> better
	// Celinski Harabasz Index: More -> better
	}
	std::string title = "#MPA, Kmeans, Cmeans\n";
	std::fstream file;

	std::string outputFileName = "outputFiles/overallResults/" + dataName + "RandIndex.csv";
	file.open(outputFileName, std::ios::app);
	file << title;
	for(int i = 0; i < (int) multipleCoordinates.size()-1; i++)
	{
		file << randResults[i] << ",";
	}
	file << randResults[multipleCoordinates.size()-1] << std::endl;
	file.close();

	outputFileName = "outputFiles/overallResults/" + dataName + "GiniIndex.csv";
	file.open(outputFileName, std::ios::app);
	file << title;
	for(int i = 0; i < (int) multipleCoordinates.size()-1; i++)
	{
		file << giniResults[i] << ",";
	}
	file << giniResults[multipleCoordinates.size()-1] << std::endl;
	file.close();

	outputFileName = "outputFiles/overallResults/" + dataName + "DaviesBouldinIndex.csv";
	file.open(outputFileName, std::ios::app);
	file << title;
	for(int i = 0; i < (int) multipleCoordinates.size()-1; i++)
	{
		file << daviesResults[i] << ",";
	}
	file << daviesResults[multipleCoordinates.size()-1] << std::endl;
	file.close();

	outputFileName = "outputFiles/overallResults/" + dataName + "SilhoutteIndex.csv";
	file.open(outputFileName, std::ios::app);
	file << title;
	for(int i = 0; i < (int) multipleCoordinates.size()-1; i++)
	{
		file << silhoutteResults[i] << ",";
	}
	file << silhoutteResults[multipleCoordinates.size()-1] << std::endl;
	file.close();

	outputFileName = "outputFiles/overallResults/" + dataName + "CelinskiHarabaszIndex.csv";
	file.open(outputFileName, std::ios::app);
	file << title;
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

void Logger::saveDbscanResultToFile(const Dataset& clusteredData)
{
	std::fstream file;
	std::string outputFileName = "outputFiles/dbscanResults/" + dataName + ".csv";
	file.open(outputFileName, std::ios::app);

	double numOfPoints = clusteredData.getData().size();
	int clusterNum = 0;
	int clusteredPoints = 0;
	for(auto& label : clusteredData.getPossibleLabels())
	{
		if(label.second >= 0)
			clusterNum++;
	}
	for(auto& point : clusteredData.getData())
	{
		if(point.label >= 0)
			clusteredPoints++;
	}
	file << "#clusters, noises, rand index" << std::endl;
	file << clusterNum << "," << numOfPoints - clusteredPoints << ",";
	file << functions::calculateRandIndex(clusteredData, dataToCluster) << std::endl; 
}

void Logger::saveHierarchicalResultToFile(const Dataset& clusteredData)
{
	std::fstream file;
	std::string outputFileName = "outputFiles/hierarchicalResults/" + dataName + ".csv";
	file.open(outputFileName, std::ios::app);

	file << "rand index" << std::endl;
	file << functions::calculateRandIndex(clusteredData, dataToCluster) << std::endl; 
}