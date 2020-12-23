#pragma once
#include <vector>
#include <string> 

struct DataEntry
{
	std::vector<double> actualData;
	int label;
};

class Dataset
{
public:
	Dataset(std::string dataFileName, int labelPosition);
	~Dataset() = default;
	int labelToInt(std::string labelToEncode);
	std::string intToLabel(int labelToDecode) const;
	void printWithLabels() const;
	void printWithIntLabels() const;
	void shuffle();
	void normalize();
	std::vector<DataEntry> getData()const;
	void setLabel(int index, int label);
	void clearLabels();
	void setPossibleLabels(std::vector<std::pair<std::string, int>> newPossibleLabels);
	std::vector<int> getLabelsDistribution();
	void assignPointsToClusters(std::vector<std::vector<double>> centroids);
	std::vector<std::pair<std::string, int>> getPossibleLabels()const;
private:
	std::vector<DataEntry> data;
	std::vector<std::pair<std::string, int>> possibleLabels;
};