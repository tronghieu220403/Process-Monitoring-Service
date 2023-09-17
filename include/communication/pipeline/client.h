#ifndef PROCESSMONITORING_COMMUNICATION_PIPELINE_CLIENT_H_
#define PROCESSMONITORING_COMMUNICATION_PIPELINE_CLIENT_H_

#ifdef _VISUAL_STUDIO_WORKSPACE
#include "E:/Code/Github/Process-Monitoring/include/ulti/everything.h"
#else
#include "include/ulti/everything.h"
#endif

namespace pm
{
    // Need to abstract a Pipe class 
    class PipelineClient
    {
    private:
        std::string server_name_;
        #ifdef _WIN32
            int buf_size_ = 0;
            HANDLE handle_pipe_ = 0;
        #elif __linux__
            int fd_send_ = -1;
            int fd_recv_ = -1;
        #endif

        std::vector<char> last_receive_;
        int last_message_type_ = 0;

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