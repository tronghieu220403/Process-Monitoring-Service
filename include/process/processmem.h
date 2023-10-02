
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
        
    public:
        ProcessMemoryStats();
        
        explicit ProcessMemoryStats(int pid);

        unsigned long long GetMemoryUsage();
        void SetMemoryUsage(unsigned long long mem_usage);

        double UpdateAttributes();
        
    };

}

#endif