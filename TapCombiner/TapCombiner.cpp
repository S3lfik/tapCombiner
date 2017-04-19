#include "TapCombiner.h"
#include <algorithm>
#include "ConfigReader.h"


TapCombiner::TapCombiner()
{
}

TapCombiner::TapCombiner(const std::string& path)
	: m_path(path)
{
}

TapCombiner::~TapCombiner()
{
}

bool TapCombiner::exec()
{
	try
	{
		if (!readFiles())
			return false;

		combineFiles();
	}

	catch (const bfs::filesystem_error& e)
	{
		std::cerr << e.what() << std::endl;
		return false;
	}

	return true;
}

bool TapCombiner::readFiles()
{
	if (!bfs::exists(m_path) || !bfs::is_directory(m_path))
	{
		std::cerr << "Tap folder path error. Check directory and try again!\n"
			<< "For example: \"D:\\>TapCombiner taps\"" << std::endl;
		return false;
	}

	ConfigReader* configReader = ConfigReader::getInstance();

	std::ifstream tapStream;

	for (bfs::directory_entry& entry : bfs::directory_iterator(m_path))
	{
		TapFile tapFile;
		int lineNumber = 0;
		size_t minimalLineNumber = configReader->getheaderSize() +
			configReader->getfooterSize();

		tapFile.name = entry.path().filename().string();

		if (std::string::npos == tapFile.name.find(FILE_TYPE))
			continue;

		tapStream.open("taps/\\" + tapFile.name, std::ifstream::in);

		if (tapStream.is_open())
		{
			std::string bufferStr{ "" };

			for (int lineIdx = 0; !tapStream.eof(); ++lineIdx)
			{
				std::getline(tapStream, bufferStr);

				if (bufferStr.empty())
				{
					lineIdx--;
					continue;
				}

				tapFile.fileMapping.insert(
					std::pair<int, std::string>(lineIdx, bufferStr));

			}

			tapStream.close();

			if (tapFile.fileMapping.size() < minimalLineNumber)
			{
				std::cerr << "TapFile " << tapFile.name << " has invalid content!" << std::endl;
				return false;
			}
		}
		m_tapFiles.push_back(tapFile);
	}

	if (m_tapFiles.empty())
	{
		std::cerr << "Tap files reading error!" << std::endl;
		return false;
	}

	std::sort(m_tapFiles.begin(), m_tapFiles.end(), compareFilesByNameDesc);

	std::cout << "Tap files were read successfully!" << std::endl;
	return true;
}

void TapCombiner::combineFiles()
{
	ConfigReader* configReader = ConfigReader::getInstance();
	std::ofstream resultFile{ (m_path.string() + '/' + RESULT_FILE_NAME).c_str(), std::ios_base::trunc };

	// Config values
	int headerSize = configReader->getheaderSize();
	int footerSize = configReader->getfooterSize();
	std::string injectionText = configReader->getTopInjectonText();
	std::string separatorText = configReader->getFileSeparator();
	//
	int filesCount = m_tapFiles.size();

	if (!resultFile.is_open())
		return;

	for (int idx = 0; idx < filesCount; idx++)
	{
		TapFile tapFile = m_tapFiles.at(idx);
		int linesCount = tapFile.fileMapping.size();

		/** If first file - put top footer & top injection*/
		if (idx == 0)
		{
			for (int i = 0; i < headerSize; i++)
				resultFile << tapFile.fileMapping.at(i) << std::endl;

			resultFile << injectionText << std::endl;
		}
		else
			resultFile << separatorText << idx + 1<< std::endl;

		for (int lineNumber = headerSize; lineNumber < linesCount - footerSize; lineNumber++)
			resultFile << tapFile.fileMapping.at(lineNumber) << std::endl;;

		if (idx == filesCount - 1)
		{			
			for (int i = linesCount - footerSize; i < linesCount; i++)
				resultFile << tapFile.fileMapping.at(i) << std::endl;

		}
	}

	resultFile.close();
}

bool TapCombiner::compareFilesByNameDesc(TapFile lhs, TapFile rhs)
{
	std::string lhsName = lhs.name;
	std::string rhsName = rhs.name;

	lhsName = lhsName.substr(0, lhsName.length() - FILE_TYPE.length());
	rhsName = rhsName.substr(0, rhsName.length() - FILE_TYPE.length());

	return std::atoi(lhsName.c_str()) < std::atoi(rhsName.c_str());
}
