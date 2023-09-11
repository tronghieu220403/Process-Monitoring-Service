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
#include <vector>

namespace pm
{
    class PipelineClient
    {
    private:
        std::string server_name_;
        int buf_size_;
        #ifdef _WIN32
            HANDLE handle_pipe_;
            int n_remaining_;
            std::vector<char> last_receive_;
            int type_;
        #elif __linux__

        #endif
    public:
        PipelineClient() = default;
        PipelineClient(const std::string& server_pipe_name);

        void SetServerPipeName(const std::string& server_pipe_name);
        void SetBufferSize(int buf_size);

        std::string GetPipeName();
        std::vector<char> GetLastMessage();
        
        int GetType();

        bool ConnectToPipeServer();

        bool IsActive();

        bool TryGetMessage();
        bool TrySendMessage(int type, std::vector<char> data);

        void Close();

    };

}

#endif