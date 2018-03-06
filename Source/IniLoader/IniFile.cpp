#include "IniFile.h"
#include <fstream>
#include <iostream>
#include <string>

IniFile::IniFile()
{
}


IniFile::~IniFile()
{
}

bool IniFile::LoadFile(const char * name)
{
	filename = name;
	std::ifstream fs;
	fs.open(name);
	if (fs.good())
	{
		while (!fs.eof())
		{
			std::string line;
			std::getline(fs, line);
			std::string ParamName = line.substr(0, line.find_first_of(" = "));
			std::string ParamVal = line.substr(line.find_first_of(" = ") + 3);
			parameters.insert(std::pair<const char*, const char*>(ParamName.c_str(), ParamVal.c_str()));
		}
	}
	fs.close();
	return false;
}
