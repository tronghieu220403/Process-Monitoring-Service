
#include "processlogger.h"


namespace pm
{
    ProcessLogger::ProcessLogger(std::string& process_name):
        process_name_(process_name)
    {
        
    }

    std::string ProcessLogger::GetAlert(ProcessLoggerType type, UsageData usage_data)
    {
        #ifdef _WIN32
        SYSTEMTIME st;
        FileTimeToSystemTime(&usage_data.time, &st);

        int day = st.wDay;
        int month = st.wMonth; // Month is 0 - 11, add 1 to get a jan-dec 1-12 concept
        int year = st.wYear; // Year is # years since 1900
        int hour = st.wHour;
        int min = st.wMinute;
        int sec = st.wSecond;
        #elif __linux__
        time_t t = usage_data.time;
        struct tm *time_struct = localtime(&t);
        int day = time_struct->tm_mday;
        int month = time_struct->tm_mon + 1; // Month is 0 - 11, add 1 to get a jan-dec 1-12 concept
        int year = time_struct->tm_year + 1900; // Year is # years since 1900
        int hour = time_struct->tm_hour;
        int min = time_struct->tm_min;
        int sec = time_struct->tm_sec;
        #endif
        std::stringstream ss;
        ss << year << "-" << month << "-" << day << " " << hour << ":" << min << ":" << sec << ",";

        ss << process_name_ << ",";

        if (type == ProcessLoggerType::kProcessLoggerCpu)
        {
            ss << "CPU," << usage_data.data << "%";
        }
        else if (type == ProcessLoggerType::kProcessLoggerMem)
        {
            ss << "Memory," << usage_data.data << "MB";
        }
        else if (type == ProcessLoggerType::kProcessLoggerDisk)
        {
            ss << "Disk," << usage_data.data << "MB";
        }
        else if (type == ProcessLoggerType::kProcessLoggerNet)
        {
            ss << "Network," << usage_data.data << "KB";
        }
        ss << "\n";
        return ss.str();


    }
}

