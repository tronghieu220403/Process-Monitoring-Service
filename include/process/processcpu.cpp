
#ifdef _VISUAL_STUDIO_WORKSPACE
#include "processcpu.h"
#else
#include "include/process/processcpu.h"
#endif


namespace pm
{
    ProcessCpuStats::ProcessCpuStats()
    {
        #ifdef _WIN32

            if (num_processors_ == 0)
            {
                num_processors_ = GetNumberOfProcessors();
            }

        #elif __linux__

        #endif
    };

    ProcessCpuStats::ProcessCpuStats(const ProcessCpuStats& pcs) = default;

    ProcessCpuStats& ProcessCpuStats::operator=(const ProcessCpuStats& pcs) = default;


#ifdef _WIN32
    ProcessCpuStats::ProcessCpuStats(HANDLE p_handle)
    {
        if (GetProcessId(p_handle) == NULL)
        {
            return;
        }
        process_handle_ = p_handle;

        num_processors_ = GetNumberOfProcessors();

        last_system_cpu_unit_ = GetSystemClockCycle();

        last_process_cpu_unit_ = GetClockCycle();

    };
#endif

#ifdef __linux__
    ProcessCpuStats::ProcessCpuStats(int pid)
    {
        if (std::filesystem::is_directory("/proc/" + std::to_string(pid)) == false)
        {
            pid_ = 0;
            return;
        }

        pid_ = pid;

        num_processors_ = GetNumberOfProcessors();

        last_process_cpu_unit_ = GetSystemClockCycle();

        last_system_cpu_unit_ = GetClockCycle();

    }

#endif

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

    unsigned long long ProcessCpuStats::GetSystemClockCycle()
    {
        #ifdef _WIN32
            
            FILETIME ftime;
            ULARGE_INTEGER now_time;
            GetSystemTimeAsFileTime(&ftime);
            memcpy(&now_time, &ftime, sizeof(FILETIME));
            return now_time.QuadPart;

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
        #ifdef _WIN32

            if (GetProcessId(process_handle_) == NULL)
            {
                return 0.0;
            }

            FILETIME ftime;
            FILETIME fsys;
            FILETIME fuser;

            ULARGE_INTEGER sys;
            ULARGE_INTEGER user;

            GetProcessTimes(process_handle_, &ftime, &ftime, &fsys, &fuser);
            memcpy(&sys, &fsys, sizeof(FILETIME));
            memcpy(&user, &fuser, sizeof(FILETIME));
            
            return sys.QuadPart + user.QuadPart;

        #elif __linux__

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
        #endif
    }

    double ProcessCpuStats::GetCurrentUsage()
    {
        double percent = 0.0;

        #ifdef _WIN32

            if (GetProcessId(process_handle_) == NULL)
            {
                return 0.0;
            }

        #elif __linux__

            if (std::filesystem::is_directory("/proc/" + std::to_string(pid_)) == false)
            {
                return 0;
            }

        #endif

            unsigned long now_process_cpu_unit = GetClockCycle();
            unsigned long long now_system_cpu_unit = GetSystemClockCycle();

            if (now_system_cpu_unit - last_system_cpu_unit_ != 0)
            {
                #ifdef _WIN32
                    percent = static_cast<double>((now_process_cpu_unit - last_process_cpu_unit_)/(now_system_cpu_unit - last_system_cpu_unit_));
                    percent /= num_processors_; // ?
                    percent *= 100;
                #elif __linux__

                    percent = static_cast<double>(now_process_cpu_unit - last_process_cpu_unit_) * 100 /(now_system_cpu_unit - last_system_cpu_unit_);

                #endif
            }
            else
            {
                percent = 0;
            }

            last_process_cpu_unit_ = now_process_cpu_unit;

            last_system_cpu_unit_ = now_system_cpu_unit;


        last_usage_percent_ = percent;
        return last_usage_percent_;
    };

    double ProcessCpuStats::GetLastUsage()
    {
        #ifdef _WIN32
            if (GetProcessId(process_handle_) == NULL)
            {
                return 0.0;
            }
        #elif __linux__
            if (std::filesystem::is_directory("/proc/" + std::to_string(pid_)) == false)
            {
                return 0.0;
            }
        #endif

        return last_usage_percent_;
    }

}