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

    struct MonitoringComponent
    {
        double cpu_usage = 0;
        double mem_usage = 0;
        double disk_usage = 0;
        double network_usage = 0;
    };

	struct IoInfo
	{
		FILETIME filetime = {0};
		unsigned long long size = 0;
		long long pid = 0;
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

struct UsageData
{
    time_t time;
    double data;
};

#endif

}

#endif