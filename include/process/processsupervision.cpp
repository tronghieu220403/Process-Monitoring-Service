    
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
        if (process_controller_->IsExists() == true && process_controller_->GetProcessInfo() != nullptr)
        {
            process_controller_->GetProcessInfo()->UpdateAttributes();
        }
    }

    void ProcessSupervision::CheckProcessStats()
    {
        #ifdef __linux__
        std::shared_ptr<ProcessInfo> p_info = process_controller_->GetProcessInfo();
        if (p_info->GetCpuUsage() > max_usage_.cpu_usage)
        {
            Alert(ProcessLoggerType::kProcessLoggerCpu);
        }
        if (p_info->GetMemoryUsage() > max_usage_.mem_usage)
        {
            Alert(ProcessLoggerType::kProcessLoggerMem);
        }
        if (p_info->GetDiskUsage() > max_usage_.disk_usage)
        {
            Alert(ProcessLoggerType::kProcessLoggerDisk);
        }
        if (p_info->GetNetworkUsage() > max_usage_.network_usage)
        {
            Alert(ProcessLoggerType::kProcessLoggerNet);
        }
        #elif _WIN32
            
            std::shared_ptr<ProcessInfo> p_info = process_controller_->GetProcessInfo();
            if (p_info == nullptr)
            {
                return;
            }
            UsageData mem_usage = p_info->GetMemoryUsageStats()->GetMemoryUsageData();
            if (mem_usage.data > max_usage_.mem_usage)
            {
                Alert(ProcessLoggerType::kProcessLoggerMem, mem_usage);
            }

            UsageData cpu_usage = p_info->GetMemoryUsageStats()->GetMemoryUsageData();
            if (cpu_usage.data > max_usage_.cpu_usage)
            {
                Alert(ProcessLoggerType::kProcessLoggerCpu, cpu_usage);
            }

            while(p_info->GetDiskUsageStats()->HasData())
            {
                UsageData disk_usage = p_info->GetDiskUsageStats()->GetFrontIoDataInMb();
                p_info->GetDiskUsageStats()->DeleteFrontIodata();
                if (disk_usage.data > max_usage_.disk_usage)
                {
                    Alert(ProcessLoggerType::kProcessLoggerDisk, disk_usage);
                }
            }

            while(p_info->GetNetworkUsageStats()->HasData())
            {
                UsageData network_usage = p_info->GetNetworkUsageStats()->GetFrontIoDataInKb();
                p_info->GetNetworkUsageStats()->DeleteFrontIodata();
                if (network_usage.data > max_usage_.network_usage)
                {
                    Alert(ProcessLoggerType::kProcessLoggerNet, network_usage);
                }
            }

        #endif
    }

#ifdef __linux__
    void ProcessSupervision::Alert(ProcessLoggerType type)
    {
        process_logger_->AddMessage(process_logger_->GetAlert(type));
    }
#elif _WIN32
    void ProcessSupervision::Alert(ProcessLoggerType type, UsageData usage_data)
    {
        process_logger_->AddMessage(process_logger_->GetAlert(type, usage_data));
    }
#endif
}
