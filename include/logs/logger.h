#ifndef PROCESSMONITORING_PROCESS_PROCESSINFO_H_
#define PROCESSMONITORING_PROCESS_PROCESSINFO_H_

#define UNICODE
#define _UNICODE

#include <string.h>



#include <string>
#include <fstream>

#include <processinfo.h>

namespace pm
{
    class Logger
    {
    private:
        std::wstring message_;
    public:
        Logger() = default;
        explicit Logger(std::wstring_view& message);
        explicit Logger(std::string_view& message);
        
        virtual std::wstring_view ToWString() const;
        virtual bool WriteLog(std::wstring_view& file_name) const;
        virtual bool WriteLog(std::string_view& file_name) const;
    };
        
}

#endif