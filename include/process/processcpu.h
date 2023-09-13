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
#include <filesystem>

namespace pm
{
    class ProcessCpuStats
    {
    
    private:

        double last_usage_percent_ = 0;
        int num_processors_ = 0;

        #ifdef _WIN32
            HANDLE process_handle_ = nullptr;

        #elif __linux__
            int pid_;
            
        #endif

        unsigned long long last_process_cpu_unit_;
        
        unsigned long long last_system_cpu_unit_;

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