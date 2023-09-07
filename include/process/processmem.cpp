
#ifdef _VISUAL_STUDIO_WORKSPACE
#include "processmem.h"
#else
#include "include/process/processmem.h"
#endif


namespace pm
{

    ProcessMemoryStats::ProcessMemoryStats() = default;

#ifdef _WIN32
    ProcessMemoryStats::ProcessMemoryStats(HANDLE p_handle)
    {
        if (GetProcessId(p_handle) == NULL)
        {
            return;
        }

        process_handle_ = p_handle;
    };

    ProcessMemoryStats::ProcessMemoryStats(const HANDLE& process_handle_, double last_mem)
        : process_handle_(process_handle_), last_mem_(last_mem){};

#endif

    double ProcessMemoryStats::GetCurrentSpeed()
    {
        #ifdef _WIN32
            if (GetProcessId(process_handle_) == NULL)
            {
                return 0.0;
            }
            PROCESS_MEMORY_COUNTERS_EX pmc{};

            if (GetProcessId(process_handle_) == NULL)
            {
                return 0;
            }

            GetProcessMemoryInfo(process_handle_, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
            last_mem_ = double(pmc.WorkingSetSize) / 1024;
            return last_mem_;
        #elif __linux__
            last_mem_ = 0.0;
        #endif
            return last_mem_;
    }

    double ProcessMemoryStats::GetLastSpeed()
    {
        #ifdef _WIN32
            if (GetProcessId(process_handle_) == NULL)
            {
                return 0.0;
            }
        #elif __linux__

        #endif

        return last_mem_;
    }

}
