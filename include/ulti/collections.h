#ifdef _VISUAL_STUDIO_WORKSPACE
#pragma once
#endif

#ifndef PROCESSMONITORING_ULTI_COLLECTIONS_H_
#define PROCESSMONITORING_ULTI_COLLECTIONS_H_

#ifdef _VISUAL_STUDIO_WORKSPACE
#pragma once
#endif

namespace pm
{
    struct MonitoringComponent
    {
        double cpu_usage = 0;
        double mem_usage = 0;
        double disk_usage = 0;
        double network_usage = 0;
    };

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

}

#endif