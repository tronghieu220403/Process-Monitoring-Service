#pragma once

#ifndef PROCESSMONITORING_PROCESS_PROCESSDISK_H_
#define PROCESSMONITORING_PROCESS_PROCESSDISK_H_

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
    class ProcessDiskStats
    {
    
    private:
        #ifdef _WIN32
            IO_COUNTERS last_io_counter_;
            FILETIME last_time_;
            HANDLE process_handle_;
        #elif __linux__

        #endif
        
        double last_speed_ = 0;
    public:
        static int num_processors_;

        ProcessDiskStats();
        #ifdef _WIN32
            explicit ProcessDiskStats(HANDLE p_handle);
        #endif
        ProcessDiskStats(const ProcessDiskStats& pds);
        ProcessDiskStats(const ProcessDiskStats&& pds) = delete;

        ProcessDiskStats& operator=(const ProcessDiskStats& pds);


        double GetCurrentSpeed();
        double GetLastSpeed();

    };
}

#endif