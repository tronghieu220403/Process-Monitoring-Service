#ifndef PROCESSMONITORING_PROCESS_PROCESSSUPERVISION_H_
#define PROCESSMONITORING_PROCESS_PROCESSSUPERVISION_H_

#include "include/process/processcontroller.h"
#include "include/logs/processlogger.h"
#include "include/ulti/collections.h"

#ifdef _VISUAL_STUDIO_WORKSPACE
#include "E:/Code/Github/Process-Monitoring/include/process/processinfo.h"
#else
#include "include/process/processinfo.h"
#endif


namespace pm
{
    class ProcessSupervision
    {
    protected:
        MonitoringComponent max_usage_;
        ProcessController process_controller_;
        ProcessLogger process_logger_;

    public:
        ProcessSupervision();
        
        ProcessSupervision(std::string name);

        explicit ProcessSupervision(ProcessSupervision& ps);

        explicit ProcessSupervision(ProcessController& pc);

        ProcessSupervision& operator=(const ProcessSupervision& ps);

        void SetProcessController(ProcessController process_control);

        void SetMaxUsage(MonitoringComponent max_usage);
        void SetMaxCpuUsage(float max_cpu_usage);
        void SetMaxMemUsage(double max_mem_usage);
        void SetMaxDiskUsage(float max_disk_usage);
        void SetMaxNetworkUsage(float max_network_usage);

        ProcessController GetProcessController();
        ProcessLogger GetProcessLogger();

        void UpdateProcessStats();
        void CheckProcessStats();
        bool Alert(ProcessLoggerType type);
    };
}

#endif