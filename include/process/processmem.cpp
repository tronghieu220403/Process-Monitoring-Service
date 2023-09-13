
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
            last_mem_ = 0;
            return;
        }

        process_handle_ = p_handle;
    };

    ProcessMemoryStats::ProcessMemoryStats(const HANDLE process_handle, double last_mem)
        : process_handle_(process_handle), last_mem_(last_mem){}

#elif __linux__
    ProcessMemoryStats::ProcessMemoryStats(int pid):
        pid_(pid)
    {
        
    };

#endif

    double ProcessMemoryStats::GetCurrentUsage()
    {
        #ifdef _WIN32
            if (GetProcessId(process_handle_) == NULL)
            {
                last_mem_ = 0;
                return last_mem_;
            }
            PROCESS_MEMORY_COUNTERS_EX pmc{};

            GetProcessMemoryInfo(process_handle_, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
            last_mem_ = double(pmc.WorkingSetSize) / (1024*1024);
            return last_mem_;
        #elif __linux__
            if (std::filesystem::is_directory("/proc/" + std::to_string(pid_) + "/status") == false)
            {
                last_mem_ = 0;
                return 0;
            }
            std::ifstream file("/proc/" + std::to_string(pid_) + "/status");
            std::string line;
            double usage;
            while(std::getline(file, line))
            {
                if (line.substr(0, std::string("VmRSS").size()) == "VmRSS")
                {
                    std::stringstream s(line);
                    std::string ss;
                    s >> ss >> last_mem_;
                    break;
                }
            }
            file.close();
            last_mem_ /= 1024;
        #endif
            return last_mem_;
    }

    double ProcessMemoryStats::GetLastUsage()
    {
        #ifdef _WIN32
            if (GetProcessId(process_handle_) == NULL)
            {
                last_mem_ = 0;
            }
        #elif __linux__
            if (std::filesystem::is_directory("/proc/" + std::to_string(pid_) + "/status") == false)
            {
                last_mem_ = 0;
            }
        #endif

        return last_mem_;
    }

}
