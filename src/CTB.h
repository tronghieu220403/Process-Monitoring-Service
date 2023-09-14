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

#include <string.h>

#ifdef _WIN32
    #include <direct.h>
#else
    #include <unistd.h>
 #endif

#ifdef _WIN32
#include <Windows.h>
#endif

#ifdef _VISUAL_STUDIO_WORKSPACE
#include "E:/Code/Github/Process-Monitoring/include/process/processsupervision.h"
#include "E:/Code/Github/Process-Monitoring/include/ulti/collections.h"
#include "E:/Code/Github/Process-Monitoring/include/registry/registry.h"
#include "E:/Code/Github/Process-Monitoring/include/communication/pipeline/client.h"
#include "E:/Code/Github/Process-Monitoring/include/file/file.h"
#include "E:/Code/Github/Process-Monitoring/include/configuration/processjsonconfiguration.h"
#include "E:/Code/Github/Process-Monitoring/include/mutex/mutex.h"
#include "E:/Code/Github/Process-Monitoring/include/ulti/everything.h"

#else
#include "include/process/processsupervision.h"
#include "include/ulti/collections.h"
#include "include/registry/registry.h"
#include "include/communication/pipeline/client.h"
#include "include/file/file.h"
#include "include/configuration/processjsonconfiguration.h"
#include "include/mutex/mutex.h"
#include "include/ulti/everything.h"
#endif


#include <iostream>
#include <string>
#include <vector>

namespace pm
{
    class CTB
    {
    private:
        PipelineClient client;

        NamedMutex cta_log_mutex_;
        NamedMutex config_registry_mutex_;
        NamedMutex inner_mutex_;

        #ifdef _WIN32
            HANDLE handle_pipe_;
        #elif __linux__

        #endif

    public:
        CTB();
        void UpdateConfig(const std::string& file_path);
        void GetLog(const std::string& cta_logs_path);
        void CommunicateWithCta();
    };
}

#endif
