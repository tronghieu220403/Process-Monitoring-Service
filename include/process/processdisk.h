#ifndef PROCESSMONITORING_PROCESS_PROCESSDISK_H_
#define PROCESSMONITORING_PROCESS_PROCESSDISK_H_


#ifdef _VISUAL_STUDIO_WORKSPACE
#include "E:/Code/Github/Process-Monitoring/include/ulti/everything.h"
#else
#include "include/ulti/everything.h"
#endif

namespace pm
{
    class ProcessDiskStats
    {
    
    private:
        #ifdef _WIN32
            FILETIME last_time_;
            HANDLE process_handle_;
        #elif __linux__
            int pid_;
            clock_t last_time_;
        #endif
        
        unsigned long long last_io_;

        double last_speed_ = 0;

    public:
        static int num_processors_;

        ProcessDiskStats();
        #ifdef _WIN32
            explicit ProcessDiskStats(HANDLE p_handle);
        #elif __linux__
            explicit ProcessDiskStats(int pid);
        #endif

        unsigned long long GetCurrentCounter();
        double GetCurrentSpeed();
        double GetLastSpeed();

    };
}

#endif