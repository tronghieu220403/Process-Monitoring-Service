#ifdef _VISUAL_STUDIO_WORKSPACE
#pragma once
#endif

#ifndef PROCESSMONITORING_COMMUNICATION_PIPELINE_CLIENT_H_
#define PROCESSMONITORING_COMMUNICATION_PIPELINE_CLIENT_H_

#define _CRT_SECURE_NO_DEPRECATE

#ifndef _VISUAL_STUDIO_WORKSPACE
#define UNICODE
#define _UNICODE
#endif


#include <string.h>
#include <time.h>
#include <strsafe.h>
#include <stdio.h>
#include <tchar.h>

#ifdef _WIN32
#include <Windows.h> 
#elif __linux__

#endif

#include <string>
#include <fstream>
#include <sstream>

namespace pm
{
    class PipelineClient
    {
    private:
        std::string server_name_;
    public:
        PipelineClient();
        PipelineClient(const std::string& pipe_name);

        void SetServerName(const std::string& pipe_name);
        bool ConnectToServer();

        bool ReceiveMessage();
        bool SendMessage();

    };
        
}

#endif