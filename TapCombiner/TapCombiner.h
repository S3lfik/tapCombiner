#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/filesystem.hpp>

namespace bfs = boost::filesystem;

const std::string FILET_TYPE = ".tap";
const std::string RESULT_FILE_NAME = "gotovo.cnc";

class TapCombiner
{
public:
	
	/** Takes a path to *tap files folder */
	explicit TapCombiner(const std::string& path);
	
	/** Inits class member variables. Returns false on errors */
	bool exec();

	~TapCombiner();
private:
	/** This is not inteded to be used */
	TapCombiner();

	/** Prepare *.tap file names*/
	void initFileNames();

	/** Combining functionality itself*/
	void combineFiles();

	/** Returns the number of lines in current file input stream */
	int getLinesInStream(std::ifstream& stream);

	/** Predecate function for file names descending sorting */
	static bool compareFileNamesDesc(std::string lhs, std::string rhs);

private:
	/** As long as we need to use own sorting, we can't trust std::set and its ordering :) */
	std::vector<std::string> m_fileNames;
	const bfs::path m_path;
};

