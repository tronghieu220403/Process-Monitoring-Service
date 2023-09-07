
#ifndef PROCESSMONITORING_PROCESS_PROCESSNETWORK_H_
#define PROCESSMONITORING_PROCESS_PROCESSNETWORK_H_

#ifndef _VISUAL_STUDIO_WORKSPACE
#define UNICODE
#define _UNICODE
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")
#endif

#include <string>

#ifdef _WIN32
    #include <Windows.h>
    #include <WinSock2.h>
    #include <WS2tcpip.h>
    #include <iphlpapi.h>
#elif __linux__

#endif

#ifndef _VISUAL_STUDIO_WORKSPACE
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")
#endif


namespace pm
{
    class ProcessNetworkStats
    {
    private:
        #ifdef _WIN32
            HANDLE process_handle_ = nullptr;
        #elif __linux__

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

        #endif

        double GetCurrentSpeed();
        double GetLastSpeed();
        
    };

}

#endif