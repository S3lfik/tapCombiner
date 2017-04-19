#include "ConfigReader.h"
#include <iostream>
#include <algorithm>
#include <cassert>

const std::string CONFIG_FILE_NAME = "config.xml";

ConfigReader* ConfigReader::m_instance = nullptr;

ConfigReader::ConfigReader()
	: m_headerSize(0)
	, m_footerSize(0)
	, m_separator{""}
	, m_injectionText{""}
{
	pugi::xml_parse_result result = m_doc.load_file(CONFIG_FILE_NAME.c_str(), pugi::parse_minimal);
	std::cout << "Config load result: " << result.description() << std::endl;

	readValues();
}


ConfigReader::~ConfigReader()
{
}

ConfigReader * ConfigReader::getInstance()
{
	if (!m_instance)
		m_instance = new ConfigReader();

	return m_instance;
}

void ConfigReader::readValues()
{
	pugi::xml_node root = m_doc.document_element();
	pugi::xml_attribute atribute;
	auto nodes = root.children();
	for (auto itr = nodes.begin(); itr != nodes.end(); ++itr)
	{
		atribute = itr->first_attribute();
		const std::string stringname{ itr->name() };

		if (!stringname.compare("headerSize"))
		{
			m_headerSize = atribute.as_int();
		}
		else if (!stringname.compare("footerSize"))
		{
			m_footerSize = atribute.as_int();
		}
		else if (!stringname.compare("fileSeparator"))
		{
			m_separator = atribute.as_string();
			replaceNewLine(m_separator);
		}
		else if (!stringname.compare("topInsertionText"))
		{
			m_injectionText = atribute.as_string();
			replaceNewLine(m_injectionText);
		}
	}
}

void ConfigReader::replaceNewLine(std::string & input)
{
	std::string::size_type pos = 0;
	//Replace newline and cariage return symbols in line
	while ((pos = input.find("\\n", pos)) != std::string::npos)
	{
		input.replace(pos, 2, "\n");
	}
	while ((pos = input.find("\\r", pos)) != std::string::npos)
	{
		input.replace(pos, 2, "\r");
	}
	//Remove tabulations and whitespaces
	input.erase(std::remove(input.begin(), input.end(), '\t'), input.end());
	input.erase(std::remove(input.begin(), input.end(), ' '), input.end());
}
