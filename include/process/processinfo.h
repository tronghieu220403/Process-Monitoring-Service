#ifndef PROCESSMONITORING_PROCESS_PROCESSINFO_H_
#define PROCESSMONITORING_PROCESS_PROCESSINFO_H_

#include "ulti/everything.h"

    #include "process/processmem.h"
    #include "process/processcpu.h"
    #include "process/processdisk.h"
    #include "process/processnetwork.h"
    #include "ulti/collections.h"

namespace pm
{

    class ProcessInfo
    {
    private:
        int pid_ = 0;
        std::string process_name_;
        time_t time_ = 0;
        ProcessMemoryStats memory_usage_{};
        ProcessCpuStats cpu_usage_{};
        ProcessDiskStats disk_usage_{};
        ProcessNetworkStats network_usage_{};

        MonitoringComponent last_usage_;

        time_t UpdateTime();
        double UpdateCpuUsage();
        double UpdateMemoryUsage();
        double UpdateDiskUsage();
        double UpdateNetworkUsage();

    public:

        ProcessInfo() = default;
        explicit ProcessInfo(std::string& p_name, int pid);

        int GetPid() const;
                
        time_t GetTime() const;
        double GetCpuUsage() const;
        double GetMemoryUsage() const;
        double GetDiskUsage() const;
        double GetNetworkUsage() const;
        MonitoringComponent GetUsage() const;

        void UpdateAttributes();
    };
}

#endif