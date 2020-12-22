#include "Logger.h"
#include <iostream>
#include <sstream>
#include <fstream>
void Logger::saveDataSetToJson(Dataset dataToSave, std::string fileName)
{
	std::vector<DataEntry> data = dataToSave.getData();
	std::fstream file;
	file.open(fileName, std::ios::out);
	std::stringstream output;
	output << "{\n\t\"points:\": [" << std::endl;

	for(auto& dataEntry : data)
	{
		output << "\t\t{\"coordinates\": [";
		for(auto& coordinate : dataEntry.actualData)
		{
			output << "\"" << coordinate <<"\"";
			if(&coordinate != &dataEntry.actualData.back())
				output << ", ";
		}
		output << "], \"label\": \"" << dataEntry.label << "\"}";
		if(&dataEntry != &data.back())
			output << ", ";
		output << std::endl;
	}
	output << "\t]\n}";
	file << output.rdbuf();
}