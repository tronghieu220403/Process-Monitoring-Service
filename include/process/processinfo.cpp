#include "processinfo.h"


namespace pm
{

    ProcessInfo::ProcessInfo(std::string& p_name, int pid)
    {
        process_name_ = p_name;
        pid_ = pid;
        memory_usage_ = ProcessMemoryStats(p_name, pid); // need update
        cpu_usage_ = ProcessCpuStats(pid); // need update
        disk_usage_ = ProcessDiskStats(pid); // need update
        network_usage_ = ProcessNetworkStats(pid);
        UpdateTime();
    };

#ifdef __linux__
    time_t ProcessInfo::UpdateTime()
    {
        this->time_ = time(0);
        return time_;
    }
#endif

    int ProcessInfo::GetPid() const
    {
        return pid_;
    };

    double ProcessInfo::UpdateCpuUsage()
    {
        #ifdef _WIN32

        #elif __linux__

        #endif

        cpu_usage_.UpdateAttributes();

        return cpu_usage_.GetLastUsagePercentage();
    };

    double ProcessInfo::UpdateMemoryUsage()
    {
        #ifdef _WIN32

        #elif __linux__

        #endif

        return memory_usage_.UpdateAttributes(); // ?
    };

    double ProcessInfo::UpdateDiskUsage()
    {
        #ifdef _WIN32

        #elif __linux__

        #endif

        return disk_usage_.GetCurrentSpeed();
    };

    double ProcessInfo::UpdateNetworkUsage()
    {
        #ifdef _WIN32

        #elif __linux__

        #endif

        return network_usage_.GetCurrentSpeed();
    };

    void ProcessInfo::UpdateAttributes()
    {
        #ifdef __linux__
        ProcessInfo::UpdateTime();
        last_usage_.cpu_usage.data = UpdateCpuUsage();
        last_usage_.mem_usage.data = UpdateMemoryUsage();
        last_usage_.disk_usage.data = UpdateDiskUsage();
        last_usage_.network_usage.data = UpdateNetworkUsage();
        #elif _WIN32
            Counter::UpdateQuery();
        #endif
    }

#ifdef __linux__
    time_t ProcessInfo::GetTime() const
    {
        return time_;
    }
#endif

    double ProcessInfo::GetCpuUsage() const
    {
        return last_usage_.cpu_usage.data;
    }

    double ProcessInfo::GetMemoryUsage() const
    {
        return last_usage_.mem_usage.data;
    }

    double ProcessInfo::GetDiskUsage() const
    {
        return last_usage_.disk_usage.data;
    }

    double ProcessInfo::GetNetworkUsage() const
    {
        return last_usage_.network_usage.data;
    }

    MonitoringComponent ProcessInfo::GetUsage() const
    {
        return last_usage_;
    }

}


