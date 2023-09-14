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
#ifdef _WIN32
#include <strsafe.h>
#include <tchar.h>
#elif __linux__
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#endif

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
    class PipelineServer
    {
    private:
        std::string server_name_;
        std::vector<char> last_receive_;
        int last_message_type_;

        #ifdef _WIN32
            int buf_size_;
            int max_connection_;
            HANDLE handle_pipe_;
        #elif __linux__
            int fd_send_;
            int fd_recv_;
        #endif
    public:
        PipelineServer() = default;
        PipelineServer(const std::string& pipe_name);
        
        void SetPipeName(const std::string& pipe_name);
        #ifdef _WIN32
            PipelineServer(const std::string& pipe_name, int buf_size);
            bool SetMaxConnection(int max_connection);
            void SetBufferSize(int buf_size);
            int GetBufferSize();
        #endif

        std::string GetPipeName();
        std::vector<char> GetLastMessage();
        int GetLastMessageType();

        bool IsActive();

        unsigned long int CreateServer();
        unsigned long int ListenToClient();

        bool TryGetMessage();
        bool TrySendMessage(int type, std::vector<char> data);

        void Close();

        ~PipelineServer();
    };
        
}

#endif