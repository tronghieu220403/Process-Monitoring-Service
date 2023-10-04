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
    }
    
    ProcessMemoryStats::ProcessMemoryStats(std::string &p_name, int pid):
        pid_(pid), process_name_(p_name)
        #ifdef _WIN32
        , counter_(Counter(process_name_, pid, "Working Set - Private"))
        #endif

    {
        #ifdef _WIN32

 
        #elif __linux__

        #endif
    };

    int ProcessMemoryStats::GetPid() const
    {
        return pid_;
    }

    void ProcessMemoryStats::SetPid(int pid)
    {
        pid_ = pid;
    }

    double ProcessMemoryStats::GetMemoryUsage() const
    {
        return mem_usage_;
    }

    void ProcessMemoryStats::SetMemoryUsage(double mem_usage)
    {
        mem_usage_ = mem_usage;
    }

    FILETIME ProcessMemoryStats::GetLastRetrievedTime() const
    {
        return last_retrieved_time_;
    }

    void ProcessMemoryStats::SetLastRetrievedTime(FILETIME time)
    {
        last_retrieved_time_ = time;
    }

    UsageData ProcessMemoryStats::GetMemoryUsageData() const
    {
        UsageData usage_data;
        usage_data.time = GetLastRetrievedTime();
        usage_data.data - GetMemoryUsage();
        return usage_data;
    }

    void ProcessMemoryStats::UpdateAttributes()
    {
        #ifdef _WIN32
            SetLastRetrievedTime(Counter::GetLastQueryTime());
            SetMemoryUsage(counter_.GetValue() / (1024 * 1024));
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
            mem_usage_ /= 1024 * 1024;
        #endif
    }

}
