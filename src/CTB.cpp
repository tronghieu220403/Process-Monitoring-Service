#include "src/CTB.h"

namespace pm
{

    CTB::CTB() = default;

    void CTB::UpdateConfig(const std::string& file_path)
    {
        #ifdef _WIN32
            std::vector<char> config_json = File(file_path).ReadAll();
            auto data = 
            ProcessJsonConfiguration(std::string(config_json.begin(), config_json.end())).GetData();
            
            // named mutex lock for registry
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
            // named mutex unlock for registry
            client.TrySendMessage(Command::CTB_NOTI_CONFIG, std::vector<char>());
        #elif __linux__

        #endif
    }

    void CTB::GetLog()
    {
        //using mutex here
        if (new_log_ == false)
        {
            return;
        }
        //end mutex here

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
                    Sleep(1000);
                    continue;
                }
                if (client.GetLastMessageType() == Command::CTA_SEND_LOGS)
                {
                    // using mutex here
                    new_log_ = true;
                    // using mutex here
                }
            }

        }
    }
}