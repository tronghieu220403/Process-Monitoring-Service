
#ifndef PROCESSMONITORING_PROCESS_PROCESSNETWORK_H_
#define PROCESSMONITORING_PROCESS_PROCESSNETWORK_H_

#ifdef _VISUAL_STUDIO_WORKSPACE
#include "E:/Code/Github/Process-Monitoring/include/ulti/everything.h"
#else
#include "include/ulti/everything.h"
#endif


namespace pm
{
    class ProcessNetworkStats
    {
    private:
        #ifdef _WIN32
            HANDLE process_handle_ = nullptr;
        #elif __linux__
            int pid_ = 0;
        #endif

        long long last_data_recv_ = 0;
        long long last_data_sent_ = 0;
        double last_speed_ = 0;
        
    public:
        ProcessNetworkStats();
        
        #ifdef _WIN32
            explicit ProcessNetworkStats(HANDLE p_handle);
            ProcessNetworkStats(const HANDLE& process_handle_, long long last_data_recv_, long long last_data_sent_, double last_speed_);
        #elif __linux__
            explicit ProcessNetworkStats(int pid);
        #endif

        double GetCurrentSpeed();
        double GetLastSpeed();
        
    };

}

#endif