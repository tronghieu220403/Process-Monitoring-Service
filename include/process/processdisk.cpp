#include "include/process/processdisk.h"

namespace pm
{

ProcessDiskStats::ProcessDiskStats()
{
    last_speed_ = 0;
    #ifdef _WIN32
        ZeroMemory(&last_io_counter_, sizeof(IO_COUNTERS));
        ZeroMemory(&last_time_, sizeof(FILETIME));
        ZeroMemory(&process_handle_, sizeof(HANDLE));
    #elif __linux__

    #endif

};

ProcessDiskStats::ProcessDiskStats(ProcessDiskStats& pds)
{
    #ifdef _WIN32
        this.last_io_counter_ = pds.last_io_counter_;
        this.last_time_ = pds.last_time_;
        this.process_handle_ = pds.process_handle_;
        this.last_speed_ = pds.last_speed_;
    #elif __linux__

    #endif

};

ProcessDiskStats::ProcessDiskStats(ProcessDiskStats&& pds)
{
    #ifdef _WIN32
        this.last_io_counter_ = std::move(pds.last_io_counter_);
        this.last_time_ = std::move(pds.last_time_);
        this.process_handle_ = std::move(pds.process_handle_);
        this.last_speed_ = pds.last_speed_;
    #elif __linux__

    #endif

};

ProcessDiskStats::ProcessDiskStats(HANDLE p_handle)
{
    #ifdef _WIN32
        if (GetProcessId(p_handle) == NULL)
        {
            return;
        }
        process_handle_ = p_handle;
        GetProcessIoCounters(process_handle_, &last_io_counter_);
        GetSystemTimeAsFileTime(&last_time_);
    #elif __linux__

    #endif
};

double ProcessDiskStats::GetCurrentSpeed()
{
    unsigned long long speed;

    #ifdef _WIN32

        IO_COUNTERS now_io_counter;
        FILETIME now_time;

        if (GetProcessId(process_handle_) == NULL)
        {
            return 0.0;
        }

        GetProcessIoCounters(process_handle_, &now_io_counter);
        GetSystemTimeAsFileTime(&now_time);

        speed = (now_io_counter.ReadTransferCount + now_io_counter.WriteTransferCount + now_io_counter.OtherTransferCount - last_io_counter_.ReadTransferCount + last_io_counter_.WriteTransferCount + last_io_counter_.OtherTransferCount) * 100 * 10000000 / (now_time.dwLowDateTime - last_time_.dwLowDateTime) / 1024;

        last_time_ = now_time;
        last_io_counter_ = now_io_counter;

    #elif __linux__

    #endif

    last_speed_ = double(speed)/100;
    return last_speed_;
};

double ProcessDiskStats::GetLastSpeed()
{
    #ifdef _WIN32
        if (GetProcessId(process_handle_) == NULL)
        {
            return 0.0;
        }
    #elif __linux__

    #endif

    return last_speed_;
};

}