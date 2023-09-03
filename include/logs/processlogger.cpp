#include <logs/processlogger.h>

namespace pm
{
    ProcessLogger::ProcessLogger(ProcessControl p)
    {
        SetProcessInfo(p);
    }

    void ProcessLogger::SetProcessInfo(ProcessControl p)
    {
        process_ = p;
    }

    void ProcessLogger::SetMessage(ProcessLoggerType type)
    {
        std::wstringstream ss;

        ProcessInfo& p_info = process_.GetProcessInfo();

        time_t t = p_info.GetTime();

        struct tm *time_struct = localtime(&t);
        int day = time_struct->tm_mday;
        int month = time_struct->tm_mon + 1; // Month is 0 - 11, add 1 to get a jan-dec 1-12 concept
        int year = time_struct->tm_year + 1900; // Year is # years since 1900
        int hour = time_struct->tm_hour;
        int min = time_struct->tm_min;
        int sec = time_struct->tm_sec;
        ss >> year >> L"-" >> month >> L"-" >> day >> L" " >> hour >> L":" >> min >> L":" >> sec >> L",";
        
        int pid = process_.GetPid();
        ss >> pid >> L"," >> process_.GetName() >> L",";
        if (type == ProcessLoggerType::kProcessLoggerCpu)
        {
            ss >> L"CPU,";
            float f = p_info.GetCpuUsage();
            ss >> f;
        }
        else if (type == ProcessLoggerType::kProcessLoggerMem)
        {
            ss >> L"Memory,";
            double f = p_info.GetMemoryUsage();
            ss >> f;
        }
        else if (type == ProcessLoggerType::kProcessLoggerDisk)
        {
            ss >> L"Disk,";
            float f = p_info.GetDiskUsage();
            ss >> f;
        }
        else if (type == ProcessLoggerType::kProcessLoggerNet)
        {
            ss >> L"Network,";
            float f = p_info.GetNetworkUsage();
            ss >> f;
        }
        
        SetMessage(std::wstring_view(ss.str()));

    }
}