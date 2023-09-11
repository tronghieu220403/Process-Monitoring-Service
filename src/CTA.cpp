#include "src/CTA.h"

namespace pm
{
    CTA::CTA() = default;

    void CTA::UpdateConfig()
    {
        #ifdef _WIN32
            //using mutex here
            if (new_config_ == false)
            {
                return;
            }
            //end mutex here

            // named mutex lock for registry
            Registry reg("SOFTWARE/CtaProcessMonitoring/ProcsesConf");
            std::vector< std::pair< std::string, std::vector<char> > > info = reg.GetAllBinaryValues();
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
            // named mutex unlock for registry

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
                    ps.GetProcessLogger()->WriteLog();
                    // named mutex unlock for log file
                }
            }
            if (server.IsActive())
            {
                std::vector<char> log_path;
                log_path.resize(1000);
                //int len = GetModuleFileNameA(NULL, &log_path[0], 1000);
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
            
            while(server.ListenToClient() == false);

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
                    new_config_ = true;
                    // using mutex here
                }
            }

        }
    }
}