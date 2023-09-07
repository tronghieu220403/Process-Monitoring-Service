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
        ZeroMemory(&last_io_counter_, sizeof(IO_COUNTERS));
        ZeroMemory(&last_time_, sizeof(FILETIME));
        ZeroMemory(&process_handle_, sizeof(HANDLE));
    #elif __linux__

    #endif

};

#ifdef _WIN32
ProcessDiskStats::ProcessDiskStats(HANDLE p_handle)
{
        ZeroMemory(&last_io_counter_, sizeof(IO_COUNTERS));
        ZeroMemory(&last_time_, sizeof(FILETIME));
        ZeroMemory(&process_handle_, sizeof(HANDLE));

        if (GetProcessId(p_handle) == NULL)
        {
            return;
        }
        process_handle_ = p_handle;
        GetProcessIoCounters(process_handle_, &last_io_counter_);
        GetSystemTimeAsFileTime(&last_time_);
};
#endif

double ProcessDiskStats::GetCurrentSpeed()
{
    double speed;

    #ifdef _WIN32

        IO_COUNTERS now_io_counter;
        FILETIME now_time;

        if (GetProcessId(process_handle_) == NULL)
        {
            last_speed_ = 0;
            return 0;
        }

        GetProcessIoCounters(process_handle_, &now_io_counter);
        GetSystemTimeAsFileTime(&now_time);

        double time_range_in_sec = static_cast<double>(now_time.dwLowDateTime - last_time_.dwLowDateTime) / 10000000;

        speed = static_cast<double>(now_io_counter.ReadTransferCount + now_io_counter.WriteTransferCount + 
        now_io_counter.OtherTransferCount - 
        last_io_counter_.ReadTransferCount - last_io_counter_.WriteTransferCount - last_io_counter_.OtherTransferCount) / time_range_in_sec ;

        last_time_ = now_time;
        last_io_counter_ = now_io_counter;

    #elif __linux__

    #endif

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

    #endif

    return last_speed_;
};


}