
#ifdef _VISUAL_STUDIO_WORKSPACE
#include "processnetwork.h"
#else
#include "include/process/processnetwork.h"
#endif


namespace pm
{

ProcessNetworkStats::ProcessNetworkStats()
{
    #ifdef _WIN32
        process_handle_ = 0;
    #elif __linux__

    #endif

    long long last_data_recv_ = 0;
    long long last_data_sent_ = 0;
    double last_speed_ = 0;
};

ProcessNetworkStats::ProcessNetworkStats(ProcessNetworkStats& pns)
{
    #ifdef _WIN32
        process_handle_ = pns.process_handle_;
    #elif __linux__

    #endif

    last_data_recv_ = pns.last_data_recv_;
    last_data_sent_ = pns.last_data_sent_;
    last_speed_ = pns.last_speed_;

};

ProcessNetworkStats::ProcessNetworkStats(ProcessNetworkStats&& pns)
{
    #ifdef _WIN32
        process_handle_ = pns.process_handle_;
    #elif __linux__

    #endif

    last_data_recv_ = pns.last_data_recv_;
    last_data_sent_ = pns.last_data_sent_;
    last_speed_ = pns.last_speed_;

};


ProcessNetworkStats::ProcessNetworkStats(HANDLE p_handle)
{
    #ifdef _WIN32
        if (GetProcessId(p_handle) == NULL)
        {
            return;
        }
    #elif __linux__

    #endif
    process_handle_ = p_handle;
};

double ProcessNetworkStats::GetCurrentSpeed()
{
#ifdef _WIN32
    if (GetProcessId(process_handle_) == NULL)
    {
        return 0.0;
    }
    last_speed_ = 0.0;
#elif __linux__
    last_speed_ = 0.0;
#endif
    return last_speed_;
}

double ProcessNetworkStats::GetLastSpeed()
{
    #ifdef _WIN32
        if (GetProcessId(process_handle_) == NULL)
        {
            return 0.0;
        }
    #elif __linux__

    #endif

    return last_speed_;
}

}

pm::ProcessNetworkStats::ProcessNetworkStats(const HANDLE& process_handle_, long long last_data_recv_, long long last_data_sent_, double last_speed_)
    : process_handle_(process_handle_), last_data_recv_(last_data_recv_), last_data_sent_(last_data_sent_), last_speed_(last_speed_)
{
}
