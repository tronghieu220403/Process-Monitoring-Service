#ifndef PROCESSMONITORING_PROCESS_PROCESSNETWORK_H_
#define PROCESSMONITORING_PROCESS_PROCESSNETWORK_H_

#ifdef _WIN32
    #include <Windows.h>
    #include <WinSock2.h>
    #include <iphlpapi.h>
    #pragma comment(lib, "Ws2_32.lib")
    #pragma comment(lib, "iphlpapi.lib")

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
        explicit ProcessNetworkStats(HANDLE p_handle);

        ProcessNetworkStats(ProcessNetworkStats& pns);
        ProcessNetworkStats(ProcessNetworkStats&& pns) noexcept;

        double GetCurrentSpeed();
        double GetLastSpeed();
        ProcessNetworkStats(const HANDLE& process_handle_, long long last_data_recv_, long long last_data_sent_, double last_speed_)
            : process_handle_(process_handle_), last_data_recv_(last_data_recv_), last_data_sent_(last_data_sent_), last_speed_(last_speed_);
    };

}

#endif