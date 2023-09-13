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

#include <sstream>
#include <string>
#include <fstream>

namespace pm
{
    class ProcessCpuStats
    {
    
    private:

        double last_usage_percent_ = 0;
        int num_processors_ = 0;

        #ifdef _WIN32
            HANDLE process_handle_ = nullptr;

            unsigned long long last_system_cpu_;
            unsigned long long last_process_cpu_; 

        #elif __linux__
            int pid_;

            unsigned long long last_process_clock_cycle_;
            unsigned long long last_system_clock_cycle_;
            
        #endif
    public:

        ProcessCpuStats();
        ProcessCpuStats(const ProcessCpuStats& pcs);
        ProcessCpuStats(const ProcessCpuStats&& pcs) = delete;

        ProcessCpuStats& operator=(const ProcessCpuStats& pcs);


        #ifdef _WIN32
            explicit ProcessCpuStats(HANDLE p_handle);
        #elif __linux__
            explicit ProcessCpuStats(int pid);
        #endif

        unsigned long long GetClockCycle();

        double GetCurrentUsage();
        double GetLastUsage();

        static int GetNumberOfProcessors();
        unsigned long long GetSystemClockCycle();

    };
}

#endif