#ifndef PROCESSMONITORING_PROCESS_PROCESSINFO_H_
#define PROCESSMONITORING_PROCESS_PROCESSINFO_H_

#define UNICODE
#define _UNICODE

#include <string.h>

#if defined(_WIN32)
	#include <WS2tcpip.h>
	#include <TlHelp32.h>
	#include "windows.h"
    #include "psapi.h"

#elif __linux__
	#include <sys/wait.h>
	#include <dirent.h>
#endif

#include <string>
#include <fstream>

#include "process/processcpu.h"
#include "process/processdisk.h"
#include "process/processnetwork.h"
#include "ulti/collections.h"

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
        float UpdateCpuUsage();
        double UpdateMemoryUsage();
        float UpdateDiskUsage();
        float UpdateNetworkUsage();

    public:

        ProcessInfo();
        #ifdef _WIN32
            explicit ProcessInfo(HANDLE process_handle_);
        #elif __linux__

        #endif
        int GetPid();
                
        time_t GetTime();
        float GetCpuUsage();
        double GetMemoryUsage();
        float GetDiskUsage();
        float GetNetworkUsage();
        MonitoringComponent GetUsage();

        void UpdateAttributes();

    };
}

#endif