#include "ctb.h"
namespace pm
{

    CTB::CTB():
        cta_log_mutex_(NamedMutex("pm_cta_log")),
        config_mutex_(NamedMutex("config_reg")),
        inner_mutex_(NamedMutex(""))
    {
    }

    void CTB::AddToStartUp()
    {
        StartUpProgram startup = StartUpProgram();
        startup.SetThisProgram();
        startup.Register();
    }


    void CTB::UpdateConfig(const std::string& file_path)
    {
        std::vector<char> config_json = File(file_path).ReadAll();
        
        if (config_json.empty())
        {
            return;
        }

        auto data = 
        ProcessJsonConfiguration(std::string(config_json.begin(), config_json.end())).GetData();

        #ifdef _WIN32
            
            // named mutex lock for registry
            config_mutex_.Lock();
            Registry reg("SOFTWARE\\CtaProcessMonitoring\\ProcsesConf");
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
            reg.Close();
            config_mutex_.Unlock();
            // named mutex unlock for registry

        #elif __linux__
            
            std::string pm_config_path = GetCurrentUserPath() + "/.config/Process Monitoring";

            if (CreateFolder(pm_config_path) == false)
            {
                return;
            }

            config_mutex_.Lock();
            
            std::ofstream outfile(pm_config_path + "/ProcessMonitoringStats");

            for (int i = 0; i < data.size(); i++)
            {
                outfile << data[i].first << std::endl;
                outfile << data[i].second.cpu_usage << std::endl;
                outfile << data[i].second.mem_usage << std::endl;
                outfile << data[i].second.disk_usage << std::endl;
                outfile << data[i].second.network_usage << std::endl;
            }

            outfile.close();

            config_mutex_.Unlock();


        #endif
        inner_mutex_.Lock();
        new_config_ = true;
        inner_mutex_.Unlock();
    }

    void CTB::WriteLog(const std::string& content)
    {
        log.SetMessage(content);
        log.WriteLog();
    }

    void CTB::RecvCommunication()
    {
        while (true)
        {
             std::osyncstream(std::cout) << "Receiving" << std::endl;
            if (client.TryGetMessage() == false)
            {
                return;
            }
            if (client.GetLastMessageType() == Command::CTA_SEND_LOGS)
            {
                 std::osyncstream(std::cout) << "Message received: " << client.GetLastMessage().size() << std::endl;
                WriteLog(CharVectorToString(client.GetLastMessage()));
            }
            Sleep(500);
        }
    }

    void CTB::SendCommunication()
    {
        while (true)
        {
            if (new_config_ == true)
            {
                inner_mutex_.Lock();
                new_config_ = false;
                inner_mutex_.Unlock();
                if (client.TrySendMessage(Command::CTB_NOTI_CONFIG, std::vector<char>()) == false)
                {
                    return;
                }
            }
            if (!client.IsActive())
            {
                return;
            }
            Sleep(500);
        }
    }

    void CTB::CommunicateWithCta()
    {
        client = PipelineClient("processmonitoringpipe");
        while(true)
        {
             std::osyncstream(std::cout) << "Connecting" << std::endl;
            while(true)
            {
                if (client.ConnectToPipeServer() == true)
                {
                    break;
                }
                Sleep(50);
            }
             std::osyncstream(std::cout) << "Server connected" << std::endl;
            std::jthread recv(std::bind_front(&pm::CTB::RecvCommunication, this));
            std::jthread send(std::bind_front(&pm::CTB::SendCommunication, this));
            recv.join();
            send.join();
            std::osyncstream(std::cout) << "Server disconnected" << std::endl;
            client.Close();
        }
    }
}