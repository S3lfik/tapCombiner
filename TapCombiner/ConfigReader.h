#pragma once
#include <pugixml.hpp>
#include <string>

class ConfigReader
{
public:
	ConfigReader();
	~ConfigReader();

	static ConfigReader* getInstance();

	inline const int getheaderSize() const { return m_headerSize; }
	inline const int getfooterSize() const { return m_footerSize; }
	inline const std::string getFileSeparator() const { return m_separator; }
	inline const std::string getTopInjectonText() const { return m_injectionText; };

private:
	void readValues();
	void replaceNewLine(std::string& input);

private:
	static ConfigReader *m_instance;

	pugi::xml_document m_doc;

	int m_headerSize;
	int m_footerSize;
	std::string m_separator;
	std::string m_injectionText;
};

