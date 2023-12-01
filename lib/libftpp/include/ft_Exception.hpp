#pragma once

#include "ft_Log.hpp"

#include <exception>
#include <string>

namespace ft {
    class Exception;
    class FailedToOpenFile;
};

class ft::Exception : public std::exception {
    private:
        class ExceptionStream {
            public:
                explicit ExceptionStream(ft::Log::DebugLevel debugLevel);

                template<typename T>
                const ExceptionStream& operator<<(const T& value) const {
                    switch (_debugLevel) {
                        case ft::Log::DEBUG:
                            ft::Log::debug << value;
                            return *this;
                        case ft::Log::INFO:
                            ft::Log::info << value;
                            return *this;
                        case ft::Log::WARNING:
                            ft::Log::warning << value;
                            return *this;
                        case ft::Log::ERROR:
                            ft::Log::error << value;
                            return *this;
                        case ft::Log::CRITICAL:
                            ft::Log::critical << value;
                            return *this;
                        case ft::Log::NO_LOGS:
                            return *this;
                        default:
                            std::cerr << value;
                            return *this;
                    }
                }

                const ExceptionStream&  operator<<(std::ostream& (* manipulator)(std::ostream&)) const;

            private:
                const ft::Log::DebugLevel   _debugLevel;
        };

    public:
        Exception(const ft::String& whatMessage, Log::DebugLevel debugLevel);

        virtual ~Exception() throw();

        virtual void    printError() const;

        virtual const char* what() const throw();

        const ExceptionStream   stream;

    private:

        const ft::String   _whatMessage;
};

class ft::FailedToOpenFile : public ft::Exception {
    public:
        FailedToOpenFile(const ft::String& whatMessage,
                         ft::Log::DebugLevel debugLevel);
};
