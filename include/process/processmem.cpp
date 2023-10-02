#include "processmem.h"

namespace pm
{

    ProcessMemoryStats::ProcessMemoryStats() = default;

    ProcessMemoryStats::ProcessMemoryStats(int pid)
    {
        #ifdef _WIN32
            pid_ = pid;
        #elif __linux__

            if (std::filesystem::is_directory("/proc/" + std::to_string(pid)) == false)
            {
                pid_ = 0;
                return;
            }
            pid_ = pid;
        #endif
    };

    unsigned long long ProcessMemoryStats::GetMemoryUsage()
    {
        return mem_usage_;
    }

    void SetMemoryUsage(unsigned long long mem_usage)
    {
        mem_usage_ = mem_usage;
    }

    double ProcessMemoryStats::UpdateAttributes()
    {
        #ifdef _WIN32
            
        #elif __linux__
            
            if (std::filesystem::is_directory("/proc/" + std::to_string(pid_)) == false)
            {
                mem_usage_ = 0;
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
                    s >> ss >> mem_usage_;
                    break;
                }
            }
            file.close();
            mem_usage_ /= 1024;
        #endif
            return mem_usage_;
    }

}
