#ifndef PROCESSMONITORING_PROCESS_PROCESSDISK_H_
#define PROCESSMONITORING_PROCESS_PROCESSDISK_H_

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
        
        double last_speed_;
    public:
        static int num_processors_;

        ProcessDiskStats();

        ProcessDiskStats(HANDLE p_handle);

        ProcessDiskStats(ProcessDiskStats& pds);
        ProcessDiskStats(ProcessDiskStats&& pds) = delete;

        ProcessDiskStats& operator=(const ProcessDiskStats& pds);


        double GetCurrentSpeed();
        double GetLastSpeed();

    };
}

#endif