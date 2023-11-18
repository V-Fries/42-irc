#include "ft_Log.hpp"
#include "ft_Io.hpp"
#include "ft_Exception.hpp"

ft::Log::Debug      ft::Log::debug;
ft::Log::Info       ft::Log::info;
ft::Log::Warning    ft::Log::warning;
ft::Log::Error      ft::Log::error;
ft::Log::Critical   ft::Log::critical;

ft::Log::DebugLevel ft::Log::_debugLevel = WARNING;
bool                ft::Log::_shouldWriteToTerminal = true;
std::ofstream       ft::Log::_file;

void  ft::Log::setDebugLevel(const ft::Log::DebugLevel newDebugLevel) {
    _debugLevel = newDebugLevel;
}

ft::Log::DebugLevel ft::Log::getDebugLevel() {
    return _debugLevel;
}


void    ft::Log::setShouldWriteToTerminal(const bool shouldWriteToTerminal) {
    _shouldWriteToTerminal = shouldWriteToTerminal;
}


void    ft::Log::setFileToWriteTo(const std::string& fileName) {
    _file.open(fileName.c_str(), std::ios_base::app | std::ios_base::ate);

    if (!_file.is_open()) {
        throw FailedToOpenFile("ft::Log::setFileToWriteTo(): "
                               "Failed to open " + fileName, ft::Log::CERR);
    }
}

ft::Log::Debug& ft::Log::Debug::operator<<(std::ostream& (*manipulator)(std::ostream&)) {
    if (_debugLevel > DEBUG) return *this;

    if (_shouldWriteToTerminal) std::cerr << manipulator;
    if (_file.is_open()) _file << manipulator;
    return *this;
}

ft::Log::Info& ft::Log::Info::operator<<(std::ostream& (*manipulator)(std::ostream&)) {
    if (_debugLevel > INFO) return *this;

    if (_shouldWriteToTerminal) std::cerr << manipulator;
    if (_file.is_open()) _file << manipulator;
    return *this;
}

ft::Log::Warning& ft::Log::Warning::operator<<(std::ostream& (*manipulator)(std::ostream&)) {
    if (_debugLevel > WARNING) return *this;

    if (_shouldWriteToTerminal) std::cerr << manipulator;
    if (_file.is_open()) _file << manipulator;
    return *this;
}

ft::Log::Error& ft::Log::Error::operator<<(std::ostream& (*manipulator)(std::ostream&)) {
    if (_debugLevel > ERROR) return *this;

    if (_shouldWriteToTerminal) std::cerr << manipulator;
    if (_file.is_open()) _file << manipulator;
    return *this;
}

ft::Log::Critical& ft::Log::Critical::operator<<(std::ostream& (*manipulator)(std::ostream&)) {
    if (_debugLevel > CRITICAL) return *this;

    if (_shouldWriteToTerminal) std::cerr << manipulator;
    if (_file.is_open()) _file << manipulator;
    return *this;
}
