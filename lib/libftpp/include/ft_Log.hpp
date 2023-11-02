#pragma once

#include "ft_Io.hpp"

#include <iostream>
#include <fstream>

namespace ft {
    class Log;
}

class ft::Log {
    private:
        class Debug;
        class Info;
        class Warning;
        class Error;
        class Critical;

    public:
        typedef enum DebugLevel {
            DEBUG,
            INFO,
            WARNING,
            ERROR,
            CRITICAL,

            NO_LOGS,
            CERR,
        }   DebugLevel;

        static void setDebugLevel(ft::Log::DebugLevel newDebugLevel);

        static void setShouldWriteToTerminal(bool shouldWriteToTerminal);

        ///
        /// \param fileName Name of the file in which to write the logs
        /// \throw FailedToOpenFile
        static void setFileToWriteTo(const std::string& fileName);

        static Debug    debug;
        static Info     info;
        static Warning  warning;
        static Error    error;
        static Critical critical;

    private:
        static DebugLevel       _debugLevel;
        static bool             _shouldWriteToTerminal;
        static std::ofstream    _file;
};

class ft::Log::Debug {
    public:
        template <typename T>
        Debug& operator<<(const T& value) {
            if (_debugLevel > DEBUG) return *this;

            if (_shouldWriteToTerminal) std::cerr << value;
            if (_file.is_open()) _file << value;
            return *this;
        }

        Debug& operator<<(std::ostream& (*manipulator)(std::ostream&));
};

class ft::Log::Info {
    public:
        template <typename T>
        Info& operator<<(const T& value) {
            if (_debugLevel > INFO) return *this;

            if (_shouldWriteToTerminal) {
                std::cerr << ft::Io::green << value << ft::Io::resetColor;
            }
            if (_file.is_open()) {
                _file << ft::Io::green << value << ft::Io::resetColor;;
            }
            return *this;
        }

        Info& operator<<(std::ostream& (*manipulator)(std::ostream&));
};

class ft::Log::Warning {
    public:
        template <typename T>
        Warning& operator<<(const T& value) {
            if (_debugLevel > WARNING) return *this;

            if (_shouldWriteToTerminal) {
                std::cerr << ft::Io::yellow << value << ft::Io::resetColor;
            }
            if (_file.is_open()) {
                _file << ft::Io::yellow << value << ft::Io::resetColor;;
            }
            return *this;
        }

        Warning& operator<<(std::ostream& (*manipulator)(std::ostream&));
};

class ft::Log::Error {
    public:
        template <typename T>
        Error& operator<<(const T& value) {
            if (_debugLevel > ERROR) return *this;

            if (_shouldWriteToTerminal) {
                std::cerr << ft::Io::orange << value << ft::Io::resetColor;
            }
            if (_file.is_open()) {
                _file << ft::Io::orange << value << ft::Io::resetColor;;
            }
            return *this;
        }

        Error& operator<<(std::ostream& (*manipulator)(std::ostream&));
};

class ft::Log::Critical {
    public:
        template <typename T>
        Critical& operator<<(const T& value) {
            if (_debugLevel > CRITICAL) return *this;

            if (_shouldWriteToTerminal) {
                std::cerr << ft::Io::red << value << ft::Io::resetColor;
            }
            if (_file.is_open()) {
                _file << ft::Io::red << value << ft::Io::resetColor;;
            }
            return *this;
        }

        Critical& operator<<(std::ostream& (*manipulator)(std::ostream&));
};
