#include "process/processsupervision.h"

namespace pm
{

    ProcessSupervision::ProcessSupervision(std::wstring name)
    {
        process_controller_ = ProcessController(name); 
        process_logger_ = ProcessLogger(process_controller_);
    }

    ProcessSupervision::ProcessSupervision(std::string name)
    {
        process_controller_ = ProcessController(name);
        process_logger_ = ProcessLogger(process_controller_);
    }

    ProcessSupervision::ProcessSupervision(ProcessController process_controller)
    {
        process_controller_ = process_controller;
        process_logger_ = ProcessLogger(process_controller_);
    }

    void ProcessSupervision::SetProcessController(ProcessController process_controller)
    {
        process_controller_ = process_controller;
        process_logger_ = ProcessLogger(process_controller_);
    }

    void ProcessSupervision::SetMaxUsage(float max_cpu_usage)
    {
        max_cpu_usage_ = max_cpu_usage;
    }

    void ProcessSupervision::SetMaxMemsage(double max_mem_usage)
    {
        max_mem_usage_ = max_mem_usage;
    }

    void ProcessSupervision::SetMaxDiskUsage(float max_disk_usage)
    {
        max_disk_usage_ = max_disk_usage;
    }

    void ProcessSupervision::SetMaxNetworkUsage(float max_network_usage)
    {
        max_network_usage_ = max_network_usage;
    }

    ProcessController ProcessSupervision::GetProcessController()
    {
        return process_controller_;
    }

    ProcessLogger ProcessSupervision::GetProcessLogger()
    {
        return process_logger_;
    }


    void ProcessSupervision::UpdateProcessStats()
    {
        if (process_controller_.IsExists() == false)
        {
            process_controller_.TryFindHandle();
        }
        ProcessInfo& p_info = process_controller_.GetProcessInfo();
        p_info.UpdateAttributes();
    }

    void ProcessSupervision::CheckProcessStats()
    {
        ProcessInfo& p_info = process_controller_.GetProcessInfo();
        if (p_info.GetCpuUsage() > max_cpu_usage_)
        {
            Alert(ProcessLoggerType::kProcessLoggerCpu);
        }
        if (p_info.GetDiskUsage() > max_disk_usage_)
        {
            Alert(ProcessLoggerType::kProcessLoggerDisk);
        }
        if (p_info.GetMemoryUsage() > max_mem_usage_)
        {
            Alert(ProcessLoggerType::kProcessLoggerMem);
        }
        if (p_info.GetNetworkUsage() > max_network_usage_)
        {
            Alert(ProcessLoggerType::kProcessLoggerNet);
        }
    }

    bool ProcessSupervision::Alert(ProcessLoggerType type)
    {
        process_logger_.SetMessage(type);
        process_logger_.WriteLog();        
    }

}
