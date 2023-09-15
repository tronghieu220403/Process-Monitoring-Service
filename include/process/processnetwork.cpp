
#ifdef _VISUAL_STUDIO_WORKSPACE
#include "processnetwork.h"
#else
#include "include/process/processnetwork.h"
#endif


namespace pm
{

    ProcessNetworkStats::ProcessNetworkStats() = default;

#ifdef _WIN32
    ProcessNetworkStats::ProcessNetworkStats(HANDLE p_handle)
    {
        if (GetProcessId(p_handle) == NULL)
        {
            return;
        }

        process_handle_ = p_handle;
    };

    ProcessNetworkStats::ProcessNetworkStats(const HANDLE& process_handle_, long long last_data_recv_, long long last_data_sent_, double last_speed_)
        : process_handle_(process_handle_), last_data_recv_(last_data_recv_), last_data_sent_(last_data_sent_), last_speed_(last_speed_){};

#elif __linux__
    ProcessNetworkStats::ProcessNetworkStats(int pid)
    {
        if (std::filesystem::is_directory("/proc/" + std::to_string(pid)) == false)
        {
            pid_ = 0;
            return;
        }
    };

#endif

    double ProcessNetworkStats::GetCurrentSpeed()
    {
        #ifdef _WIN32
            if (GetProcessId(process_handle_) == NULL)
            {
                last_speed_ = 0.0;
            }
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
                last_speed_ = 0;
            }
        #elif __linux__

        #endif

        return last_speed_;
    }

}
