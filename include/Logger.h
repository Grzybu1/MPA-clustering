/*****************************************
******************************************
******	Potencjalnie niepotrzebne	******
******	Rozważyć usunięcie			******
******************************************
*****************************************/
#pragma once
#include "Dataset.h"
#include <string>
class Logger
{
public:
	Logger();
	~Logger();
	static void saveDataSetToJson(Dataset dataToSave, std::string fileName);

};