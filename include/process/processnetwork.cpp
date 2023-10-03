#include "processnetwork.h"

namespace pm
{

    ProcessNetworkStats::ProcessNetworkStats() = default;

    ProcessNetworkStats::ProcessNetworkStats(int pid)
    {
        #ifdef _WIN32
            pid_ = pid;
        #elif __linux__

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
        UsageIoData& last_io_data = io_deque_.back();
        if (last_io_data.time.dwHighDateTime == time.dwHighDateTime && last_io_data.time.dwLowDateTime == time.dwLowDateTime)
        {
            
        }
    }

    bool ProcessNetworkStats::HasData() const
    {
        return io_deque_.size() > 1;
    }

    UsageIoData ProcessNetworkStats::GetFirstIoData()
    {
        return io_deque_.front();
    }

    void ProcessNetworkStats::DeleteFirstIodata()
    {
        io_deque_.pop_front();
    }

#elif __linux__

    double ProcessNetworkStats::GetCurrentSpeed()
    {
        return last_speed_;
    }

    double ProcessNetworkStats::GetLastSpeed()
    {
        return last_speed_;
    }

#endif
}
