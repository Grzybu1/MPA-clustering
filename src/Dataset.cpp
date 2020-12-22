#include <Dataset.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <string> 
#include <fstream>
#include <sstream>
#include <algorithm>
#include <random>
#include "functions.h"
Dataset::Dataset(std::string dataFileName)
{
	std::fstream file;
	file.open(dataFileName, std::ios::in);
	if(!file.is_open())
		std::cout << "Nie można otworzyć pliku" << std::endl;
	std::vector<std::string> singleRow;
	std::string line, word;
	int iterator = 0;
	while(!file.eof())
	{
		singleRow.clear();

		std::getline(file, line);
		std::stringstream tmp(line);

		while(std::getline(tmp, word, ','))
		{
			singleRow.push_back(word);
		}
		if(singleRow.size()==0)
			continue;

		data.resize(data.size()+1);
		for(int i = 0; i < (int) singleRow.size()-1; i++)
		{
			data[iterator].actualData.push_back(std::stof(singleRow[i].c_str()));
		}
		
		data[iterator].label = labelToInt(singleRow.back().c_str());

		iterator++;
	}
}

int Dataset::labelToInt(std::string labelToEncode)
{
	for(auto& label : possibleLabels)
	{
		if(label.first == labelToEncode)
			return label.second;
	}
	possibleLabels.push_back({labelToEncode, possibleLabels.size()});
	return possibleLabels.back().second;
}

std::string Dataset::intToLabel(int labelToDecode)const
{
	for(auto& [label, labelInt] : possibleLabels)
	{
		if(labelInt == labelToDecode)
			return label;
	}
	return "NO LABEL";
}

void Dataset::printWithLabels() const
{
	for(int record = 0; record < (int) data.size(); record++)
	{
		for(auto& dataEntry : data[record].actualData)
			std::cout << dataEntry << ", ";
		std::cout << intToLabel(data[record].label) << std::endl;
	}
}

void Dataset::printWithIntLabels() const
{
	for(int record = 0; record < (int) data.size(); record++)
	{
		for(auto& dataEntry : data[record].actualData)
			std::cout << dataEntry << ", ";
		std::cout << data[record].label << std::endl;
	}
}

void Dataset::shuffle()
{
	auto randDevice = std::random_device{};
	auto randEngine = std::default_random_engine{randDevice()};
	std::shuffle(data.begin(), data.end(), randEngine);
}

std::vector<DataEntry> Dataset::getData()const
{
	return data;
}

void Dataset::setLabel(int index, int label)
{
	data[index].label = label;
}

void Dataset::clearLabels()
{
	for(auto& entry : data)
	{
		entry.label = -1;
	}
	possibleLabels.clear();
}

void Dataset::setPossibleLabels(std::vector<std::pair<std::string, int>> newPossibleLabels)
{
	possibleLabels = newPossibleLabels;
}

std::vector<int> Dataset::getLabelsDistribution()
{
	std::vector<int> result;
	result.resize(possibleLabels.size(), 0);
	for(auto& entry : data)
	{
		result[entry.label]++;
	}
	return result;
}

void Dataset::assignPointsToClusters(std::vector<std::vector<double>> centroids)
{
	for(auto& point : data)
	{
		int bestLabel = -1;
		double bestLabelDistance = -1;
		for(int centroid = 0; centroid < (int)centroids.size(); centroid++)
		{
			double distance = functions::calculateEuclideanDistanceBetweenVectors(centroids[centroid], point.actualData);
			if(bestLabel == -1 || bestLabelDistance > distance)
			{
				bestLabel = centroid;
				bestLabelDistance = distance;
			}
		}
		point.label = bestLabel;
	}
}

std::vector<std::pair<std::string, int>> Dataset::getPossibleLabels()const
{
	return possibleLabels;
}