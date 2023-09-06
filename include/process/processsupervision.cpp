#ifdef _VISUAL_STUDIO_WORKSPACE

#include "processsupervision.h"

#else

#include "include/process/processsupervision.h"

#endif //  

namespace pm
{

    ProcessSupervision::ProcessSupervision()
    {
        this.max_usage_ = MonitoringComponent();
        this.process_controller_ = ProcessController();
        this.process_logger_ = ProcessLogger();
    }

    ProcessSupervision::ProcessSupervision(ProcessSupervision& ps)
    {
        this.max_usage_ = (pc.max_usage);
        this.process_controller_ = (pc.process_controller_);
        this.process_logger_ = (pc.process_logger_);
    }

    ProcessSupervision::ProcessSupervision(ProcessSupervision&& ps)
    {
        this.max_usage_ = std::move(pc.max_usage);
        this.process_controller_ = std::move(pc.process_controller_);
        this.process_logger_ = std::move(pc.process_logger_);
    }

    ProcessSupervision::ProcessSupervision(ProcessController p_control)
    {
        process_controller_(p_control);
    }

    ProcessSupervision::ProcessSupervision(ProcessController& p_control)
    {
        process_controller = std::move(p_control);
    }

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


    void ProcessSupervision::SetMaxUsage(MonitoringComponent max_usage)
    {
        max_usage_ = max_usage;
    }

    void ProcessSupervision::SetMaxCpuUsage(float max_cpu_usage)
    {
        max_usage_.cpu_usage = max_cpu_usage;
    }

    void ProcessSupervision::SetMaxMemUsage(double max_mem_usage)
    {
        max_usage_.mem_usage = max_mem_usage;
    }

    void ProcessSupervision::SetMaxDiskUsage(float max_disk_usage)
    {
        max_usage_.disk_usage = max_disk_usage;
    }

    void ProcessSupervision::SetMaxNetworkUsage(float max_network_usage)
    {
        max_usage_.network_usage = max_network_usage;
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
        ProcessInfo p_info = process_controller_.GetProcessInfo();
        p_info.UpdateAttributes();
    }

    void ProcessSupervision::CheckProcessStats()
    {
        ProcessInfo p_info = process_controller_.GetProcessInfo();
        if (p_info.GetCpuUsage() > max_usage_.cpu_usage)
        {
            Alert(ProcessLoggerType::kProcessLoggerCpu);
        }
        if (p_info.GetMemoryUsage() > max_usage_.mem_usage)
        {
            Alert(ProcessLoggerType::kProcessLoggerMem);
        }
        if (p_info.GetDiskUsage() > max_usage_.disk_usage)
        {
            Alert(ProcessLoggerType::kProcessLoggerDisk);
        }
        if (p_info.GetNetworkUsage() > max_usage_.network_usage)
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
