#ifndef PROCESSMONITORING_PROCESS_PROCESSDISK_H_
#define PROCESSMONITORING_PROCESS_PROCESSDISK_H_

#include "ulti/everything.h"

namespace pm
{
    class ProcessDiskStats
    {
    
    private:

        int pid_ = 0;

        #ifdef _WIN32
            FILETIME last_retrieve_time_ = { 0 };
            unsigned long long io_size_in_byte_ = 0;

        #elif __linux__
            clock_t last_time_ = 0;
            double last_io_ = 0;
            double last_speed_ = 0;
        #endif

        static int num_processors_;


    public:

        ProcessDiskStats();

        explicit ProcessDiskStats(int pid);

        void SetPid(int pid);
        int GetPid() const;

        #ifdef _WIN32

            void SetIoSizeInByte(unsigned long long io_size_in_byte_);
            unsigned long long GetIoSizeInByte();
            double GetIoSizeInMb();

            void SetLastRetrieveTime(FILETIME time);
            FILETIME GetLastRetrieveTime() const;
        #elif __linux__

            double GetCurrentCounter();
            double GetCurrentSpeed();
            double GetSpeed();

        #endif


    };
}

#endif