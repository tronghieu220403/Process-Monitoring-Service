
#ifndef PROCESSMONITORING_SRC_CTA_H_
#define PROCESSMONITORING_SRC_CTA_H_

#include "ulti/everything.h"
#include "process/processsupervision.h"
#include "ulti/collections.h"
#include "registry/registry.h"
#include "communication/pipeline/server.h"
#include "mutex/mutex.h"
#include "file/file.h"
#include "service/serviceevent.h"

namespace pm
{
    class CTA
    {
    private:
        std::vector<ProcessSupervision> process_;
        std::string log_info_;
        PipelineServer server;
        
        #ifdef _WIN32
            std::shared_ptr<ServiceEvent> event_;
            std::deque<IoInfo> disk_io_deque_;
            std::deque<IoInfo> network_io_deque_;

            std::shared_ptr<std::deque<IoInfo>> disk_io_vector_;
            std::shared_ptr<std::deque<IoInfo>> network_io_vector_;

            NamedMutex network_mutex_;
            NamedMutex disk_io_mutex_;

        #endif

        bool new_log_ = false;
        std::vector<char> v_log_path_;
        
        NamedMutex cta_log_mutex_;
        NamedMutex inner_mutex_;
        NamedMutex config_mutex_;


    public:

        CTA();
        
        #ifdef _WIN32
        CTA(std::shared_ptr<ServiceEvent> event);

        void SetDiskIoVector(std::shared_ptr< std::vector<IoInfo>> io_info);
        std::shared_ptr< std::deque<IoInfo> > GetDiskIoVector() const;

        void SetNetworkIoVector(std::shared_ptr<std::vector<IoInfo>> io_info);
        std::shared_ptr< std::deque<IoInfo> > GetNetworkIoVector() const;

        void AddIoData(int type, IoInfo io);
        void CopyData();

        #endif

        void UpdateConfig();
        void Monitoring();
        void RecvCommunication();
        void SendCommunication();
        void CommunicateWithCtb();
    };
}

#endif
