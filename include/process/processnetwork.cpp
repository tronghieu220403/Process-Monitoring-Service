#include <process/processnetwork.h>

namespace pm
{

ProcessNetworkStats::ProcessNetworkStats()
{

};

ProcessNetworkStats::ProcessNetworkStats(HANDLE p_handle)
{
    #ifdef _WIN32
        if (GetProcessId(p_handle) == NULL)
        {
            return;
        }
    #else

    #endif
    process_handle_ = p_handle;
};

float ProcessNetworkStats::GetCurrentSpeed()
{
#ifdef _WIN32
    if (GetProcessId(process_handle_) == NULL)
    {
        return 0.0;
    }
    last_speed_ = 0.0;
#else
    last_speed_ = 0.0;
#endif
    return last_speed_;
}

float ProcessNetworkStats::GetLastSpeed()
{
    #ifdef _WIN32
        if (GetProcessId(process_handle_) == NULL)
        {
            return 0.0;
        }
    #else

    #endif

    return last_speed_;
}

}