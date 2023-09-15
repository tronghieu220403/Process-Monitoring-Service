
#ifndef PROCESSMONITORING_PROCESS_PROCESSMEM_H_
#define PROCESSMONITORING_PROCESS_PROCESSMEM_H_

#ifdef _VISUAL_STUDIO_WORKSPACE
#include "E:/Code/Github/Process-Monitoring/include/ulti/everything.h"
#else
#include "include/ulti/everything.h"
#endif

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