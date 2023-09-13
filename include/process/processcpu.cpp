
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
                SYSTEM_INFO sysInfo;

                GetSystemInfo(&sysInfo);
                num_processors_ = sysInfo.dwNumberOfProcessors;
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
        #ifdef _WIN32

            if (GetProcessId(p_handle) == NULL)
            {
                return;
            }
            process_handle_ = p_handle;

            FILETIME ftime;
            FILETIME fsys;
            FILETIME fuser;

            SYSTEM_INFO sysInfo;

            GetSystemInfo(&sysInfo);
            num_processors_ = sysInfo.dwNumberOfProcessors;

            GetSystemTimeAsFileTime(&ftime);
            memcpy(&last_cpu_time_, &ftime, sizeof(FILETIME));

            GetProcessTimes(process_handle_, &ftime, &ftime, &fsys, &fuser);
            memcpy(&last_sys_cpu_, &fsys, sizeof(FILETIME));
            memcpy(&last_user_cpu_, &fuser, sizeof(FILETIME));

        #elif __linux__

        #endif

    };
#endif

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
            }
            else
            {
                percent = 0;
            }
            last_cpu_time_ = now_time;
            last_user_cpu_ = user;
            last_sys_cpu_ = sys;
            
        #elif __linux__

        #endif

        last_usage_percent_ = percent * 100;
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