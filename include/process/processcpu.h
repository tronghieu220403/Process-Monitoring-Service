#ifndef PROCESSMONITORING_PROCESS_PROCESSCPU_H_
#define PROCESSMONITORING_PROCESS_PROCESSCPU_H_


#ifdef _WIN32
#include "windows.h"
#elif __linux__

#endif

namespace pm
{
    class ProcessCpuStats
    {
    
    private:
        ULARGE_INTEGER last_cpu_;
        ULARGE_INTEGER last_sys_cpu_; 
        ULARGE_INTEGER last_user_cpu_;

        double last_usage_percent_;
        #ifdef _WIN32
            HANDLE process_handle_;
        #elif __linux__

        #endif
    public:
        static int num_processors_;

        ProcessCpuStats();
        
        #ifdef _WIN32
            ProcessCpuStats(HANDLE p_handle);
        #elif __linux__

        #endif

        double GetCurrentUsage();
        double GetLastUsage();
    };
}

#endif