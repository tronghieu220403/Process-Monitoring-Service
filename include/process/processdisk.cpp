#include "processdisk.h"

namespace pm
{

ProcessDiskStats::ProcessDiskStats()
{

};

ProcessDiskStats::ProcessDiskStats(int pid)
{
    #ifdef _WIN32
    pid_ = pid;
    #elif __linux__
    if (std::filesystem::is_directory("/proc/" + std::to_string(pid)) == false)
    {
        return;
    }

    pid_ = pid;
    last_io_ = GetCurrentCounter();
    
    //Get time here
    last_time_ = clock();
    #endif
}

void ProcessDiskStats::SetPid(int pid)
{
    pid_ = pid;
}

int ProcessDiskStats::GetPid() const
{
    return pid_;
}

#ifdef _WIN32

void ProcessDiskStats::SetIoSizeInByte(unsigned long long io_size_in_byte_)
{
    io_size_in_byte_ = io_size_in_byte;
}

unsigned long long ProcessDiskStats::GetIoSizeInByte()
{
    return io_size_in_byte_;
}

double ProcessDiskStats::GetIoSizeInMb()
{
    return double(io_size_in_byte_) / (1024 * 1024);
}

void ProcessDiskStats::SetLastRetrieveTime(FILETIME time)
{
    last_retrieve_time_ = time;
}

FILETIME ProcessDiskStats::GetLastRetrieveTime() const
{
    return last_retrieve_time_;
}

#elif __linux__

unsigned long long ProcessDiskStats::GetCurrentCounter()
{
    if (std::filesystem::is_directory("/proc/" + std::to_string(pid_)) == false)
    {
        return 0;
    }

    std::ifstream file("/proc/" + std::to_string(pid_) + "/io");
    std::string line;

    unsigned long long r;
    unsigned long long w;

    while(getline(file, line))
    {
        if (line.find("read_bytes") == 0)
        {
            std::stringstream s(line);
            std::string name;
            s >> name >> r;
        }
        if (line.find("write_bytes") == 0)
        {
            std::stringstream s(line);
            std::string name;
            s >> name >> w;
            break;
        }
    }
    return r + w;
}

unsigned long long ProcessDiskStats::GetCurrentSpeed()
{

    clock_t now_time;

    if (std::filesystem::is_directory("/proc/" + std::to_string(pid_)) == false)
    {
        last_speed_ = 0;
        return 0;
    }

    now_time = clock();

    double time_range_in_sec = (double)(now_time - last_time_) / CLOCKS_PER_SEC;

    unsigned long long cur_io = GetCurrentCounter();

    speed = static_cast<double>(cur_io - last_io_) / time_range_in_sec;

    last_time_ = now_time;

    last_io_ = cur_io;

    last_speed_ = double(speed) / (1024 * 1024);

    return last_speed_;
};

unsigned long long ProcessDiskStats::GetSpeed()
{
    if (std::filesystem::is_directory("/proc/" + std::to_string(pid_)) == false)
    {
        last_speed_ = 0;
    }

    return last_speed_;
};
#endif


}