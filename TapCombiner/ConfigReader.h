#pragma once
#include <pugixml.hpp>
#include <string>

class ConfigReader
{
public:
	ConfigReader();
	ConfigReader(const std::string& configFileName);
	~ConfigReader();
};

