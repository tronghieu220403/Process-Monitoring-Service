#ifndef PROCESSMONITORING_COMMUNICATION_PIPELINE_PIPE_H_
#define PROCESSMONITORING_COMMUNICATION_PIPELINE_PIPE_H_

#include "ulti/everything.h"
#include "mutex/mutex.h"

namespace pm
{
    class Pipeline
    {

        std::string name_;
        std::vector<char> last_receive_;
        int last_message_type_ = 0;

        bool connected_ = false;

        private:
            NamedMutex mutex_;
            #ifdef _WIN32
                HANDLE handle_pipe_ = INVALID_HANDLE_VALUE;
            #elif __linux__
                int fd_send_ = -1;
                int fd_recv_ = -1;
            #endif
        public:
            Pipeline() = default;
            #ifdef _WIN32
                Pipeline(HANDLE handle_pipe);
            #elif __linux__
                Pipeline(int fd_send, int fd_recv);
            #endif

            void SetPipelineName(const std::string& pipe_name);
            std::string GetPipelineName() const;

            void SetConnectStatus(bool status);
            #ifdef _WIN32
                void SetPipelineHandle(HANDLE handle_pipe);
            #elif __linux__
                void SetConnect(int fd_send, int fd_recv);
                void SetFdSend(int fd_send);
                void SetFdRecv(int fd_recv);
            #endif

            std::string GetPipelineName();
            std::vector<char> GetLastMessage();
            int GetLastMessageType();

            #ifdef _WIN32
                HANDLE GetHandle();
            #elif __linux__
                int GetFdSend();
                int GetFdRecv();
            #endif

            bool IsActive();

            bool TryGetMessage();
            bool TrySendMessage(int type, std::vector<char> data);

            void Close();

            ~Pipeline();
    };

}

#endif