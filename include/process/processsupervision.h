#ifndef PROCESSMONITORING_PROCESS_PROCESSSUPERVISION_H_
#define PROCESSMONITORING_PROCESS_PROCESSSUPERVISION_H_

#include "ulti/everything.h"
#include "process/processinfo.h"
#include "process/processcontroller.h"
#include "logs/processlogger.h"
#include "ulti/collections.h"

namespace pm
{
    class ProcessSupervision: 
        public ProcessController,
        public ProcessLogger
    {
    private:
        MonitoringComponent max_usage_;

    public:
        ProcessSupervision();
        
        explicit ProcessSupervision(const std::string& name);
        explicit ProcessSupervision(const std::string& name, const MonitoringComponent& max_usage);

        void SetMaxUsage(const MonitoringComponent& max_usage);
        void SetMaxCpuUsage(float max_cpu_usage);
        void SetMaxMemUsage(double max_mem_usage);
        void SetMaxDiskUsage(float max_disk_usage);
        void SetMaxNetworkUsage(float max_network_usage);

        void UpdateProcessStats();
        void CheckProcessStats();

        void Alert(ProcessLoggerType type, UsageData usage_data);
    };
}

#endif