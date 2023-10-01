    
#include "processsupervision.h"

namespace pm
{

    ProcessSupervision::ProcessSupervision() = default;

    ProcessSupervision::ProcessSupervision(const std::string& name) :
        process_controller_(std::make_shared<ProcessController>(name))
    {
        process_logger_ = std::make_shared<ProcessLogger>(process_controller_);
    }

    ProcessSupervision::ProcessSupervision(const std::string& name, const MonitoringComponent& max_usage):
        process_controller_(std::make_shared<ProcessController>(name))
    {
        process_logger_ = std::make_shared<ProcessLogger>(process_controller_);
        max_usage_ = max_usage;
    }

    ProcessSupervision::ProcessSupervision(const ProcessController& pc):
        process_controller_(std::make_shared<ProcessController>(pc))
    {
        process_logger_ = std::make_shared<ProcessLogger>(process_controller_);
    }

    void ProcessSupervision::SetProcessController(const ProcessController& process_controller)
    {
        process_controller_ = std::make_shared<ProcessController>(process_controller);

        process_logger_ = std::make_shared<ProcessLogger>(process_controller_);
    }

    void ProcessSupervision::SetMaxUsage(const MonitoringComponent& max_usage)
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

    std::shared_ptr<ProcessController>& ProcessSupervision::GetProcessController()
    {
        return process_controller_;
    }

    std::shared_ptr<ProcessLogger>& ProcessSupervision::GetProcessLogger()
    {
        return process_logger_;
    }

    MonitoringComponent ProcessSupervision::GetMonitoringComponent() const
    {
        return max_usage_;
    }
    

    void ProcessSupervision::UpdateProcessStats()
    {
        if (process_controller_->IsExists() == false)
        {
            process_controller_->TryFindHandle();
        }
        process_controller_->GetProcessInfo().UpdateAttributes();
    }

    void ProcessSupervision::CheckProcessStats()
    {
        ProcessInfo const& p_info = process_controller_->GetProcessInfo();
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

    void ProcessSupervision::Alert(ProcessLoggerType type)
    {
        process_logger_->AddMessage(process_logger_->GetAlert(type));
    }

}
