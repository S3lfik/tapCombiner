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
		initFileNames();

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

void TapCombiner::initFileNames()
{
	if (bfs::exists(m_path))
	{
		if (bfs::is_directory(m_path))
		{
			for (bfs::directory_entry& entry : bfs::directory_iterator(m_path))
			{
				m_fileNames.push_back(entry.path().filename().string());
			}
			
			std::sort(m_fileNames.begin(), m_fileNames.end(), compareFileNamesDesc);

			for (auto& path : m_fileNames)
			{
				std::cout << path << std::endl;
			}
		}
		else
			throw 0;
	}
	else
		throw 0;
}

void TapCombiner::combineFiles()
{
	/** Create resulting file stream*/
	std::ofstream resultFile{ (m_path.string() + RESULT_FILE_NAME).c_str() };
	bool bFooterMade = false;
	int startLine = 3;
	int endLine = 0;
	int fileIndx = 1;
	std::string bufferLine{ "" };
	std::string fileHead{ "" };
	std::string additionalHead{ "11111\n22222\n33333\n" };
	bool bHasTitle = false;

	if (!resultFile.good())
		return;

	for (auto& filename : m_fileNames)
	{

		std::ifstream tapStream;;
		tapStream.open("taps/\\" + filename, std::ifstream::in);

		if (tapStream.is_open())
		{
			std::cout << "File: " << filename << " has " << getLinesInStream(tapStream) << " lines" << std::endl;
			if(filename != m_fileNames.front())
				resultFile << "start#" << fileIndx << std::endl;

			endLine = getLinesInStream(tapStream) - 3;
			for (int lineIdx = 0; !tapStream.eof(); ++lineIdx)
			{
				if (lineIdx == endLine && filename.compare(m_fileNames.back()))
				{
					break;
				}
				std::getline(tapStream, bufferLine);
				// at first, write lines into title buffer
				if (lineIdx < startLine)
				{
					fileHead.append(bufferLine + "\n");
				}
				// and then all the other lines with title at the head if none were writen already
				else 
				{					
					if (!bHasTitle)
					{
						resultFile << fileHead;
						resultFile << additionalHead;
						bHasTitle = true;
					}
					//std::getline(tapStream, bufferLine);
					resultFile << bufferLine << std::endl;
				}		
				///
				// if idx < start - write to title
				// if idx > end 
			}
			
			startLine = 3;
			tapStream.close();
		}
		++fileIndx;
	}
	resultFile.close();
}

int TapCombiner::getLinesInStream(std::ifstream& stream)
	{
		std::string buffer{ "" };
		int ret = 0;
		while (stream.good())
		{
			std::getline(stream, buffer);
			++ret;
		}
		
		stream.clear();
		stream.seekg(0, std::ios::beg);

		return ret;
	}

bool TapCombiner::compareFileNamesDesc(std::string lhs, std::string rhs)
{
	lhs = lhs.substr(0, lhs.length() - FILET_TYPE.length());
	rhs = rhs.substr(0, rhs.length() - FILET_TYPE.length());

	return std::atoi(lhs.c_str()) < std::atoi(rhs.c_str());
}
