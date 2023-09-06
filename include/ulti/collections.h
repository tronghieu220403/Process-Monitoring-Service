#ifndef PROCESSMONITORING_ULTI_COLLECTIONS_H_
#define PROCESSMONITORING_ULTI_COLLECTIONS_H_
 
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

}

#endif