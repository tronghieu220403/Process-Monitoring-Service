#pragma once

#ifndef PROCESSMONITORING_ULTI_COLLECTIONS_H_
#define PROCESSMONITORING_ULTI_COLLECTIONS_H_

#ifndef _VISUAL_STUDIO_WORKSPACE
#define UNICODE
#define _UNICODE
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

    enum class Command
    {
        CTB_NOTI_CONFIG,
        CTA_SEND_LOGS
    };

}

#endif