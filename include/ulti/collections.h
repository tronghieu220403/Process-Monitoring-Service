#ifndef PROCESSMONITORING_ULTI_COLLECTIONS_H_
#define PROCESSMONITORING_ULTI_COLLECTIONS_H_
 
namespace pm
{
    struct MonitoringComponent
    {
        double cpu_usage;
        double mem_usage;
        double disk_usage;
        double network_usage;
    };

    enum class ProcessLoggerType
    {
        kProcessLoggerCpu,
        kProcessLoggerMem,
        kProcessLoggerDisk,
        kProcessLoggerNet
    };

}

#endif