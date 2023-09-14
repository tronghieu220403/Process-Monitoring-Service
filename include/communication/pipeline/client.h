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
#ifdef _WIN32
#include <strsafe.h>
#include <tchar.h>
#elif __linux__
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#endif
#include <stdio.h>

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
    // Need to abstract a Pipe class 
    class PipelineClient
    {
    private:
        std::string server_name_;
        #ifdef _WIN32
            int buf_size_;
            HANDLE handle_pipe_;
        #elif __linux__
            int fd_send_;
            int fd_recv_;
        #endif

        std::vector<char> last_receive_;
        int last_message_type_;

    public:
        PipelineClient() = default;
        PipelineClient(const std::string& server_pipe_name);

        void SetServerPipeName(const std::string& server_pipe_name);
        
        #ifdef _WIN32
            void SetBufferSize(int buf_size);
        #endif
        std::string GetPipeName();
        std::vector<char> GetLastMessage();
        
        int GetLastMessageType();

        bool ConnectToPipeServer();

        bool IsActive();

        bool TryGetMessage();
        bool TrySendMessage(int type, std::vector<char> data);

        void Close();

        ~PipelineClient();
    };

}

#endif