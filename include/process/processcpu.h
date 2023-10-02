#ifndef PROCESSMONITORING_PROCESS_PROCESSCPU_H_
#define PROCESSMONITORING_PROCESS_PROCESSCPU_H_

#ifdef _VISUAL_STUDIO_WORKSPACE
#include "ulti/everything.h"
#else
#include "include/ulti/everything.h"
#endif

namespace pm
{
    class ProcessCpuStats
    {
    
    private:

        int pid_;

        double last_usage_percent_ = 0;

        #ifdef _WIN32
            FILETIME last_retrieved_time_;
            inline static int num_processors_ = 0;
        #elif __linux__
            unsigned long long last_process_cpu_unit_;
            unsigned long long last_system_cpu_unit_;

        #endif


    public:

        ProcessCpuStats();
        ProcessCpuStats(const ProcessCpuStats& pcs);
        ProcessCpuStats(const ProcessCpuStats&& pcs) = delete;

        ProcessCpuStats& operator=(const ProcessCpuStats& pcs);

        explicit ProcessCpuStats(int pid);

        static int GetNumberOfProcessors();

        void UpdateAttributes();
        double GetLastUsagePercentage();
        
        #ifdef _WIN32
            FILETIME GetLastRetrievedTime();
        #elif __linux__

            unsigned long long GetClockCycle();
            unsigned long long GetSystemClockCycle();

        #endif



    };
}

#endif