#ifdef _VISUAL_STUDIO_WORKSPACE
#include "processdisk.h"
#else
#include "include/process/processdisk.h"
#endif

namespace pm
{

ProcessDiskStats::ProcessDiskStats()
{
    #ifdef _WIN32
        ZeroMemory(&last_time_, sizeof(FILETIME));
        ZeroMemory(&process_handle_, sizeof(HANDLE));
    #elif __linux__

    #endif

};

#ifdef _WIN32
ProcessDiskStats::ProcessDiskStats(HANDLE p_handle)
{
        ZeroMemory(&last_time_, sizeof(FILETIME));
        ZeroMemory(&process_handle_, sizeof(HANDLE));

        if (GetProcessId(p_handle) == NULL)
        {
            return;
        }
        process_handle_ = p_handle;
        last_io_ = GetCurrentCounter();
        GetSystemTimeAsFileTime(&last_time_);
}
#endif

#ifdef __linux__
ProcessDiskStats::ProcessDiskStats(int pid)
{
    if (std::filesystem::is_directory("/proc/" + std::to_string(pid)) == false)
    {
        return;
    }

    pid_ = pid;
    last_io_ = GetCurrentCounter();
    
    //Get time here
    last_time_ = clock();
}
#endif


unsigned long long ProcessDiskStats::GetCurrentCounter()
{

    #ifdef _WIN32

        IO_COUNTERS now_io_counter;

        if (GetProcessId(process_handle_) == NULL)
        {
            return 0;
        }

        GetProcessIoCounters(process_handle_, &now_io_counter);

        return now_io_counter.ReadTransferCount + now_io_counter.WriteTransferCount + 
        now_io_counter.OtherTransferCount;

        
    #elif __linux__

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
        
    #endif
}


double ProcessDiskStats::GetCurrentSpeed()
{
    double speed;

    #ifdef _WIN32

        FILETIME now_time;

        if (GetProcessId(process_handle_) == NULL)
        {
            last_speed_ = 0;
            return 0;
        }

        GetSystemTimeAsFileTime(&now_time);

        double time_range_in_sec = static_cast<double>(now_time.dwLowDateTime - last_time_.dwLowDateTime) / 10000000;

    #elif __linux__
        
        clock_t now_time;

        if (std::filesystem::is_directory("/proc/" + std::to_string(pid_)) == false)
        {
            last_speed_ = 0;
        }

        now_time = clock();

        double time_range_in_sec = (double)(now_time - last_time_) / CLOCKS_PER_SEC;

    #endif

    unsigned long long cur_io = GetCurrentCounter();

    speed = static_cast<double>(cur_io - last_io_) / time_range_in_sec;

    last_time_ = now_time;

    last_io_ = cur_io;

    last_speed_ = double(speed) / (1024 * 1024);

    return last_speed_;
};

double ProcessDiskStats::GetLastSpeed()
{
    #ifdef _WIN32
        if (GetProcessId(process_handle_) == NULL)
        {
            last_speed_ = 0;
        }
    #elif __linux__
        if (std::filesystem::is_directory("/proc/" + std::to_string(pid_)) == false)
        {
            last_speed_ = 0;
        }
    #endif

    return last_speed_;
};


}