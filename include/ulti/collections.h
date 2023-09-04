#ifndef PROCESSMONITORING_ULTI_COLLECTIONS_H_
#define PROCESSMONITORING_ULTI_COLLECTIONS_H_

#define UNICODE
#define _UNICODE
 
namespace pm
{
    struct MonitoringComponent
    {
        float cpu_usage;
        double mem_usage;
        float disk_usage;
        float network_usage;
    };

    enum ProcessLoggerType
    {
        kProcessLoggerCpu,
        kProcessLoggerMem,
        kProcessLoggerDisk,
        kProcessLoggerNet
    };

}

#endif