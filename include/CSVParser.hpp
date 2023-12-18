#pragma once

#include "ft.hpp"

class CSVParser {
    public:
        typedef std::vector<std::pair<ft::String, ft::String> > CSVFile;

        static CSVFile  parseCSVFile(const ft::String& fileName,
                                     ft::Log::DebugLevel logLevelOnError)
                            throw (ft::Exception);
};