#include "cta.h"

namespace pm
{

    CTA::CTA():
        cta_log_mutex_(NamedMutex("pm_cta_logs")),
        config_mutex_(NamedMutex("config_reg")),
        inner_mutex_(NamedMutex(""))
        #ifdef _WIN32
            , network_mutex_(NamedMutex(""))
            , disk_io_mutex_(NamedMutex("disk_io_mutex"))
        #endif
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

#ifdef _WIN32
    CTA::CTA(std::shared_ptr<ServiceEvent> event):
       event_(event)
    {
        this->CTA::CTA();
    }

    void CTA::SetDiskIoVector(std::shared_ptr<std::vector<IoInfo>> io_info)
    {
        disk_io_vector_ = io_info;
    }
    
    std::shared_ptr<std::deque<IoInfo>> CTA::GetDiskIoVector() const
    {
        return disk_io_vector_;
    }

    void CTA::SetNetworkIoVector(std::shared_ptr<std::vector<IoInfo>> io_info)
    {
        network_io_vector_ = io_info;
    }

    std::shared_ptr<std::deque<IoInfo>> CTA::GetNetworkIoVector() const
    {
        return network_io_vector_;
    }

    void CTA::AddIoData(int type, IoInfo io)
    {
        if (type == IoType::DISK_IO)
        {
            disk_io_vector_->push_back(io);
        }
        else if (type == IoType::NETWORK_IO)
        {
            network_io_vector_->push_back(io);
        }
    }

    void CTA::CopyData()
    {

    }

#endif

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

        #ifdef _WIN32

        // Ý tưởng là phân tích từng cái một (cpu, process, disk, net), ghi log riêng chứ không ghép hết vào như cách cũ.
        
        // Muốn thế thì ở class ProcessInfo, các phần update cần có thêm thời gian ghi log, ý tưởng là thêm 4 mục thời gian ghi log vào pm::MonitoringComponent

        // Sẽ có một deque chung dùng shared_ptr để lưu các sự kiện ETW đã rút gọn, sau đó ở CTA sẽ vét deque đó và phân vào các process. 

        // Mỗi pm::ProcessSupervision khi được update stat bằng UpdateProcessStats(), mỗi net và disk cần vét sạch InfoIo của nó và ghi ra một vector kết quả vector< pair<time, value> >

        // pm::ProcessSupervision mỗi lần check log bằng CheckProcessStats(), cần check hết sạch vector< pair<time, value> > của net và disk.

        // pm::ProcessNetworkStats và pm::ProcessDiskStats cần thêm 1 deque lưu lại các sự kiện pm::IoInfo để phân tích lúc dùng UpdateAttributes()

        #elif _linux__

        long long cnt = 0;
        std::string log;

        while (true)
        {
            Sleep(500);
            inner_mutex_.Lock();
            for (auto& ps : process_)
            {
                ps.UpdateProcessStats();
                ps.CheckProcessStats();
                if ((ps.GetProcessLogger()->GetMessage()).size() != 0)
                {
                    log.append(ps.GetProcessLogger()->GetMessage());
                    ps.GetProcessLogger()->SetMessage("");
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

        #endif
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
                if (server.TrySendMessage(Command::CTA_SEND_LOGS, v_log_path_) == false)
                {
                    return;
                }
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
            server.SetBufferSize(10000000);
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
            while (server.ListenToClient() == false){
                Sleep(100);
            }

            std::jthread recv(std::bind_front(&pm::CTA::RecvCommunication, this));
            std::jthread send(std::bind_front(&pm::CTA::SendCommunication, this));
            recv.join();
            send.join();

            Sleep(100);

        }
    }

    
}