
#ifdef _VISUAL_STUDIO_WORKSPACE
#include "processinfo.h"
#else
#include "include/process/processinfo.h"
#endif


namespace pm
{

#ifdef _WIN32
    ProcessInfo::ProcessInfo(HANDLE process_handle)
    {
        #ifdef _WIN32
            pid_ = GetProcessId(process_handle);
            if (pid_ == NULL)
            {
                process_handle_ = nullptr;
                return;
            }
            else
            {
                process_handle_ = process_handle;
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

    int ProcessInfo::GetPid() const
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
            PROCESS_MEMORY_COUNTERS_EX pmc{};

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

    time_t ProcessInfo::GetTime() const
    {
        return time_;
    }

    double ProcessInfo::GetCpuUsage() const
    {
        return last_usage_.cpu_usage;
    }

    double ProcessInfo::GetMemoryUsage() const
    {
        return last_usage_.mem_usage;
    }

    double ProcessInfo::GetDiskUsage() const
    {
        return last_usage_.disk_usage;

    }

    double ProcessInfo::GetNetworkUsage() const
    {
        return last_usage_.network_usage;
    }

    MonitoringComponent ProcessInfo::GetUsage() const
    {
        return last_usage_;
    }

}


