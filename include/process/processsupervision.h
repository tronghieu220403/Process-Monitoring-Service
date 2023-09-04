#ifndef PROCESSMONITORING_PROCESS_PROCESSSUPERVISION_H_
#define PROCESSMONITORING_PROCESS_PROCESSSUPERVISION_H_

#define UNICODE
#define _UNICODE

#include<process/processcontroller.h>
#include<logs/processlogger.h>

namespace pm
{
    class ProcessSupervision
    {
    protected:
        float max_cpu_usage_;
        double max_mem_usage_;
        float max_disk_usage_;
        float max_network_usage_;

        ProcessController process_controller_;
        ProcessLogger process_logger_;

    public:
        ProcessSupervision() = default;
        
        ProcessSupervision(std::wstring name);
        ProcessSupervision(std::string name);

        ProcessSupervision(ProcessController p_control);

        void SetProcessController(ProcessController process_control);

        void SetMaxUsage(float max_cpu_usage);
        void SetMaxMemsage(double max_mem_usage);
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