#ifndef PROCESSMONITORING_PROCESS_PROCESSCPU_H_
#define PROCESSMONITORING_PROCESS_PROCESSCPU_H_

#ifdef _VISUAL_STUDIO_WORKSPACE
#include "E:/Code/Github/Process-Monitoring/include/ulti/everything.h"
#else
#include "include/ulti/everything.h"
#endif

namespace pm
{
    class ProcessCpuStats
    {
    
    private:

        double last_usage_percent_ = 0;
        int num_processors_ = 0;

        #ifdef _WIN32
            HANDLE process_handle_ = nullptr;

        #elif __linux__
            int pid_;

        #endif

        unsigned long long last_process_cpu_unit_;
        
        unsigned long long last_system_cpu_unit_;

    public:

        ProcessCpuStats();
        ProcessCpuStats(const ProcessCpuStats& pcs);
        ProcessCpuStats(const ProcessCpuStats&& pcs) = delete;

        ProcessCpuStats& operator=(const ProcessCpuStats& pcs);


        #ifdef _WIN32
            explicit ProcessCpuStats(HANDLE p_handle);
        #elif __linux__
            explicit ProcessCpuStats(int pid);
        #endif

        unsigned long long GetClockCycle();

        double GetCurrentUsage();
        double GetLastUsage();

        static int GetNumberOfProcessors();
        unsigned long long GetSystemClockCycle();

    };
}

#endif