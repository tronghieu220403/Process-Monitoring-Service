#ifdef _VISUAL_STUDIO_WORKSPACE
#pragma once
#endif

#ifndef PROCESSMONITORING_PROCESS_PROCESSSUPERVISION_H_
#define PROCESSMONITORING_PROCESS_PROCESSSUPERVISION_H_

#ifndef _VISUAL_STUDIO_WORKSPACE
#define UNICODE
#define _UNICODE
#endif


#ifdef _VISUAL_STUDIO_WORKSPACE
#include "E:/Code/Github/Process-Monitoring/include/process/processinfo.h"
#include "E:/Code/Github/Process-Monitoring/include/process/processcontroller.h"
#include "E:/Code/Github/Process-Monitoring/include/logs/processlogger.h"
#include "E:/Code/Github/Process-Monitoring/include/ulti/collections.h"
#else
#include "include/process/processinfo.h"
#include "include/process/processcontroller.h"
#include "include/logs/processlogger.h"
#include "include/ulti/collections.h"

#endif


namespace pm
{
    class ProcessSupervision
    {
    private:
        MonitoringComponent max_usage_;
        ProcessController process_controller_;
        ProcessLogger process_logger_;

    public:
        ProcessSupervision();
        
        explicit ProcessSupervision(const std::string& name);

        explicit ProcessSupervision(const ProcessController& pc);

        void SetProcessController(const ProcessController& process_controller);

        void SetMaxUsage(const MonitoringComponent& max_usage);
        void SetMaxCpuUsage(float max_cpu_usage);
        void SetMaxMemUsage(double max_mem_usage);
        void SetMaxDiskUsage(float max_disk_usage);
        void SetMaxNetworkUsage(float max_network_usage);

        ProcessController& GetProcessController();
        ProcessLogger& GetProcessLogger();
        MonitoringComponent GetMonitoringComponent() const;

        void UpdateProcessStats();
        void CheckProcessStats();
        void Alert(ProcessLoggerType type);
    };
}

#endif