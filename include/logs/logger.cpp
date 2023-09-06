
#ifdef _VISUAL_STUDIO_WORKSPACE
#include "logger.h"
#else
#include "include/logs/logger.h"
#endif

namespace pm
{

    Logger::Logger(const std::string& message, const std::string& folder_path)
        : message_(message), folder_path_(folder_path)
    {

    };

    Logger::Logger(const std::string& message): message_(message)
    {
    };

    void Logger::SetMessage(const std::string_view& message)
    {
        message_ = message;
    }

    void Logger::SetFolderPath(const std::string_view& folder_path)
    {
        folder_path_ = folder_path;
    }

    std::string Logger::GetMessage() const
    {
        return message_;
    }

    std::string Logger::GetFolderPath() const
    {
        return folder_path_;
    }

    bool Logger::CreateLogFolder(const std::string_view& folder_path) const
    {
        // do sth
        return true;
    }

    void Logger::WriteLog() const
    {
        /*
            Log file name should be pm_logs_yy_mm_dd.log
        */
        time_t theTime = time(nullptr);
        struct tm const *aTime = localtime(&theTime);
        int day = aTime->tm_mday;
        int month = aTime->tm_mon + 1; // Month is 0 - 11, add 1 to get a jan-dec 1-12 concept
        int year = aTime->tm_year + 1900; // Year is # years since 1900

        std::stringstream ss;
        ss << "pm_logs_" << year << "_" << month << "_" << day << ".log";

        std::string file_name;
        file_name = folder_path_ + ss.str();

        /* 
        Should be replaced by OS native file IO API 
        */
        std::ofstream ofs(file_name, std::ios_base::app);
        ofs.write(&message_[0], strlen(&message_[0]));
        ofs.close();

    }

}