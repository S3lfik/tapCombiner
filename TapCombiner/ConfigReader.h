#pragma once
#include <pugixml.hpp>
#include <string>

class ConfigReader
{
public:
	ConfigReader();
	~ConfigReader();

	static ConfigReader* getInstance();

	inline const int getTopFooterSize() const { return m_topFooterSize; }
	inline const int getBotFooterSize() const { return m_botFooterSize; }
	inline const std::string getFileSeparator() const { return m_separator; }
	inline const std::string getTopInjectonText() const { return m_injectionText; };

private:
	void readValues();
	void replaceNewLine(std::string& input);

private:
	static ConfigReader *m_instance;

	pugi::xml_document m_doc;

	int m_topFooterSize;
	int m_botFooterSize;
	std::string m_separator;
	std::string m_injectionText;
};

