#include "processinfo.h"

namespace pm
{

ProcessInfo::ProcessInfo(): pid_(0),time_(static_cast<long long>(0))
{};
#ifdef _WIN32
    ProcessInfo::ProcessInfo(HANDLE process_handle_)
    {
        #ifdef _WIN32
            pid_ = GetProcessId(process_handle_);
            if (pid_ == NULL)
            {
                process_handle_ = 0;
                return;
            }
        #else
    
        #endif
        cpu_usage_ = ProcessCpuStats(process_handle_);
        disk_usage_ = ProcessDiskStats(process_handle_);
        network_usage_ = ProcessNetworkStats(process_handle_);
    };
#else

#endif
ProcessInfo::~ProcessInfo()
{
    #ifdef _WIN32
        CloseHandle(process_handle_);
    #else

    #endif

}

time_t ProcessInfo::UpdateTime()
{
    ctime(&time_);
    return time_;
}

int ProcessInfo::GetPid()
{
    return pid_;
};

float ProcessInfo::UpdateCpuUsage()
{
    #ifdef _WIN32
        if (GetProcessId(process_handle_) == NULL)
        {
            return 0;
        }
    #else

    #endif
    return cpu_usage_.GetCurrentUsage();
};

double ProcessInfo::UpdateMemoryUsage()
{
    #ifdef _WIN32
        PROCESS_MEMORY_COUNTERS_EX pmc;

        if (GetProcessId(process_handle_) == NULL)
        {
            return 0;
        }

        GetProcessMemoryInfo(process_handle_, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
        memory_usage_ = double(pmc.WorkingSetSize) / 1024;
        return memory_usage_;
    #else
        
    #endif
};

float ProcessInfo::UpdateDiskUsage()
{
    return disk_usage_.GetCurrentSpeed();
};

float ProcessInfo::UpdateNetworkUsage()
{
    return network_usage_.GetCurrentSpeed();
};

void ProcessInfo::UpdateAttributes()
{
    UpdateTime();
    UpdateCpuUsage();
    UpdateMemoryUsage();
    UpdateDiskUsage();
    UpdateNetworkUsage();
}

time_t ProcessInfo::GetTime()
{
    return time_;
}

float ProcessInfo::GetCpuUsage()
{
    return cpu_usage_.GetLastUsage();
}

double ProcessInfo::GetMemoryUsage()
{
    return memory_usage_;
}

float ProcessInfo::GetDiskUsage()
{
    return disk_usage_.GetLastSpeed();

}

float ProcessInfo::GetNetworkUsage()
{
    return network_usage_.GetLastSpeed();
}

}


