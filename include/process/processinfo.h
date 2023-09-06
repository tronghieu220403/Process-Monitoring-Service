#ifndef PROCESSMONITORING_PROCESS_PROCESSINFO_H_
#define PROCESSMONITORING_PROCESS_PROCESSINFO_H_

#include <string.h>

#if defined(_WIN32)
	#include <WS2tcpip.h>
	#include <TlHelp32.h>
	#include <Windows.h>
    #include <Psapi.h>

#elif __linux__
	#include <sys/wait.h>
	#include <dirent.h>
#endif

#include <string>
#include <fstream>

#ifdef _VISUAL_STUDIO_WORKSPACE
    #include "E:/Code/Github/Process-Monitoring/include/process/processcpu.h"
    #include "E:/Code/Github/Process-Monitoring/include/process/processdisk.h"
    #include "E:/Code/Github/Process-Monitoring/include/process/processnetwork.h"
    #include "E:/Code/Github/Process-Monitoring/include/ulti/collections.h"
#else
    #include "include/process/processcpu.h"
    #include "include/process/processdisk.h"
    #include "include/process/processnetwork.h"
    #include "include/ulti/collections.h"
#endif


namespace pm
{

    class ProcessInfo
    {
    private:
        int pid_;
        time_t time_;
        double memory_usage_;
        ProcessCpuStats cpu_usage_;
        ProcessDiskStats disk_usage_;
        ProcessNetworkStats network_usage_;

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

        ProcessInfo();
        #ifdef _WIN32
            explicit ProcessInfo(HANDLE process_handle_);
        #elif __linux__

        #endif

        ProcessInfo(const ProcessInfo& pi);         // copy constructor
        explicit ProcessInfo(const ProcessInfo&& pi);        // move constructor

        ProcessInfo& operator=(const ProcessInfo& pi);

        int GetPid();
                
        time_t GetTime();
        double GetCpuUsage();
        double GetMemoryUsage();
        double GetDiskUsage();
        double GetNetworkUsage();
        MonitoringComponent GetUsage();

        void UpdateAttributes();

    };
}

#endif