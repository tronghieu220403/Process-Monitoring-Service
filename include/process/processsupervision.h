#ifndef PROCESSMONITORING_PROCESS_PROCESSSUPERVISION_H_
#define PROCESSMONITORING_PROCESS_PROCESSSUPERVISION_H_

#include "ulti/everything.h"
#include "process/processinfo.h"
#include "process/processcontroller.h"
#include "logs/processlogger.h"
#include "ulti/collections.h"

namespace pm
{
    class ProcessSupervision
    {
    private:
        MonitoringComponent max_usage_;
        std::shared_ptr<ProcessController> process_controller_;
        std::shared_ptr<ProcessLogger> process_logger_;

    public:
        ProcessSupervision();
        
        explicit ProcessSupervision(const std::string& name);
        explicit ProcessSupervision(const std::string& name, const MonitoringComponent& max_usage);

        explicit ProcessSupervision(const ProcessController& pc);

        void SetProcessController(const ProcessController& process_controller);

        void SetMaxUsage(const MonitoringComponent& max_usage);
        void SetMaxCpuUsage(float max_cpu_usage);
        void SetMaxMemUsage(double max_mem_usage);
        void SetMaxDiskUsage(float max_disk_usage);
        void SetMaxNetworkUsage(float max_network_usage);

        std::shared_ptr<ProcessController>& GetProcessController();
        std::shared_ptr<ProcessLogger>& GetProcessLogger();
        MonitoringComponent GetMonitoringComponent() const;

        void UpdateProcessStats();
        void CheckProcessStats();
        #ifdef _WIN32
            void Alert(ProcessLoggerType type, UsageData usage_data);
        #elif __linux__
            void Alert(ProcessLoggerType type);
        #endif
    };
}

#endif