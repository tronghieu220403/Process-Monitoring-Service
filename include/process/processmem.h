
#ifndef PROCESSMONITORING_PROCESS_PROCESSMEM_H_
#define PROCESSMONITORING_PROCESS_PROCESSMEM_H_

#ifndef _VISUAL_STUDIO_WORKSPACE
#define UNICODE
#define _UNICODE
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

#endif

#include <string.h>

#ifdef _WIN32
	#include <WS2tcpip.h>
	#include <TlHelp32.h>
	#include <Windows.h>
    #include <Psapi.h>
#elif __linux__

#endif

#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>

namespace pm
{
    class ProcessMemoryStats
    {
    private:
        #ifdef _WIN32
            HANDLE process_handle_ = nullptr;
        #elif __linux__
            int pid_;
        #endif

        double last_mem_ = 0;
        
    public:
        ProcessMemoryStats();
        
        #ifdef _WIN32
            explicit ProcessMemoryStats(HANDLE p_handle);
            ProcessMemoryStats(const HANDLE process_handle, double last_mem);
        #elif __linux__
            explicit ProcessMemoryStats(int pid);
        #endif

        double GetCurrentUsage();
        double GetLastUsage();
        
    };

}

#endif