
#pragma once
#include "Dataset.h"
#include <string>

class Logger{
private:
    Logger(); // Disallow instantiation outside of the class.
    Dataset dataToCluster;
    std::string dataName;
    int clusterAmount;
	int precision;
public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger &) = delete;
    Logger(Logger &&) = delete;
    Logger & operator=(Logger &&) = delete;

    static auto& instance(){
        static Logger test;
        return test;
    }
    void setFileName(std::string newName);
    void setDataInfo(Dataset& data, std::string name, int amount, int prec);
    void saveIndexesToFilesAtStep(std::vector<int> coordinates, int step);
    void saveIndexesToFilesAtTheEnd(std::vector<std::vector<int>> multipleCoordinates);
    void saveDbscanResultToFile(const Dataset& clusteredData);
    void saveHierarchicalResultToFile(const Dataset& clusteredData);
}; 