#ifdef _VISUAL_STUDIO_WORKSPACE
#pragma once
#endif

#ifndef PROCESSMONITORING_ULTI_COLLECTIONS_H_
#define PROCESSMONITORING_ULTI_COLLECTIONS_H_

namespace pm
{

    enum class ProcessLoggerType
    {
        kProcessLoggerCpu,
        kProcessLoggerMem,
        kProcessLoggerDisk,
        kProcessLoggerNet
    };

    enum Command
    {
        CTB_NOTI_CONFIG = 1 ,
        CTA_SEND_LOGS = 2
    };

#ifdef _WIN32

    struct UsageData
    {
        FILETIME time;
        double data;
    };


    struct UsageIoData
    {
        FILETIME time;
        unsigned long long data;
    };

    struct MonitoringComponent
    {
        UsageData cpu_usage = {0};
        UsageData mem_usage = {0};
        UsageData disk_usage = {0};
        UsageData network_usage = {0};
    };

	struct IoInfo
	{
		FILETIME filetime = {0};
		unsigned long long size = 0;
		long long pid = 0;
	};

    struct ServiceEvent
    {
        public: 
            HANDLE stop_event_ = nullptr;
            HANDLE pause_event_ = nullptr;
            HANDLE continue_event_ = nullptr;

            void Close();
    };

    enum IoType
    {
        DISK_IO = 1 ,
        NETWORK_IO = 2
    };

#elif __linux__

struct MonitoringComponent
{
    double cpu_usage = 0;
    double mem_usage = 0;
    double disk_usage = 0;
    double network_usage = 0;
};

#endif

}

#endif