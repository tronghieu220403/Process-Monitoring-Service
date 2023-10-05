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
            std::deque<UsageData> io_deque_;
        #elif __linux__
            time_t last_retrieved_time_ = 0;
            clock_t last_time_ = 0;
            unsigned long long last_io_ = 0;
            unsigned long long last_speed_ = 0;
        #endif

    public:

        ProcessDiskStats();

        explicit ProcessDiskStats(int pid);

        void SetPid(int pid);
        int GetPid() const;

        #ifdef _WIN32

            void AddData(FILETIME time, unsigned long long data);
            bool HasData() const;
            UsageData GetFrontIoDataInMb();
            void DeleteFrontIodata();

        #elif __linux__

            unsigned long long GetCurrentCounter();
            void UpdateAttributes();
            UsageData GetLastIoSpeedInMb();

        #endif

    };
}

#endif