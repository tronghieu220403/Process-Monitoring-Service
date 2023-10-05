#ifndef PROCESSMONITORING_PROCESS_PROCESSCPU_H_
#define PROCESSMONITORING_PROCESS_PROCESSCPU_H_

#include "ulti/everything.h"
#include "ulti/collections.h"
#ifdef _WIN32
#include "pdh/counter.h"
#endif

namespace pm
{
    class ProcessCpuStats
    {
    
    public:
        #ifdef _WIN32
        static int GetNumberOfProcessors();
        #endif

    private:

        int pid_;
        std::string process_name_;
        double last_usage_percentage_ = 0;
        
        #ifdef _WIN32
            Counter counter_;
            FILETIME last_retrieved_time_;
            inline static int num_processors_ = ProcessCpuStats::GetNumberOfProcessors();
        #elif __linux__
            time_t last_retrieved_time_;
            unsigned long long last_process_cpu_unit_;
            unsigned long long last_system_cpu_unit_;

        #endif


    public:

        ProcessCpuStats();

        explicit ProcessCpuStats(std::string& process_name_, int pid);

        void UpdateAttributes();

        void SetLastUsagePercentage(double last_usage_percentage);
        double GetLastUsagePercentage() const;
        
        #ifdef _WIN32
            FILETIME GetLastRetrievedTime() const;
            void SetLastRetrievedTime(FILETIME time);

        #elif __linux__

            time_t GetLastRetrievedTime() const;
            void SetLastRetrievedTime(time_t time);

            unsigned long long GetClockCycle();
            unsigned long long GetSystemClockCycle();

        #endif

        UsageData GeCpuUsageData() const;


    };
}

#endif