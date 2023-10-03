
#ifndef PROCESSMONITORING_PROCESS_PROCESSMEM_H_
#define PROCESSMONITORING_PROCESS_PROCESSMEM_H_

#include "ulti/everything.h"

namespace pm
{
    class ProcessMemoryStats
    {
    private:

        int pid_ = 0;
        double mem_usage_ = 0;
        std::string process_name_;
        
    public:
        ProcessMemoryStats();
        
        explicit ProcessMemoryStats(int pid);
        explicit ProcessMemoryStats(std::string& p_name, int pid);

        int GetPid() const;
        void SetPid(int pid);
        
        unsigned long long GetMemoryUsage() const;
        void SetMemoryUsage(unsigned long long mem_usage);

        double UpdateAttributes();
        
    };

}

#endif