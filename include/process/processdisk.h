#ifndef PROCESSMONITORING_PROCESS_PROCESSDISK_H_
#define PROCESSMONITORING_PROCESS_PROCESSDISK_H_

#ifdef _WIN32
#include "windows.h"
#elif __linux__

#endif

namespace pm
{
    class ProcessDiskStats
    {
    
    private:
        IO_COUNTERS last_io_counter_;
        FILETIME last_time_;
        HANDLE process_handle_;
        double last_speed_;
    public:
        static int num_processors_;

        ProcessDiskStats();
        ProcessDiskStats(HANDLE p_handle);

        double GetCurrentSpeed();
        double GetLastSpeed();

    };
}

#endif