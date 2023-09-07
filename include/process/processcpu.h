#ifdef _VISUAL_STUDIO_WORKSPACE
#pragma once
#endif

#ifndef PROCESSMONITORING_PROCESS_PROCESSCPU_H_
#define PROCESSMONITORING_PROCESS_PROCESSCPU_H_

#ifndef _VISUAL_STUDIO_WORKSPACE
#define UNICODE
#define _UNICODE
#endif

#ifdef _WIN32
#include <Windows.h>
#elif __linux__

#endif

namespace pm
{
    class ProcessCpuStats
    {
    
    private:

        double last_usage_percent_ = 0;
        int num_processors_ = 0;

        #ifdef _WIN32
            ULARGE_INTEGER last_cpu_;
            ULARGE_INTEGER last_sys_cpu_; 
            ULARGE_INTEGER last_user_cpu_;

            HANDLE process_handle_ = nullptr;
        #elif __linux__

        #endif
    public:

        ProcessCpuStats();
        ProcessCpuStats(const ProcessCpuStats& pcs);
        ProcessCpuStats(const ProcessCpuStats&& pcs) = delete;

        ProcessCpuStats& operator=(const ProcessCpuStats& pcs);


        #ifdef _WIN32
            explicit ProcessCpuStats(HANDLE p_handle);
        #elif __linux__

        #endif

        double GetCurrentUsage();
        double GetLastUsage();

    };
}

#endif