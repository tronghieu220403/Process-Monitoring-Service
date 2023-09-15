
#ifndef PROCESSMONITORING_SRC_CTA_H_
#define PROCESSMONITORING_SRC_CTA_H_

#ifdef _VISUAL_STUDIO_WORKSPACE
#include "E:/Code/Github/Process-Monitoring/include/ulti/everything.h"
#else
#include "include/ulti/everything.h"
#endif

#ifdef _VISUAL_STUDIO_WORKSPACE
#include "E:/Code/Github/Process-Monitoring/include/process/processsupervision.h"
#include "E:/Code/Github/Process-Monitoring/include/ulti/collections.h"
#include "E:/Code/Github/Process-Monitoring/include/registry/registry.h"
#include "E:/Code/Github/Process-Monitoring/include/communication/pipeline/server.h"
#include "E:/Code/Github/Process-Monitoring/include/configuration/startupprogram.h"
#include "E:/Code/Github/Process-Monitoring/include/mutex/mutex.h"
#else

#include "include/process/processsupervision.h"
#include "include/ulti/collections.h"
#include "include/registry/registry.h"
#include "include/communication/pipeline/server.h"
#include "include/configuration/startupprogram.h"
#include "include/mutex/mutex.h"
#include "include/file/file.h"

#endif

namespace pm
{
    class CTA
    {
    private:
        std::vector<ProcessSupervision> process_;
        std::string log_info_;
        PipelineServer server;
        
        NamedMutex cta_log_mutex_;
        NamedMutex inner_mutex_;
        NamedMutex config_mutex_;
    public:
        CTA();
        void AddToStartup();
        void UpdateConfig();
        void Monitoring();
        void CommunicateWithCtb();
    };
}

#endif
