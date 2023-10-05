#include "processinfo.h"


namespace pm
{

    ProcessInfo::ProcessInfo(std::string& p_name, int pid)
    {
        process_name_ = p_name;
        pid_ = pid;
        memory_usage_stats = std::make_shared<ProcessMemoryStats>(p_name, pid);
        cpu_usage_stats = std::make_shared<ProcessCpuStats>(p_name, pid);
        disk_usage_stats = std::make_shared<ProcessDiskStats>(pid);
        network_usage_stats = std::make_shared<ProcessNetworkStats>(pid);
    };

    int ProcessInfo::GetPid() const
    {
        return pid_;
    };

    void ProcessInfo::UpdateDiskUsage()
    {
        #ifdef __linux__
            GetDiskUsageStats()->UpdateAttributes();
        #endif
    }

    void ProcessInfo::UpdateNetworkUsage()
    {
        #ifdef __linux__
            GetNetworkUsageStats()->UpdateAttributes();
        #endif
    }

    void ProcessInfo::UpdateCpuUsage()
    {
        GetCpuUsageStats()->UpdateAttributes();
    }

    void ProcessInfo::UpdateMemoryUsage()
    {
        GetMemoryUsageStats()->UpdateAttributes();
    }

    void ProcessInfo::UpdateAttributes()
    {
        UpdateDiskUsage();
        UpdateNetworkUsage();
        UpdateCpuUsage();
        UpdateMemoryUsage();
    }

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
    

}


