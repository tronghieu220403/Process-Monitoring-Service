
#include "processlogger.h"


namespace pm
{

    ProcessLogger::ProcessLogger()
        :process_controller_(std::make_shared<ProcessController>())
    {
    };

    ProcessLogger::ProcessLogger(const std::shared_ptr<ProcessController>& pc)
        : process_controller_(pc)
    {
    }

    ProcessLogger& ProcessLogger::operator=(const ProcessLogger& pl)
    {
        this->process_controller_ = pl.process_controller_;
        this->SetFolderPath(pl.GetFolderPath());
        this->SetMessage(pl.GetMessage());
        return *this;
    }


    void ProcessLogger::SetProcessController(const std::shared_ptr<ProcessController>& pc)
    {
        process_controller_ = pc;
    }


#ifdef __linux__

    std::string ProcessLogger::GetAlert(ProcessLoggerType type)
    {
        std::stringstream ss;

        ProcessInfo const& p_info = process_controller_->GetProcessInfo();

        time_t t = p_info.GetTime();

        struct tm *time_struct = localtime(&t);
        int day = time_struct->tm_mday;
        int month = time_struct->tm_mon + 1; // Month is 0 - 11, add 1 to get a jan-dec 1-12 concept
        int year = time_struct->tm_year + 1900; // Year is # years since 1900
        int hour = time_struct->tm_hour;
        int min = time_struct->tm_min;
        int sec = time_struct->tm_sec;
        ss << year << "-" << month << "-" << day << " " << hour << ":" << min << ":" << sec << ",";
        
        ss << process_controller_->GetPid() << "," << process_controller_->GetName() << ",";
        if (type == ProcessLoggerType::kProcessLoggerCpu)
        {
            ss << "CPU,";
            double f = p_info.GetCpuUsage();
            ss << f;
        }
        else if (type == ProcessLoggerType::kProcessLoggerMem)
        {
            ss << "Memory,";
            double f = p_info.GetMemoryUsage();
            ss << f;
        }
        else if (type == ProcessLoggerType::kProcessLoggerDisk)
        {
            ss << "Disk,";
            double f = p_info.GetDiskUsage();
            ss << f;
        }
        else if (type == ProcessLoggerType::kProcessLoggerNet)
        {
            ss << "Network,";
            double f = p_info.GetNetworkUsage();
            ss << f;
        }
        ss << "\n";
        return ss.str();
    }

#elif _WIN32

    std::string ProcessLogger::GetAlert(ProcessLoggerType type, UsageData usage_data)
    {
        SYSTEMTIME st;
        FileTimeToSystemTime(&usage_data.time, &st);

        int day = st.wDay;
        int month = st.wMonth; // Month is 0 - 11, add 1 to get a jan-dec 1-12 concept
        int year = st.wYear; // Year is # years since 1900
        int hour = st.wHour;
        int min = st.wMinute;
        int sec = st.wSecond;

        std::stringstream ss;
        ss << year << "-" << month << "-" << day << " " << hour << ":" << min << ":" << sec << ",";

        ss << process_controller_->GetPid() << "," << process_controller_->GetName() << ",";

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

#endif
}

