#include "processnetwork.h"

namespace pm
{

    ProcessNetworkStats::ProcessNetworkStats() = default;

    ProcessNetworkStats::ProcessNetworkStats(int pid):
        pid_(pid)
    {
        #ifdef __linux__
            if (std::filesystem::is_directory("/proc/" + std::to_string(pid)) == false)
            {
                pid_ = 0;
                return;
            }
        #endif
    };

void ProcessNetworkStats::SetPid(int pid)
{
    pid_ = pid;
}

int ProcessNetworkStats::GetPid() const
{
    return pid_;
}

#ifdef _WIN32

    void ProcessNetworkStats::AddData(FILETIME time, unsigned long long data)
    {
        UsageData io_data;
        SYSTEMTIME last_io_st;
        SYSTEMTIME st;

        if (io_deque_.size() == 0)
        {
            io_data.data = static_cast<double>(data);
            io_data.time = time;
            io_deque_.push_back(io_data);
            return;
        }
        UsageData& last_io_data = io_deque_.back();
        FileTimeToSystemTime(&time, &st);
        unsigned long long data_ym = st.wYear * 12 + st.wMonth;
        unsigned long long data_dhms = st.wDay * 3600 * 12 + st.wHour * 3600 + st.wMinute * 60 + st.wSecond;

        FileTimeToSystemTime(&last_io_data.time, &last_io_st);
        unsigned long long last_ym = last_io_st.wYear * 12 + last_io_st.wMonth;
        unsigned long long last_dhms = last_io_st.wDay * 3600 * 12 + last_io_st.wHour * 3600 + last_io_st.wMinute * 60 + last_io_st.wSecond;
        if (last_ym == data_ym)
        {
            if (last_dhms == data_dhms)
            {
                last_io_data.data += static_cast<double>(data);
            }
            else if (last_dhms < data_dhms)
            {
                io_data.data = static_cast<double>(data);
                io_data.time = time;
                io_deque_.push_back(io_data);
            }
        }
        else if (last_ym < data_ym)
        {
            io_data.data = static_cast<double>(data);
            io_data.time = time;
            io_deque_.push_back(io_data);
        }
    }

    bool ProcessNetworkStats::HasData() const
    {
        return io_deque_.size() > 1;
    }

    UsageData ProcessNetworkStats::GetFrontIoDataInKb()
    {
        UsageData usage_data;
        usage_data.time = io_deque_.front().time;
        usage_data.data = (double)(io_deque_.front().data) / 1024;
        return usage_data;
    }

    void ProcessNetworkStats::DeleteFrontIodata()
    {
        io_deque_.pop_front();
    }

#elif __linux__

    double ProcessNetworkStats::GetLastSpeed()
    {
        return last_speed_;
    }

    void ProcessNetworkStats::UpdateAttributes()
    {
        return;
    }

    UsageData ProcessNetworkStats::GetLastIoSpeedInKb()
    {
        UsageData usage_data;
        usage_data.data = last_speed_;
        usage_data.time = last_retrieved_time_;
        return usage_data; 
    }


#endif
}
