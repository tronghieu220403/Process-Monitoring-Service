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
        time_t time_ = 0;
        ProcessMemoryStats memory_usage_{};
        ProcessCpuStats cpu_usage_{};
        ProcessDiskStats disk_usage_{};
        ProcessNetworkStats network_usage_{};

        MonitoringComponent last_usage_;

        #ifdef _WIN32
            HANDLE process_handle_;
        #elif __linux__

        #endif

        time_t UpdateTime();
        double UpdateCpuUsage();
        double UpdateMemoryUsage();
        double UpdateDiskUsage();
        double UpdateNetworkUsage();

    public:

        ProcessInfo() = default;
        #ifdef _WIN32
            explicit ProcessInfo(HANDLE process_handle_);
        #elif __linux__
            explicit ProcessInfo(int pid);
        #endif

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