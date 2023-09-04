#ifndef PROCESSMONITORING_PROCESS_PROCESSNETWORK_H_
#define PROCESSMONITORING_PROCESS_PROCESSNETWORK_H_

#define UNICODE
#define _UNICODE

#ifdef _WIN32
    #pragma comment(lib, "Ws2_32.lib")
    #pragma comment(lib, "iphlpapi.lib")
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
        HANDLE process_handle_;
        long long last_data_recv_;
        long long last_data_sent_;
        double last_speed_;
    public:
        ProcessNetworkStats();
        ProcessNetworkStats(HANDLE p_handle);

        double GetCurrentSpeed();
        double GetLastSpeed();
    };

}

#endif