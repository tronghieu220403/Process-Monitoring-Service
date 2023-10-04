#include "processinfo.h"


namespace pm
{

    ProcessInfo::ProcessInfo(std::string& p_name, int pid)
    {
        process_name_ = p_name;
        pid_ = pid;
        memory_usage_stats = std::make_shared<ProcessMemoryStats>(p_name, pid);
        cpu_usage_stats = std::make_shared<ProcessCpuStats>(p_name, pid);
        disk_usage_stats = std::make_shared<ProcessDiskStats>(pid); // need update
        network_usage_stats = std::make_shared<ProcessNetworkStats>(pid);
        #ifdef __linux__
        UpdateTime();
        #endif
    };

    int ProcessInfo::GetPid() const
    {
        return pid_;
    };


#ifdef __linux__
    time_t ProcessInfo::UpdateTime()
    {
        this->time_ = time(0);
        return time_;
    }

    double ProcessInfo::UpdateCpuUsage()
    {
        #ifdef _WIN32

        #elif __linux__

        #endif

        cpu_usage_stats.UpdateAttributes();

        return cpu_usage_stats.GetLastUsagePercentage();
    };

    double ProcessInfo::UpdateMemoryUsage()
    {
        #ifdef _WIN32

        #elif __linux__

        #endif

        return memory_usage_stats.UpdateAttributes(); // ?
    };

    double ProcessInfo::UpdateDiskUsage()
    {
        #ifdef _WIN32

        #elif __linux__
            return disk_usage_stats.GetCurrentSpeed();
        #endif

    };

    double ProcessInfo::UpdateNetworkUsage()
    {
        #ifdef _WIN32

        #elif __linux__
            return network_usage_stats.GetCurrentSpeed();
        #endif

    };
#elif _WIN32

    void ProcessInfo::UpdateDiskUsage()
    {

    }

    void ProcessInfo::UpdateNetworkUsage()
    {
        
    }

    void ProcessInfo::UpdateCpuUsage()
    {
        GetCpuUsageStats()->UpdateAttributes();
    }

    void ProcessInfo::UpdateMemoryUsage()
    {
        GetMemoryUsageStats()->UpdateAttributes();
    }

#endif

    void ProcessInfo::UpdateAttributes()
    {
        #ifdef __linux__
        ProcessInfo::UpdateTime();
        last_usage_.cpu_usage.data = UpdateCpuUsage();
        last_usage_.mem_usage.data = UpdateMemoryUsage();
        last_usage_.disk_usage.data = UpdateDiskUsage();
        last_usage_.network_usage.data = UpdateNetworkUsage();
        #elif _WIN32
            UpdateDiskUsage();
            UpdateNetworkUsage();
            UpdateCpuUsage();
            UpdateMemoryUsage();
        #endif
    }

#ifdef __linux__
    time_t ProcessInfo::GetTime() const
    {
        return time_;
    }
#endif

    std::shared_ptr<ProcessMemoryStats> ProcessInfo::GetMemoryUsageStats() const
    {
        return memory_usage_stats;
    }

    std::shared_ptr<ProcessCpuStats> ProcessInfo::GetCpuUsageStats() const
    {
        return cpu_usage_stats;
    }

    std::shared_ptr<ProcessDiskStats> ProcessInfo::GetDiskUsageStats() const
    {
        return disk_usage_stats;
    }

    std::shared_ptr<ProcessNetworkStats> ProcessInfo::GetNetworkUsageStats() const
    {
        return network_usage_stats;
    }
    
#ifdef __linux__
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
#endif

}


