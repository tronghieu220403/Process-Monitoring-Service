#ifdef _VISUAL_STUDIO_WORKSPACE
#pragma once
#endif

#ifndef PROCESSMONITORING_SRC_CTB_H_
#define PROCESSMONITORING_SRC_CTB_H_

#ifndef _VISUAL_STUDIO_WORKSPACE
#define _CRT_SECURE_NO_DEPRECATE
#define UNICODE
#define _UNICODE
#endif

#include "ulti/everything.h"
#include "process/processsupervision.h"
#include "ulti/collections.h"
#include "registry/registry.h"
#include "communication/pipeline/client.h"
#include "file/file.h"
#include "configuration/processjsonconfiguration.h"
#include "mutex/mutex.h"
#include "configuration/startupprogram.h"
#include "logs/logger.h"

namespace pm
{
    class CTB
    {
    private:
        PipelineClient client;

        NamedMutex cta_log_mutex_;
        NamedMutex config_mutex_;
        NamedMutex inner_mutex_;

        Logger log;

        bool new_config_ = true;

        #ifdef _WIN32
            HANDLE handle_pipe_;
        #elif __linux__

        #endif

    public:
        CTB();
        void AddToStartUp();
        void UpdateConfig(const std::string& file_path);
        void WriteLog(const std::string& content);
        void RecvCommunication();
        void SendCommunication();
        void CommunicateWithCta();
    };
}

#endif
