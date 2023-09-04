#ifndef PROCESSMONITORING_ULTI_COLLECTIONS_H_
#define PROCESSMONITORING_ULTI_COLLECTIONS_H_

#define UNICODE
#define _UNICODE
 
namespace pm
{
    struct MonitoringComponent
    {
        double cpu_usage;
        double mem_usage;
        double disk_usage;
        double network_usage;
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