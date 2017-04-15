#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <boost/filesystem.hpp>

namespace bfs = boost::filesystem;

const std::string FILE_TYPE = ".tap";
const std::string RESULT_FILE_NAME = "gotovo.cnc";

// Represents the structure of our *.tap files
struct TapFile
{
	std::string name;
	std::map<int, std::string> fileMapping;
};

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

	/** Inits files vector */
	bool readFiles();

	/** Combining functionality itself*/
	void combineFiles();

	/** Predecate function for file names descending sorting */
	static bool compareFilesByNameDesc(TapFile lhs, TapFile rhs);

private:
	const bfs::path m_path;
	std::vector<TapFile> m_tapFiles;
};

