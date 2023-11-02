#include "ft_Exception.hpp"

typedef ft::Exception   Exception;

Exception::ExceptionStream::ExceptionStream(const ft::Log::DebugLevel debugLevel):
    _debugLevel(debugLevel) {
}

const Exception::ExceptionStream&  Exception::ExceptionStream::operator<<(
                                        std::ostream& (* manipulator)(std::ostream&)) const {
    switch (_debugLevel) {
        case ft::Log::DEBUG:
            ft::Log::debug << manipulator;
            return *this;
        case ft::Log::INFO:
            ft::Log::info << manipulator;
            return *this;
        case ft::Log::WARNING:
            ft::Log::warning << manipulator;
            return *this;
        case ft::Log::ERROR:
            ft::Log::error << manipulator;
            return *this;
        case ft::Log::CRITICAL:
            ft::Log::critical << manipulator;
            return *this;
        case ft::Log::NO_LOGS:
            return *this;
        default:
            std::cerr << manipulator;
            return *this;
    }
}

Exception::Exception(const std::string& whatMessage,
                         const ft::Log::DebugLevel debugLevel):
    stream(debugLevel),
    _whatMessage(whatMessage) {
}


Exception::~Exception() throw() {}


const char* Exception::what() const throw() {
    return _whatMessage.c_str();
}


ft::FailedToOpenFile::FailedToOpenFile(const std::string& whatMessage,
                                       const ft::Log::DebugLevel debugLevel):
    Exception(whatMessage, debugLevel) {
}
