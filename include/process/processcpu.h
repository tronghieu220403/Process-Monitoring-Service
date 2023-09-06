#ifndef PROCESSMONITORING_PROCESS_PROCESSCPU_H_
#define PROCESSMONITORING_PROCESS_PROCESSCPU_H_


#ifdef _WIN32
#include <Windows.h>
#elif __linux__

#endif

namespace pm
{
    class ProcessCpuStats
    {
    
    private:

        double last_usage_percent_;
        static int num_processors_;

        #ifdef _WIN32
            ULARGE_INTEGER last_cpu_;
            ULARGE_INTEGER last_sys_cpu_; 
            ULARGE_INTEGER last_user_cpu_;

            HANDLE process_handle_;
        #elif __linux__

        #endif
    public:

        ProcessCpuStats();
        ProcessCpuStats(const ProcessCpuStats& pcs);
        ProcessCpuStats(const ProcessCpuStats&& pcs) = delete;

        ProcessCpuStats& operator=(const ProcessCpuStats& pcs);


        #ifdef _WIN32
            explicit ProcessCpuStats(HANDLE p_handle);
        #elif __linux__

        #endif

        double GetCurrentUsage();
        double GetLastUsage();
    };
}

#endif