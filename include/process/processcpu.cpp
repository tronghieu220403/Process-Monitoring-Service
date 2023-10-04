#include "processcpu.h"


namespace pm
{
    ProcessCpuStats::ProcessCpuStats()
    {
    };

    ProcessCpuStats::ProcessCpuStats(std::string& process_name, int pid):
        pid_(pid), process_name_(process_name)
        #ifdef _WIN32
        , counter_(Counter(process_name_, pid_, "% Processor Time"))
        #endif
    {
        #ifdef _WIN32


        #elif __linux__
        if (std::filesystem::is_directory("/proc/" + std::to_string(pid)) == false)
        {
            pid_ = 0;
            return;
        }

        last_process_cpu_unit_ = GetSystemClockCycle();

        last_system_cpu_unit_ = GetClockCycle();
        #endif
    }

#ifdef _WIN32
    int ProcessCpuStats::GetNumberOfProcessors()
    {
        #ifdef _WIN32
            SYSTEM_INFO sysInfo;

            GetSystemInfo(&sysInfo);
            return sysInfo.dwNumberOfProcessors;

        #elif __linux__

            int n_processors = 0;
            std::ifstream file("/proc/stat");
            std::string line;
            while(std::getline(file, line))
            {
                if (line.find("cpu") == 0)
                {
                    n_processors++;
                }
            }
            n_processors--;
            file.close();
            return n_processors;

        #endif
    }
#endif

#ifdef __linux__

    unsigned long long ProcessCpuStats::GetSystemClockCycle()
    {
        #ifdef _WIN32
            
        #elif __linux__

            std::ifstream file("/proc/stat");
            std::string line;
            long long system_clock_cycle = 0;

            while(getline(file, line))
            {
                if (line.find("cpu") == 0)
                {
                    std::stringstream s(line);
                    std::string trash;
                    s >> trash;
                    
                    for (int i = 0; i < 9; i++)
                    {
                        int value; s >> value;                
                        system_clock_cycle += value;
                    }
                    break;
                }
            }
            file.close();
            return system_clock_cycle;

        #endif
    }

    unsigned long long ProcessCpuStats::GetClockCycle()
    {
        if (std::filesystem::is_directory("/proc/" + std::to_string(pid_)) == false)
        {
            return 0;
        }

        std::ifstream file("/proc/" + std::to_string(pid_) + "/stat");
        std::string line;
        std::getline(file, line);
        file.close();
        std::string new_line; 
        for (char c: line)
        {
            if (('0' <= c && c <= '9') || c == ' ')
            {
                new_line.push_back(c);
            }
        }
        std::stringstream s(new_line);
        long long trash;
        
        s >> trash;
        for (int i = 0; i < 10; i++)
        {
            s >> trash;
        }
        unsigned long long user_process_cpu_unit;
        unsigned long long sys_process_cpu_unit;

        s >> user_process_cpu_unit;
        s >> sys_process_cpu_unit;
        return user_process_cpu_unit + sys_process_cpu_unit;
    }

#endif

    void ProcessCpuStats::UpdateAttributes()
    {
        double percent = 0.0;

        #ifdef _WIN32
            SetLastRetrievedTime(Counter::GetLastQueryTime());
            SetLastUsagePercentage(counter_.GetValue() / num_processors_);
        #elif __linux__

            if (std::filesystem::is_directory("/proc/" + std::to_string(pid_)) == false)
            {
                return 0;
            }

            unsigned long now_process_cpu_unit = GetClockCycle();
            unsigned long long now_system_cpu_unit = GetSystemClockCycle();

            if (now_system_cpu_unit - last_system_cpu_unit_ != 0)
            {
                #ifdef _WIN32
                    percent = static_cast<double>((now_process_cpu_unit - last_process_cpu_unit_)/(now_system_cpu_unit - last_system_cpu_unit_));
                    percent /= num_processors_;
                    percent *= 100;
                #elif __linux__

                    percent = (double)(now_process_cpu_unit - last_process_cpu_unit_);
                    percent /= (double)(now_system_cpu_unit - last_system_cpu_unit_);
                    percent *= 100;

                #endif
            }
            else
            {
                percent = 0;
            }

            last_process_cpu_unit_ = now_process_cpu_unit;

            last_system_cpu_unit_ = now_system_cpu_unit;

            last_usage_percentage_ = percent;

        #endif

        return;
    }

    void ProcessCpuStats::SetLastUsagePercentage(double last_usage_percentage)
    {
        last_usage_percentage_ = last_usage_percentage;
    }

    double ProcessCpuStats::GetLastUsagePercentage() const
    {
        #ifdef _WIN32

        #elif __linux__
            if (std::filesystem::is_directory("/proc/" + std::to_string(pid_)) == false)
            {
                return 0.0;
            }
        #endif

        return last_usage_percentage_;
    }

#ifdef _WIN32
    FILETIME ProcessCpuStats::GetLastRetrievedTime() const
    {
        return last_retrieved_time_;
    }

    void ProcessCpuStats::SetLastRetrievedTime(FILETIME time)
    {
        last_retrieved_time_ = time;
    }

    UsageData ProcessCpuStats::GeCpuUsageData() const
    {
        UsageData usage_data;
        usage_data.time = GetLastRetrievedTime();
        usage_data.data - GetLastUsagePercentage();
        return usage_data;
    }

#endif

}