#include "src/CTB.h"

namespace pm
{

    CTB::CTB()
    {
        cta_log_mutex_ = NamedMutex("pm_cta_log");
        config_registry_mutex_ = NamedMutex("config_reg");
        inner_mutex_ = NamedMutex("");
    }

    void CTB::UpdateConfig(const std::string& file_path)
    {
        #ifdef _WIN32
            std::vector<char> config_json = File(file_path).ReadAll();
            if (config_json.size()==0)
            {
                return;
            }
            auto data = 
            ProcessJsonConfiguration(std::string(config_json.begin(), config_json.end())).GetData();
            
            // named mutex lock for registry
            config_registry_mutex_.Lock();
            Registry reg("SOFTWARE/CtaProcessMonitoring/ProcsesConf");
            reg.DeleteContent();
            for (int i = 0; i < data.size(); i++)
            {
                std::vector<char> value;
                value.resize(sizeof(double) * 4);
                memcpy(&value[0], &data[i].second.cpu_usage, sizeof(double));
                memcpy(&value[sizeof(double) * 1], &data[i].second.mem_usage, sizeof(double));
                memcpy(&value[sizeof(double) * 2], &data[i].second.disk_usage, sizeof(double));
                memcpy(&value[sizeof(double) * 3], &data[i].second.network_usage, sizeof(double));
                reg.CreateBinaryValue(data[i].first, value);
            }
            config_registry_mutex_.Unlock();
            // named mutex unlock for registry

            client.TrySendMessage(Command::CTB_NOTI_CONFIG, std::vector<char>());
        #elif __linux__

        #endif
    }

    void CTB::GetLog(const std::string& cta_log_path)
    {
        std::string log_file_name = "pm_logs.log";
        File ctb_log = File(log_file_name);
        cta_log_mutex_.Lock();
        ctb_log.AppendFromFile(cta_log_path);
        File cta_log = File(cta_log_path);
        cta_log.SelfDelete();
        cta_log_mutex_.Unlock();
    }

    void CTB::CommunicateWithCta()
    {
        client = PipelineClient("\\\\.\\pipe\\processmonitoringpipe");
        while(true)
        {
            while(true)
            {
                if (client.ConnectToPipeServer() == true)
                {
                    break;
                }
                Sleep(1000);
            }
            
            while(true)
            {
                if (client.TryGetMessage() == false)
                {
                    Sleep(500);
                    continue;
                }
                if (client.GetLastMessageType() == Command::CTA_SEND_LOGS)
                {
                    std::string msg(client.GetLastMessage().begin(), client.GetLastMessage().end());
                    GetLog(msg);
                }
            }

        }
    }
}