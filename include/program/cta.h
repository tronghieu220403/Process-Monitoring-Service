
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
#include "etw/consumer.h"
#include "pdh/counter.h"

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

        #endif

        std::deque<std::vector<char>> log_deque_;
        std::vector<char> v_log_path_;

        NamedMutex cta_log_mutex_;
        NamedMutex inner_mutex_;
        NamedMutex config_mutex_;


    public:

        CTA();
        
        #ifdef _WIN32
        CTA(std::shared_ptr<ServiceEvent> event);

        #endif

        void UpdateConfig();
        void Monitoring();
        void RecvCommunication();
        void SendCommunication();
        void CommunicateWithCtb();
    };
}

#endif
