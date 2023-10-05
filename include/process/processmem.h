
#ifndef PROCESSMONITORING_PROCESS_PROCESSMEM_H_
#define PROCESSMONITORING_PROCESS_PROCESSMEM_H_

#include "ulti/everything.h"
#include "pdh/counter.h"

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

            UsageData GetMemoryUsageData() const;
        #endif

        void UpdateAttributes();
        
    };

}

#endif