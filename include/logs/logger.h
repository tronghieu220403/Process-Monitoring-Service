#ifndef PROCESSMONITORING_LOGGER_LOGGER_H_
#define PROCESSMONITORING_LOGGER_LOGGER_H_

#define UNICODE
#define _UNICODE

#include <string.h>
#include <time.h>



#include <string>
#include <fstream>
#include <sstream>

#include <processinfo.h>

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
        
        virtual void SetMessage(std::string_view& message) final;
        virtual void SetMessage(std::wstring_view& message) final;

        virtual bool SetLoggingFolder(std::string folder_path) final;
        virtual bool SetLoggingFolder(std::wstring folder_path) final;

        virtual std::wstring_view ToWString() const;

        virtual bool WriteLog() const;

    };
        
}

#endif