#include "cta.h"

namespace pm
{

    CTA::CTA():
        cta_log_mutex_(NamedMutex("pm_cta_logs")),
        config_mutex_(NamedMutex("config_reg")),
        inner_mutex_(NamedMutex(""))
    {

    }

#ifdef _WIN32
    CTA::CTA(std::shared_ptr<ServiceEvent> event):
       event_(event)
    {
        this->CTA::CTA();
    }

#endif

    void CTA::UpdateConfig()
    {
        #ifdef _WIN32

            // named mutex lock for registry
            config_mutex_.Lock();
            Registry reg(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\CtaProcessMonitoring\\ProcsesConf");
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
                process_.push_back(std::make_shared<ProcessSupervision>(info[i].first, mc));
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
                process_.push_back(std::make_shared<ProcessSupervision>(name, mc));
            }

            config_mutex_.Unlock();
        #endif
    }

    long long GetTimeNow()
    {
        return
            std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
    }

    void CTA::Monitoring()
    {
        WriteDebug("Monitoring");
        CTA::UpdateConfig();

        long long run_time = 0;
        std::string log;
        
        #ifdef _WIN32
            std::vector<IoInfo> disk_data;
            std::vector<IoInfo> net_data;
        #endif
        
        while (true)
        {
            inner_mutex_.Lock();

            WriteDebug("Update query");
            long long start_time =
                std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
            
            for (auto& ps : process_)
            {
                ps->TryFindHandle();
                WriteDebug(ps->GetName() + " " + std::to_string(ps->GetPid()));
            }

            #ifdef _WIN32

            disk_data = KernelConsumer::GetDiskIoSharedVector();

            net_data = KernelConsumer::GetNetworkIoSharedVector();

            Counter::UpdateQuery();

            for (auto &data: disk_data)
            {
                for (auto& ps: process_)
                {
                    if (ps->GetPid() == data.pid)
                    {
                        ps->GetProcessInfo()->GetDiskUsageStats()->AddData(data.filetime, data.size);
                        #ifdef _DEBUG
                            SYSTEMTIME st;
                            FileTimeToSystemTime(&data.filetime, &st);
                            WriteDebug(std::to_string(st.wHour) + " " + std::to_string(st.wMinute) + " " + std::to_string(st.wSecond) + " " + "add disk data for PID " + std::to_string(net_data.size()));
                        #endif // _DEBUG

                        break;
                    }
                }
            }
            
            for (auto &data: net_data)
            {
                for (auto& ps: process_)
                {
                    if (ps->GetPid() == data.pid)
                    {
                        ps->GetProcessInfo()->GetNetworkUsageStats()->AddData(data.filetime, data.size);
                        SYSTEMTIME st;
                        FileTimeToSystemTime(&data.filetime, &st);
                        WriteDebug(std::to_string(st.wHour) + " " + std::to_string(st.wMinute) + " " + std::to_string(st.wSecond) + " " + "add net data for PID " + std::to_string(net_data.size()));
                        break;
                    }
                }
            }
            #endif

            for (auto& ps : process_)
            {
                ps->UpdateProcessStats();
                ps->CheckProcessStats();
                if ((ps->GetMessage()).size() != 0)
                {
                    log.append(ps->GetMessage());
                    ps->SetMessage("");
                }
            }
            
            inner_mutex_.Unlock();
            if (log.size() > 0)
            {
                WriteDebug("Write event to queue");
                cta_log_mutex_.Lock();
                log_deque_.push_back(StringToVectorChar(log));
                cta_log_mutex_.Unlock();
            }

            log.clear();

            #ifdef _WIN32
                disk_data.clear();
                net_data.clear();
            #endif

            run_time =
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch()).count() - start_time;
            #ifdef _DEBUG
                if (run_time < 10000)
                {
                    Sleep(1000 - run_time);
                }
            #else
                if (run_time < 1000)
                {
                    Sleep(1000 - run_time);
                }
            #endif // _DEBUG
        }
    }

    void CTA::RecvCommunication()
    {
        while (true)
        {
            WriteDebug("Getting...");
            if (server.TryGetMessage() == false)
            {
                Sleep(500);
                WriteDebug("RecvCommunication() server disconnected");
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
            std::vector<char> v;
            v.clear();
            cta_log_mutex_.Lock();
            if (log_deque_.size() > 0)
            {
                v = log_deque_.front();
            }
            cta_log_mutex_.Unlock();

            if (v.size() > 0)
            {
                WriteDebug("Sending " + v.size());
                if (server.TrySendMessage(Command::CTA_SEND_LOGS, v) == false)
                {
                    WriteDebug("SendCommunication() server disconnected");
                    return;
                }
                WriteDebug("Sent oke");

                cta_log_mutex_.Lock();
                log_deque_.pop_front();
                cta_log_mutex_.Unlock();
            }

            if (!server.IsActive())
            {
                WriteDebug("SendCommunication() server disconnected");
                return;
            }
            if (log_deque_.size() == 0)
            {
                Sleep(500);
            }
        }
    }

    void CTA::CommunicateWithCtb()
    {
        server = PipelineServer("processmonitoringpipe");
        #ifdef _WIN32
            server.SetMaxConnection(1);
            server.SetBufferSize(10000000);
        #endif
        while(true)
        {
            WriteDebug("Creating server");
            while(true)
            {
                if (server.CreateServer() == true)
                {
                    break;
                }
                Sleep(1000);
            }
            WriteDebug("Listening to client");
            while (server.ListenToClient() == false){
                Sleep(100);
            }
            WriteDebug("Client connected!");

            std::jthread recv(std::bind_front(&pm::CTA::RecvCommunication, this));
            std::jthread send(std::bind_front(&pm::CTA::SendCommunication, this));
            recv.join();
            send.join();

            WriteDebug("Client disconnected!");

            server.Close();
            Sleep(100);

        }
    }

    
}