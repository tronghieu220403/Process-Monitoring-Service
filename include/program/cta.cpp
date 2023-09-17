#ifdef _VISUAL_STUDIO_WORKSPACE
#include "cta.h"
#else
#include "include/program/cta.h"
#endif // _VISUAL_STUDIO_WORKSPACE


namespace pm
{

    CTA::CTA():
        cta_log_mutex_(NamedMutex("pm_cta_logs")),
        config_mutex_(NamedMutex("config_reg")),
        inner_mutex_(NamedMutex(""))
    {
        std::vector<char> log_path;
        log_path.resize(1000);
        GetCurrentDir(&log_path[0], 1000);
        log_path.resize(strlen(&log_path[0]));
#ifdef _WIN32
        log_path.push_back('\\');
#elif __linux__
        log_path.push_back('/');
#endif
        for (std::string log_name = "pm_logs.log"; char c : log_name)
        {
            log_path.push_back(c);
        }
            v_log_path_ = log_path;

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
            Registry reg("SOFTWARE\\CtaProcessMonitoring\\ProcsesConf");
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

            config_mutex_.Lock();

            process_.clear();

            std::stringstream ss(CharVectorToString(File(GetCurrentUserPath() + "/.config/Process Monitoring" + "/ProcessMonitoringStats").ReadAll()));

            std::string name;
            MonitoringComponent mc;

            while(ss >> name)
            {
                ss >> mc.cpu_usage;
                ss >> mc.mem_usage;
                ss >> mc.disk_usage;
                ss >> mc.network_usage;
                process_.push_back(ProcessSupervision(name, mc));
            }

            config_mutex_.Unlock();
        #endif
    }

    void CTA::Monitoring()
    {
        CTA::UpdateConfig();

        long long cnt = 0;
        std::string log;

        while (true)
        {
            Sleep(500);
            inner_mutex_.Lock();
            for (auto& ps : process_)
            {
                //std::cout << ps.GetProcessController()->GetPid() << std::endl;
                ps.UpdateProcessStats();
                ps.CheckProcessStats();
                if ((ps.GetProcessLogger()->GetMessage()).size() != 0)
                {
                    // named mutex lock for log file
                    //cta_log_mutex_.Lock();
                    //iii++;
                    //std::cout << "Lock log " << iii << std::endl;
                    log.append(ps.GetProcessLogger()->GetMessage());
                    ps.GetProcessLogger()->SetMessage("");
                    //cta_log_mutex_.Unlock();
                    // named mutex unlock for log file
                }
            }

            if (log.size() > 100)
            {
                cta_log_mutex_.Lock();
                Logger(log).WriteLog();
                log.clear();
                cta_log_mutex_.Unlock();
            }
            cnt++;
            if (server.IsActive() && cnt % 10 == 0)
            {
                new_log_ = true;
            }
            inner_mutex_.Unlock();
        }
    }

    void CTA::RecvCommunication()
    {
        while (true)
        {
            if (server.TryGetMessage() == false)
            {
                Sleep(500);
                return;
            }
            else
            {
                if (server.GetLastMessageType() == Command::CTB_NOTI_CONFIG)
                {
                    inner_mutex_.Lock();
                    UpdateConfig();
                    inner_mutex_.Unlock();
                }
                Sleep(500);
            }
        }
    }

    void CTA::SendCommunication()
    {
        while (true)
        {
            if (new_log_ == true)
            {
                inner_mutex_.Lock();
                new_log_ = false;
                inner_mutex_.Lock();
                std::cout << "Send msg" << std::endl;
                if (server.TrySendMessage(Command::CTA_SEND_LOGS, v_log_path_) == false)
                {
                    return;
                }
                std::cout << "Send oke.\n";
            }
            if (!server.IsActive())
            {
                return;
            }
            Sleep(500);
        }
    }

    void CTA::CommunicateWithCtb()
    {
        server = PipelineServer("processmonitoringpipe");
        #ifdef _WIN32
            server.SetMaxConnection(1);
            server.SetBufferSize(10000);
        #endif
        while(true)
        {
            std::cout << "Server Creating...\n";
            while(true)
            {
                if (server.CreateServer() == true)
                {
                    break;
                }
                Sleep(1000);
            }
            std::cout << "Server Created...\n";
            while (server.ListenToClient() == false){
                Sleep(100);
            }

            std::cout << "Client connected" << std::endl;

            std::jthread recv(std::bind_front(&pm::CTA::RecvCommunication, this));
            std::jthread send(std::bind_front(&pm::CTA::SendCommunication, this));
            recv.join();
            send.join();

            std::cout << "Client disconnected" << std::endl;

            Sleep(100);

        }
    }

    
}