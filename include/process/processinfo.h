#ifndef PROCESSMONITORING_PROCESS_PROCESSINFO_H_
#define PROCESSMONITORING_PROCESS_PROCESSINFO_H_

#include "ulti/everything.h"

    #include "process/processmem.h"
    #include "process/processcpu.h"
    #include "process/processdisk.h"
    #include "process/processnetwork.h"
    #include "ulti/collections.h"
#ifdef _WIN32
    #include "pdh/counter.h"
    #include "etw/consumer.h"
#endif
namespace pm
{

    class ProcessInfo
    {
    private:
        int pid_ = 0;
        std::string process_name_;

        std::shared_ptr<ProcessMemoryStats> memory_usage_stats;
        std::shared_ptr<ProcessCpuStats> cpu_usage_stats;
        std::shared_ptr<ProcessDiskStats> disk_usage_stats;
        std::shared_ptr<ProcessNetworkStats> network_usage_stats;

    public:

        ProcessInfo() = default;
        explicit ProcessInfo(std::string& p_name, int pid);

        int GetPid() const;
        #ifdef __linux__
            time_t GetTime() const;
        #endif

        std::shared_ptr<ProcessMemoryStats> GetMemoryUsageStats() const;
        std::shared_ptr<ProcessCpuStats> GetCpuUsageStats() const;
        std::shared_ptr<ProcessDiskStats> GetDiskUsageStats() const;
        std::shared_ptr<ProcessNetworkStats> GetNetworkUsageStats() const;

        void UpdateDiskUsage();
        void UpdateNetworkUsage();
        void UpdateCpuUsage();
        void UpdateMemoryUsage();

        void UpdateAttributes();
    };
}

#endif