#include "src/CTA.h"

namespace pm
{

    CTA::CTA()
    {
        cta_log_mutex_ = NamedMutex("pm_cta_logs");
        config_registry_mutex_ = NamedMutex("config_reg");
        inner_mutex_ = NamedMutex("");
    }

    void CTA::AddToStartup()
    {
        StartUpProgram startup = StartUpProgram();
        startup.SetThisProgram();
        startup.Register();
    }

    void CTA::UpdateConfig()
    {
        #ifdef _WIN32
            inner_mutex_.Lock();
            //using mutex here
            if (new_config_ == false)
            {
                return;
            }
            inner_mutex_.Unlock();
            //end mutex here

            // named mutex lock for registry
            config_registry_mutex_.Lock();
            Registry reg("SOFTWARE/CtaProcessMonitoring/ProcsesConf");
            std::vector< std::pair< std::string, std::vector<char> > > info = reg.GetAllBinaryValues();
            config_registry_mutex_.Unlock();
            // named mutex unlock for registry

            process_.clear();
            
            for (int i = 0; i < info.size(); i++)
            {
                MonitoringComponent mc;
                memcpy(&mc.cpu_usage, &(info[i].second[0 * sizeof(double)]), sizeof(double));
                memcpy(&mc.mem_usage, &(info[i].second[1 * sizeof(double)]), sizeof(double));
                memcpy(&mc.disk_usage, &(info[i].second[2 * sizeof(double)]), sizeof(double));
                memcpy(&mc.network_usage, &(info[i].second[3 * sizeof(double)]), sizeof(double));
                process_.push_back(ProcessSupervision(info[i].first, mc));
            }

            inner_mutex_.Lock();
            //using mutex here
            new_config_ = false;
            inner_mutex_.Unlock();
            //end mutex here


        #elif __linux__

        #endif
    }

    void CTA::Monitoring()
    {
        while(true)
        {
            Sleep(1000);
            UpdateConfig();
            for (auto ps: process_)
            {
                ps.UpdateProcessStats();
                ps.CheckProcessStats();
                if ((ps.GetProcessLogger()->GetMessage()).size() != 0)
                {
                    // named mutex lock for log file
                    cta_log_mutex_.Lock();
                    ps.GetProcessLogger()->WriteLog();
                    cta_log_mutex_.Unlock();
                    // named mutex unlock for log file
                }
            }
            if (server.IsActive())
            {
                std::vector<char> log_path;
                log_path.resize(1000);
                GetCurrentDir(&log_path[0], 1000);
                log_path.resize(strlen(&log_path[0]));
                log_path.push_back('\\');
                server.TrySendMessage(Command::CTA_SEND_LOGS, log_path);
            }
        }
    }

    void CTA::CommunicateWithCtb()
    {
        server = PipelineServer("\\\\.\\pipe\\processmonitoringpipe");
        server.SetMaxConnection(1);
        while(true)
        {
            while(true)
            {
                if (server.CreateServer() == true)
                {
                    break;
                }
                Sleep(1000);
            }
            
            while(server.ListenToClient() == false)
            {
                Sleep(100);
            }

            while(true)
            {
                if (server.TryGetMessage() == false)
                {
                    Sleep(1000);
                    continue;
                }
                if (server.GetLastMessageType() == Command::CTB_NOTI_CONFIG)
                {
                    // using mutex here
                    inner_mutex_.Lock();
                    new_config_ = true;
                    inner_mutex_.Unlock();
                    // using mutex here
                }
            }

        }
    }
}