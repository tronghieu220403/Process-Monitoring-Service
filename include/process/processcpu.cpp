
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

            ZeroMemory(&last_cpu_time_, sizeof(ULARGE_INTEGER));
            ZeroMemory(&last_sys_cpu_, sizeof(ULARGE_INTEGER));
            ZeroMemory(&last_user_cpu_, sizeof(ULARGE_INTEGER));
            if (num_processors_ == 0)
            {
                num_processors_ = GetNumberOfProcessors();
            }

        #elif __linux__

        #endif
    };

    ProcessCpuStats::ProcessCpuStats(const ProcessCpuStats& pcs) = default;

    ProcessCpuStats& ProcessCpuStats::operator=(const ProcessCpuStats& pcs)
    {
        this->last_usage_percent_ = pcs.last_usage_percent_;
        #ifdef _WIN32

            this->last_cpu_time_ = pcs.last_cpu_time_;
            this->last_sys_cpu_ = pcs.last_sys_cpu_;
            this->last_user_cpu_ = pcs.last_user_cpu_;
            this->process_handle_ = pcs.process_handle_;
        
        #elif __linux__

        #endif

        return *this;
    }


#ifdef _WIN32
    ProcessCpuStats::ProcessCpuStats(HANDLE p_handle)
    {
        if (GetProcessId(p_handle) == NULL)
        {
            return;
        }
        process_handle_ = p_handle;

        FILETIME ftime;
        FILETIME fsys;
        FILETIME fuser;

        num_processors_ = num_processors_ = GetNumberOfProcessors();

        GetSystemTimeAsFileTime(&ftime);
        memcpy(&last_cpu_time_, &ftime, sizeof(FILETIME));

        GetProcessTimes(process_handle_, &ftime, &ftime, &fsys, &fuser);
        memcpy(&last_sys_cpu_, &fsys, sizeof(FILETIME));
        memcpy(&last_user_cpu_, &fuser, sizeof(FILETIME));

    };
#endif

#ifdef __linux__
    ProcessCpuStats::ProcessCpuStats(int pid): pid_(pid)
    {

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
        #endif
    }

    long long ProcessCpuStats::GetSystemClockCycle()
    {
        #ifdef _WIN32
            FILETIME ftime;
            GetSystemTimeAsFileTime(&ftime);
            return ftime.QuadPart;
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

    long long ProcessCpuStats::GetProcessClockCycle()
    {
        #ifdef _WIN32

        #elif __linux__
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
            long long user_process_clock_cycle;
            long long sys_process_clock_cycle;

            long long total_clock_cycle;

            s >> user_process_clock_cycle;
            s >> sys_process_clock_cycle;
            return user_process_clock_cycle + sys_process_clock_cycle;
        #endif
    }

    double ProcessCpuStats::GetCurrentUsage()
    {
        double percent;

        #ifdef _WIN32

            if (GetProcessId(process_handle_) == NULL)
            {
                return 0.0;
            }
            FILETIME ftime;
            FILETIME fsys;
            FILETIME fuser;
            ULARGE_INTEGER now_time;
            ULARGE_INTEGER sys;
            ULARGE_INTEGER user;



            GetSystemTimeAsFileTime(&ftime);
            memcpy(&now_time, &ftime, sizeof(FILETIME));

            GetProcessTimes(process_handle_, &ftime, &ftime, &fsys, &fuser);
            memcpy(&sys, &fsys, sizeof(FILETIME));
            memcpy(&user, &fuser, sizeof(FILETIME));
            percent = static_cast<double>((sys.QuadPart - last_sys_cpu_.QuadPart) + (user.QuadPart - last_user_cpu_.QuadPart));
            if (now_time.QuadPart - last_cpu_time_.QuadPart != 0)
            {
                percent /= static_cast<double>(now_time.QuadPart - last_cpu_time_.QuadPart);
                percent /= num_processors_;
                percent *= 100;
            }
            else
            {
                percent = 0;
            }
            last_cpu_time_ = now_time;
            last_user_cpu_ = user;
            last_sys_cpu_ = sys;
            
        #elif __linux__
            long long now_process_clock_cycle = GetProcessClockCycle();
            long long now_system_clock_cycle = GetSystemClockCycle();
            if (now_system_clock_cycle - last_system_clock_cycle_ != 0)
            {
                percent = static_cast<double>(now_process_clock_cycle - last_process_clock_cycle_) * 100 /(now_system_clock_cycle - last_system_clock_cycle_);
            }
            else
            {
                percent = 0;
            }
            last_system_clock_cycle_ = now_system_clock_cycle;
            last_process_clock_cycle_ = now_process_clock_cycle;
        #endif

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

        #endif

        return last_usage_percent_;
    }

}