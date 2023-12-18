#include "CSVParser.hpp"

#include <sstream>

static ft::Exception    failedToOpenFile(const ft::String& fileName,
                                         ft::Log::DebugLevel debugLevel);
static ft::Exception    fileIsNotFormattedCorrectly(const ft::String& fileName,
                                                    ft::Log::DebugLevel debugLevel);
static void             parseLine(const ft::String& line,
                                  CSVParser::CSVFile& result);

CSVParser::CSVFile  CSVParser::parseCSVFile(const ft::String& fileName,
                                            ft::Log::DebugLevel logLevelOnError)
                        throw (ft::Exception) {
    std::ifstream   fileStream(fileName.c_str());

    if (!fileStream.is_open()) {
        throw failedToOpenFile(fileName, logLevelOnError);
    }
    ft::String  line;
    if (!getline(fileStream, line)) {
        throw fileIsNotFormattedCorrectly(fileName, logLevelOnError);
    }

    CSVFile result;
    while (getline(fileStream, line)) {
        parseLine(line, result);
    }
    return result;
}

static ft::Exception    failedToOpenFile(const ft::String& fileName,
                                         const ft::Log::DebugLevel debugLevel) {
    std::stringstream   logMessage;

    logMessage << "Failed to open file " << fileName;
    return ft::Exception(logMessage.str(), debugLevel);
}

static ft::Exception    fileIsNotFormattedCorrectly(const ft::String& fileName,
                                                    const ft::Log::DebugLevel debugLevel) {
    std::stringstream   logMessage;

    logMessage << "CSV file " << fileName << " is not formatted correctly";
    return ft::Exception(logMessage.str(), debugLevel);
}

static void parseLine(const ft::String& line,
                      CSVParser::CSVFile& result) {
    const std::vector<ft::String> splitedLine = line.split(",");

    if (splitedLine.empty()) {
        return;
    }
    if (splitedLine.size() < 2) {
        result.push_back(std::pair<ft::String, ft::String>(splitedLine[0],
                                                           ""));
        return;
    }
    result.push_back(std::pair<ft::String, ft::String>(splitedLine[0],
                                                       splitedLine[1]));
}