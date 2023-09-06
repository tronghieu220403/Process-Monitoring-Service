#ifndef PROCESSMONITORING_LOGGER_LOGGER_H_
#define PROCESSMONITORING_LOGGER_LOGGER_H_

#include <string.h>
#include <time.h>


#include <string>
#include <fstream>
#include <sstream>

#ifdef _VISUAL_STUDIO_WORKSPACE
    #include "E:/Code/Github/Process-Monitoring/include/process/processinfo.h"
#else
    #include "include/process/processinfo.h"
#endif

namespace pm
{
    class Logger
    {
    private:
        std::string message_;
        std::string folder_path_;
    public:
        Logger() = default;
        
        Logger(const std::string& message, const std::string& folder_path);
                
        explicit Logger(std::string_view& message);
        
        void SetMessage(std::string_view& message);
        void SetFolderPath(std::string_view& folder_path);

        std::string GetMessage();
        std::string GetFolderPath();

        bool CreateLogFolder(std::string folder_path);
        bool WriteLog() const;

        virtual ~Logger() = default;

    };
        
}

#endif