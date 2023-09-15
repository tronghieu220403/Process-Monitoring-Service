#ifndef PROCESSMONITORING_COMMUNICATION_PIPELINE_SERVER_H_
#define PROCESSMONITORING_COMMUNICATION_PIPELINE_SERVER_H_

#ifdef _VISUAL_STUDIO_WORKSPACE
#include "E:/Code/Github/Process-Monitoring/include/ulti/everything.h"
#else
#include "include/ulti/everything.h"
#endif

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