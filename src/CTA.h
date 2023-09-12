#ifdef _VISUAL_STUDIO_WORKSPACE
#pragma once
#endif

#ifndef PROCESSMONITORING_SRC_CTA_H_
#define PROCESSMONITORING_SRC_CTA_H_

#ifndef _VISUAL_STUDIO_WORKSPACE
#define _CRT_SECURE_NO_DEPRECATE
#define UNICODE
#define _UNICODE
#endif

#include <string.h>

#ifdef _WIN32
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
 #endif

#ifdef _WIN32
#include <Windows.h>
#endif

#ifdef _VISUAL_STUDIO_WORKSPACE
#include "E:/Code/Github/Process-Monitoring/include/process/processsupervision.h"
#include "E:/Code/Github/Process-Monitoring/include/ulti/collections.h"
#include "E:/Code/Github/Process-Monitoring/include/registry/registry.h"
#include "E:/Code/Github/Process-Monitoring/include/communication/pipeline/server.h"
#include "E:/Code/Github/Process-Monitoring/include/configuration/startupprogram.h"

#else

#include "include/process/processsupervision.h"
#include "include/ulti/collections.h"
#include "include/registry/registry.h"
#include "include/communication/pipeline/server.h"
#include "include/configuration/startupprogram.h"
#endif


#include <iostream>
#include <string>
#include <vector>

namespace pm
{
    class CTA
    {
    private:
        std::vector<ProcessSupervision> process_;
        std::string log_info_;
        bool new_config_;
        PipelineServer server;
        #ifdef _WIN32
            HANDLE cta_log_mutex_;
            HANDLE config_registry_mutex_;
            HANDLE inner_mutex_;
        #elif __linux__

        #endif
    public:
        CTA();
        void AddToStartup();
        void UpdateConfig();
        void Monitoring();
        void CommunicateWithCtb();
    };
}

#endif
