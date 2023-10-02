
#ifndef PROCESSMONITORING_PROCESS_PROCESSNETWORK_H_
#define PROCESSMONITORING_PROCESS_PROCESSNETWORK_H_

#include "ulti/everything.h"


namespace pm
{
    class ProcessNetworkStats
    {
    private:
        int pid_ = 0;

        #ifdef _WIN32
            FILETIME last_retrieve_time_;
            unsigned long long io_size_in_byte_ = 0;
        #elif __linux__
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

            void SetIoSizeInByte(unsigned long long io_size_in_byte_);
            unsigned long long GetIoSizeInByte();
            double GetIoSizeInMb();

            void SetLastRetrieveTime(FILETIME time);
            FILETIME GetLastRetrieveTime() const;

        #elif __linux__

            double GetCurrentSpeed();
            double GetLastSpeed();

        #endif
    };

}

#endif