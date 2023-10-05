#include "processmem.h"

namespace pm
{

    ProcessMemoryStats::ProcessMemoryStats() = default;

    ProcessMemoryStats::ProcessMemoryStats(int pid) :
        pid_(pid)
    {
        #ifdef __linux__
            if (std::filesystem::is_directory("/proc/" + std::to_string(pid)) == false)
            {
                pid_ = 0;
                return;
            }
        #endif
    }
    
    ProcessMemoryStats::ProcessMemoryStats(std::string &p_name, int pid):
        pid_(pid), process_name_(p_name)
        #ifdef _WIN32
        , counter_(Counter(process_name_, pid, "Working Set - Private"))
        #endif

    {
        #ifdef _WIN32
        counter_.AddCounter();
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

    #ifdef _WIN32

    FILETIME ProcessMemoryStats::GetLastRetrievedTime() const
    {
        return last_retrieved_time_;
    }

    void ProcessMemoryStats::SetLastRetrievedTime(FILETIME time)
    {
        last_retrieved_time_ = time;
    }

    #elif __linux__

    time_t ProcessMemoryStats::GetLastRetrievedTime() const
    {
        return last_retrieved_time_;
    }

    void ProcessMemoryStats::SetLastRetrievedTime(time_t time)
    {
        last_retrieved_time_ = time;
    }

    #endif

    UsageData ProcessMemoryStats::GetMemoryUsageData() const
    {
        UsageData usage_data;
        usage_data.time = GetLastRetrievedTime();
        usage_data.data = GetMemoryUsage();
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
                SetMemoryUsage(0);
                return;
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
                    s >> ss >> usage;
                    break;
                }
            }
            file.close();
            SetLastRetrievedTime(time(0));
            SetMemoryUsage(usage / (1024 * 1024) );
        #endif
    }

}
