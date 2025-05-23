
#ifndef PROCESSMONITORING_PROCESS_PROCESSNETWORK_H_
#define PROCESSMONITORING_PROCESS_PROCESSNETWORK_H_

#include "ulti/everything.h"
#include "ulti/collections.h"

namespace pm
{
    class ProcessNetworkStats
    {
    private:
        int pid_ = 0;

        #ifdef _WIN32
        std::deque<UsageData> io_deque_;
        #elif __linux__
        time_t last_retrieved_time_ = 0;
        long long last_data_recv_ = 0;
        long long last_data_sent_ = 0;
        double last_speed_ = 0;
        #endif
    public:
        ProcessNetworkStats();
        
        explicit ProcessNetworkStats(int pid);

        void SetPid(int pid);
        int GetPid() const;

        #ifdef _WIN32

            void AddData(FILETIME time, unsigned long long data);
            bool HasData() const;
            UsageData GetFrontIoDataInKb();
            void DeleteFrontIodata();

        #elif __linux__

            double GetLastSpeed();
            void UpdateAttributes();
            UsageData GetLastIoSpeedInKb();
            
        #endif
    };

}

#endif