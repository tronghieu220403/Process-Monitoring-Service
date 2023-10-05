    
#include "processsupervision.h"

namespace pm
{

    ProcessSupervision::ProcessSupervision() = default;

    ProcessSupervision::ProcessSupervision(std::string& name)
    {
        this->ProcessController::ProcessController(name);
        this->ProcessLogger::ProcessLogger(name);
    }

    ProcessSupervision::ProcessSupervision(std::string& name, const MonitoringComponent& max_usage):
        max_usage_(max_usage)
    {
        this->ProcessController::ProcessController(name);
        this->ProcessLogger::ProcessLogger(name);
        max_usage_ = max_usage;
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
    
    void ProcessSupervision::UpdateProcessStats()
    {
        if (IsExists() == false)
        {
            TryFindHandle();
        }
        if (IsExists() == true && GetProcessInfo() != nullptr)
        {
            GetProcessInfo()->UpdateAttributes();
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
        
            UsageData usage = { 0 };

            std::shared_ptr<ProcessInfo> p_info = GetProcessInfo();
            if (p_info == nullptr)
            {
                return;
            }
            usage = p_info->GetMemoryUsageStats()->GetMemoryUsageData();
            if (usage.data > max_usage_.mem_usage)
            {
                Alert(ProcessLoggerType::kProcessLoggerMem, usage);
            }

            usage = p_info->GetCpuUsageStats()->GeCpuUsageData();
            if (static_cast<long long>(usage.data * 10000) > static_cast<long long>(max_usage_.cpu_usage * 10000))
            {
                Alert(ProcessLoggerType::kProcessLoggerCpu, usage);
            }

            while(p_info->GetDiskUsageStats()->HasData())
            {
                usage = p_info->GetDiskUsageStats()->GetFrontIoDataInMb();
                p_info->GetDiskUsageStats()->DeleteFrontIodata();
                if (usage.data > max_usage_.disk_usage)
                {
                    Alert(ProcessLoggerType::kProcessLoggerDisk, usage);
                }
            }

            while(p_info->GetNetworkUsageStats()->HasData())
            {
                usage = p_info->GetNetworkUsageStats()->GetFrontIoDataInKb();
                p_info->GetNetworkUsageStats()->DeleteFrontIodata();
                if (usage.data > max_usage_.network_usage)
                {
                    Alert(ProcessLoggerType::kProcessLoggerNet, usage);
                }
            }

        #endif
    }

#ifdef __linux__
    void ProcessSupervision::Alert(ProcessLoggerType type)
    {
        AddMessage(GetAlert(type));
    }
#elif _WIN32
    void ProcessSupervision::Alert(ProcessLoggerType type, UsageData usage_data)
    {
        AddMessage(GetAlert(type, usage_data));
    }
#endif
}
