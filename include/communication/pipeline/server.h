#ifdef _VISUAL_STUDIO_WORKSPACE
#pragma once
#endif

#ifndef PROCESSMONITORING_COMMUNICATION_PIPELINE_SERVER_H_
#define PROCESSMONITORING_COMMUNICATION_PIPELINE_SERVER_H_

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
    class PipelineServer
    {
    private:
        std::string server_name_;
        int buf_size_;
        int max_connection_;
        std::vector<char> last_receive_;
        int last_message_type_;

        #ifdef _WIN32
            HANDLE handle_pipe_;
            int n_remaining_;
        #elif __linux__

        #endif
    public:
        PipelineServer() = default;
        PipelineServer(const std::string& pipe_name);
        PipelineServer(const std::string& pipe_name, int buf_size);
        
        void SetPipeName(const std::string& pipe_name);
        void SetBufferSize(int buf_size);
        bool SetMaxConnection(int max_connection);

        std::string GetPipeName();
        int GetBufferSize();
        std::vector<char> GetLastMessage();
        int GetLastMessageType();

        bool IsActive();

        unsigned long int CreateServer();
        unsigned long int ListenToClient();

        bool TryGetMessage();
        bool TrySendMessage(int type, std::vector<char> data);

        void Close();

    };
        
}

#endif