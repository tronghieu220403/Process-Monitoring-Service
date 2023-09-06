
#ifndef PROCESSMONITORING_PROCESS_PROCESSNETWORK_H_
#define PROCESSMONITORING_PROCESS_PROCESSNETWORK_H_

#ifdef _WIN32
    #include <Windows.h>
    #include <WinSock2.h>
    #include <iphlpapi.h>
#elif __linux__

#endif

namespace pm
{
    class ProcessNetworkStats
    {
    private:
        #ifdef _WIN32
            HANDLE process_handle_;
        #elif __linux__

        #endif

        long long last_data_recv_;
        long long last_data_sent_;
        double last_speed_;
        
    public:
        ProcessNetworkStats();
        
        #ifdef _WIN32
            explicit ProcessNetworkStats(HANDLE p_handle);
            ProcessNetworkStats(const HANDLE& process_handle_, long long last_data_recv_, long long last_data_sent_, double last_speed_);
        #elif __linux__

        #endif
        ProcessNetworkStats(const ProcessNetworkStats& pns);
        ProcessNetworkStats(const ProcessNetworkStats&& pns) = delete;

        ProcessNetworkStats& operator=(const ProcessNetworkStats& pns);

        double GetCurrentSpeed();
        double GetLastSpeed();
        
        
    };

}

#endif