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

void ProcessNetworkStats::SetIoSizeInByte(unsigned long long io_size_in_byte)
{
    io_size_in_byte_ = io_size_in_byte;
}

unsigned long long ProcessNetworkStats::GetIoSizeInByte()
{
    return io_size_in_byte_;
}

double ProcessNetworkStats::GetIoSizeInMb()
{
    return double(io_size_in_byte_) / (1024 * 1024);
}


void ProcessNetworkStats::SetLastRetrieveTime(FILETIME time)
{
    last_retrieve_time_ = time;
}

FILETIME ProcessNetworkStats::GetLastRetrieveTime() const
{
    return last_retrieve_time_;
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
