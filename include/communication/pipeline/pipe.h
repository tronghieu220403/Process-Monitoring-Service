#ifndef PROCESSMONITORING_COMMUNICATION_PIPELINE_SERVER_H_
#define PROCESSMONITORING_COMMUNICATION_PIPELINE_SERVER_H_

#ifdef _VISUAL_STUDIO_WORKSPACE
#include "E:/Code/Github/Process-Monitoring/include/ulti/everything.h"
#else
#include "include/ulti/everything.h"
#endif

namespace pm
{
    class Pipeline
    {

        std::string name_;
        std::vector<char> last_receive_;
        int last_message_type_ = 0;

        bool connected_ = false;

        private:
            #ifdef _WIN32
                HANDLE handle_pipe_ = 0;
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

            void SetPipeName(const std::string& pipe_name);
            #ifdef _WIN32
                void SetConnect(HANDLE handle_pipe);
            #elif __linux__
                void SetConnect(int fd_send, int fd_recv);
            #endif

            std::string GetPipeName();
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

    };

}

#endif