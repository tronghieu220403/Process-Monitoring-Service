
#ifndef PROCESSMONITORING_PROCESS_PROCESSMEM_H_
#define PROCESSMONITORING_PROCESS_PROCESSMEM_H_

#include "ulti/everything.h"
#include "ulti/collections.h"

#ifdef _WIN32
#include "pdh/counter.h"
#endif
namespace pm
{
    class ProcessMemoryStats
    {
    private:

        int pid_ = 0;
        double mem_usage_ = 0;
        std::string process_name_;
        #ifdef _WIN32
            Counter counter_;
            FILETIME last_retrieved_time_ = { 0 };
        #elif __linux__
            time_t last_retrieved_time_ = 0;
        #endif
    public:
        ProcessMemoryStats();
        
        explicit ProcessMemoryStats(int pid);
        explicit ProcessMemoryStats(std::string& p_name, int pid);

        int GetPid() const;
        void SetPid(int pid);
        
        double GetMemoryUsage() const;
        void SetMemoryUsage(double mem_usage);

        #ifdef _WIN32
            FILETIME GetLastRetrievedTime() const;
            void SetLastRetrievedTime(FILETIME time);
        #elif __linux__
            time_t GetLastRetrievedTime() const;
            void SetLastRetrievedTime(time_t time);

        #endif

        UsageData GetMemoryUsageData() const;
        void UpdateAttributes();

    };

}

#endif