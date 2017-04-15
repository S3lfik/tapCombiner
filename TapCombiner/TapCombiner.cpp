#include "TapCombiner.h"
#include <algorithm>
#include "ConfigReader.h"


TapCombiner::TapCombiner()
{
}

TapCombiner::TapCombiner(const std::string& path)
	: m_path(path)
{
	ConfigReader reader;
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
	/** We dont't really need the value, just a type is enough */
	catch (const int& )
	{
		std::cerr << "Directory doesn't exist!\n"
			<< "Please enter the folder that contains *.tap files. E.g. \"D:/folder\"" << std::endl;
		return false;
	}
	catch (const bfs::filesystem_error& e)
	{
		std::cerr << e.what() << std::endl;
		return false;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return false;
	}
	catch (...)
	{
		std::cerr << "Something has gone really wrong" << std::endl;
		return false;
	}

	return true;
}

bool TapCombiner::readFiles()
{
	if (!bfs::exists(m_path) || !bfs::is_directory(m_path))
		return false;

	ConfigReader* configReader = ConfigReader::getInstance();

	std::ifstream tapStream;

	for (bfs::directory_entry& entry : bfs::directory_iterator(m_path))
	{
		TapFile tapFile;
		int lineNumber = 0;
		size_t minimalLineNumber = configReader->getTopFooterSize() +
			configReader->getBotFooterSize();

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

	std::sort(m_tapFiles.begin(), m_tapFiles.end(), compareFilesByNameDesc);

	return true;
}

void TapCombiner::combineFiles()
{
	ConfigReader* configReader = ConfigReader::getInstance();
	std::ofstream resultFile{ (m_path.string() + '/' + RESULT_FILE_NAME).c_str(), std::ios_base::trunc };

	// Config values
	int topFooterSize = configReader->getTopFooterSize();
	int botFooterSize = configReader->getBotFooterSize();
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
			for (int i = 0; i < topFooterSize; i++)
				resultFile << tapFile.fileMapping.at(i) << std::endl;

			resultFile << injectionText << std::endl;
		}
		else
			resultFile << separatorText << idx + 1<< std::endl;

		for (int lineNumber = topFooterSize; lineNumber < linesCount - botFooterSize; lineNumber++)
			resultFile << tapFile.fileMapping.at(lineNumber) << std::endl;;

		if (idx == filesCount - 1)
		{			
			for (int i = linesCount - botFooterSize; i < linesCount; i++)
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

	return false;
}
