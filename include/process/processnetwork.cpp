#include "process/processnetwork.h"

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