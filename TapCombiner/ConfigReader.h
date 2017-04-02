#pragma once
#include <pugixml.hpp>
#include <string>

class ConfigReader
{
public:
	ConfigReader();
	~ConfigReader();

private:
	void readValues();
	void replaceNewLine(std::string& input);

private:
	pugi::xml_document m_doc;

	int m_topFooterSize;
	int m_botFooterSize;
	std::string m_separator;
	std::string m_injectionText;
};

