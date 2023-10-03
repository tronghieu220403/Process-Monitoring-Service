#ifndef PROCESSMONITORING_PROCESS_PROCESSDISK_H_
#define PROCESSMONITORING_PROCESS_PROCESSDISK_H_

#include "ulti/everything.h"
#include "ulti/collections.h"

namespace pm
{
    class ProcessDiskStats
    {
    
    private:

        int pid_ = 0;

        #ifdef _WIN32
            std::deque<UsageIoData> io_deque_;
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

            void AddData(FILETIME time, unsigned long long data);
            bool HasData() const;
            UsageIoData GetFrontIoData();
            void DeleteFrontIodata();

        #elif __linux__

            double GetCurrentCounter();
            double GetCurrentSpeed();
            double GetSpeed();

        #endif


    };
}

#endif