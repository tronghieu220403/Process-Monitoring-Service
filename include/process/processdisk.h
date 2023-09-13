#ifdef _VISUAL_STUDIO_WORKSPACE
#pragma once
#endif

#ifndef PROCESSMONITORING_PROCESS_PROCESSDISK_H_
#define PROCESSMONITORING_PROCESS_PROCESSDISK_H_

#ifndef _VISUAL_STUDIO_WORKSPACE
#define UNICODE
#define _UNICODE
#endif

#ifdef __linux__
#include<time.h>
#include<unistd.h>
#endif

#ifdef _WIN32
#include <Windows.h>
#elif __linux__

#endif

namespace pm
{
    class ProcessDiskStats
    {
    
    private:
        #ifdef _WIN32
            FILETIME last_time_;
            HANDLE process_handle_;
        #elif __linux__
            int pid_;
            clock_t last_time_;
        #endif
        
        unsigned long long last_io_;

        double last_speed_ = 0;

    public:
        static int num_processors_;

        ProcessDiskStats();
        #ifdef _WIN32
            explicit ProcessDiskStats(HANDLE p_handle);
        #endif

        unsigned long long GetCurrentCounter();
        double GetCurrentSpeed();
        double GetLastSpeed();

    };
}

#endif