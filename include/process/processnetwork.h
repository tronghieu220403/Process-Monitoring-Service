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
        float last_speed_;
    public:
        ProcessNetworkStats();
        ProcessNetworkStats(HANDLE p_handle);

        float GetCurrentSpeed();
        float GetLastSpeed();
    };

}

#endif