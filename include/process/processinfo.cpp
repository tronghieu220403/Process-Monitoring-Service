#include "processinfo.h"


namespace pm
{

    ProcessInfo::ProcessInfo(std::string& p_name, int pid)
    {
        process_name_ = p_name;
        pid_ = pid;
        memory_usage_ = ProcessMemoryStats(pid);
        cpu_usage_ = ProcessCpuStats(pid);
        disk_usage_ = ProcessDiskStats(pid);
        network_usage_ = ProcessNetworkStats(pid);
        UpdateTime();
    };

    time_t ProcessInfo::UpdateTime()
    {
        this->time_ = time(0);
        return time_;
    }

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

        return memory_usage_.UpdateAttributes();
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
        ProcessInfo::UpdateTime();
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


