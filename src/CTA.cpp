#include "src/CTA.h"

namespace pm
{

    CTA::CTA()
    {
        cta_log_mutex_ = NamedMutex("pm_cta_logs");
        config_mutex_ = NamedMutex("config_reg");
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

            // named mutex lock for registry
            config_mutex_.Lock();
            Registry reg("SOFTWARE/CtaProcessMonitoring/ProcsesConf");
            std::vector< std::pair< std::string, std::vector<char> > > info = reg.GetAllBinaryValues();
            config_mutex_.Unlock();
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

        #elif __linux__

        #endif
    }

    void CTA::Monitoring()
    {
        while(true)
        {
            Sleep(500);
            inner_mutex_.Lock();
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
                std::string log_name = "pm_logs.log";
                for (char c: log_name)
                {
                    log_path.push_back(c);
                }
                server.TrySendMessage(Command::CTA_SEND_LOGS, log_path);
            }
            inner_mutex_.Unlock();
        }
    }

    void CTA::CommunicateWithCtb()
    {
        server = PipelineServer("processmonitoringpipe");
        #ifdef _WIN32
            server.SetMaxConnection(1);
        #endif
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
                    Sleep(500);
                    continue;
                }
                if (server.GetLastMessageType() == Command::CTB_NOTI_CONFIG)
                {
                    inner_mutex_.Lock();
                    UpdateConfig();
                    inner_mutex_.Unlock();
                }
            }

        }
    }
}