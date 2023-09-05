#ifndef PROCESSMONITORING_LOGGER_LOGGER_H_
#define PROCESSMONITORING_LOGGER_LOGGER_H_

#define UNICODE
#define _UNICODE

#include <string.h>
#include <time.h>



#include <string>
#include <fstream>
#include <sstream>

#include "include/process/processinfo.h"

namespace pm
{
    class Logger
    {
    private:
        std::wstring message_;
        std::wstring folder_path_;
    public:
        Logger() = default;
        explicit Logger(std::wstring_view& message);
        
        explicit Logger(std::string_view& message);
        
        void SetMessage(std::string_view& message);
        void SetMessage(std::wstring_view& message);

        virtual bool SetLoggingFolder(std::string folder_path);
        virtual bool SetLoggingFolder(std::wstring folder_path);

        std::wstring GetMessage();

        bool WriteLog() const;

        bool operator==(const Logger& other) const = default;

        virtual ~Logger() = default;

    };
        
}

#endif