
#ifdef _VISUAL_STUDIO_WORKSPACE
#include "processinfo.h"
#else
#include "include/process/processinfo.h"
#endif


namespace pm
{

ProcessInfo::ProcessInfo(): pid_(0),time_(static_cast<long long>(0))
{

};

ProcessInfo::ProcessInfo(const ProcessInfo& pi) = default;

ProcessInfo& ProcessInfo::operator=(const ProcessInfo& pi) = default;

ProcessInfo::ProcessInfo(const ProcessInfo&& pi)
{
    this->pid_ = pi.pid_;
    this->time_ = pi.time_;
    this->memory_usage_ = std::move(pi.memory_usage_);
    this->cpu_usage_ = std::move(pi.cpu_usage_);
    this->disk_usage_ = std::move(pi.disk_usage_);
    this->network_usage_ = std::move(pi.network_usage_);
    this->last_usage_ = std::move(pi.last_usage_);
}


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
        #elif __linux__
    
        #endif
        cpu_usage_ = ProcessCpuStats(process_handle_);
        disk_usage_ = ProcessDiskStats(process_handle_);
        network_usage_ = ProcessNetworkStats(process_handle_);
    };
#elif __linux__

#endif

time_t ProcessInfo::UpdateTime()
{
    ctime(&time_);
    return time_;
}

int ProcessInfo::GetPid()
{
    return pid_;
};

double ProcessInfo::UpdateCpuUsage()
{
    #ifdef _WIN32
        if (GetProcessId(process_handle_) == NULL)
        {
            return 0;
        }
    #elif __linux__

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
    #elif __linux__
        
    #endif
};

double ProcessInfo::UpdateDiskUsage()
{
    return disk_usage_.GetCurrentSpeed();
};

double ProcessInfo::UpdateNetworkUsage()
{
    return network_usage_.GetCurrentSpeed();
};

void ProcessInfo::UpdateAttributes()
{
    UpdateTime();
    last_usage_.cpu_usage = UpdateCpuUsage();
    last_usage_.mem_usage = UpdateMemoryUsage();
    last_usage_.disk_usage = UpdateDiskUsage();
    last_usage_.network_usage = UpdateNetworkUsage();
}

time_t ProcessInfo::GetTime()
{
    return time_;
}

double ProcessInfo::GetCpuUsage()
{
    return last_usage_.cpu_usage;
}

double ProcessInfo::GetMemoryUsage()
{
    return last_usage_.mem_usage;
}

double ProcessInfo::GetDiskUsage()
{
    return last_usage_.disk_usage;

}

double ProcessInfo::GetNetworkUsage()
{
    return last_usage_.network_usage;
}

MonitoringComponent ProcessInfo::GetUsage()
{
    return last_usage_;
}

}


