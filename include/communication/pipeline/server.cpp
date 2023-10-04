#include "server.h"

namespace pm
{
    PipelineServer::PipelineServer(const std::string& pipe_name)
    {
        Pipeline::SetPipelineName(pipe_name);
    };

#ifdef _WIN32

    PipelineServer::PipelineServer(const std::string& pipe_name, int buf_size)
        : buf_size_(buf_size)
    {
        Pipeline::SetPipelineName(pipe_name);
    };

    PipelineServer::PipelineServer(const std::string& pipe_name, int buf_size, int max_connection)
        : buf_size_(buf_size)
    {
        Pipeline::SetPipelineName(pipe_name);
        PipelineServer::SetMaxConnection(max_connection);
    }

    void PipelineServer::SetBufferSize(int buf_size)
    {
        buf_size_ = buf_size;
    }

    bool PipelineServer::SetMaxConnection(int max_connection)
    {
        #ifdef _WIN32
            if (max_connection_ != 0 || max_connection > PIPE_UNLIMITED_INSTANCES)
            {
                return false;
            }
        #endif
        max_connection_ = max_connection;
        return true;
    }

    int PipelineServer::GetMaximumConnection()
    {
        return max_connection_;
    }

    int PipelineServer::GetBufferSize()
    {
        return buf_size_;
    }

#endif

    bool PipelineServer::CreateServer()
    {
        #ifdef _WIN32

            std::string server_pipe = R"(\\.\pipe\)" + Pipeline::GetPipelineName();
            HANDLE handle_pipe = CreateNamedPipe(
                (std::wstring(server_pipe.begin(), server_pipe.end())).data(),             // pipe name 
                PIPE_ACCESS_DUPLEX,         // read/write access 
                PIPE_TYPE_MESSAGE |         // message type pipe 
                PIPE_READMODE_MESSAGE |     // message-read mode 
                //PIPE_NOWAIT,                // blocking mode: NON-BLOCKING
                PIPE_WAIT,                  // blocking mode: BLOCKING
                max_connection_,            // max. instances  
                buf_size_ + 100,                  // output buffer size 
                buf_size_ + 100,                  // input buffer size 
                0,                          // client time-out 
                nullptr);                   // default security attribute 
            
            if (handle_pipe == INVALID_HANDLE_VALUE)
            {
                return false;
            }
            Pipeline::SetPipelineHandle(handle_pipe);

            return true;

        #elif __linux__

            std::string server_send = "/tmp/" + Pipeline::GetPipelineName() + "serversend";
            std::string server_recv = "/tmp/" + Pipeline::GetPipelineName() + "serverrecv";

            int fd_send = -1;
            int fd_recv = -1;

            if (mkfifo(server_send.data(), 0666) == -1)                      /* read/write for user/group/others */
            {
                if ( errno != EEXIST )
                {
                    return false;
                }
            }
                
            if (mkfifo(server_recv.data(), 0666) == -1)                      /* read/write for user/group/others */
            {
                if ( errno != EEXIST )
                {
                    return false;
                }
            }

        #endif
        
        return true;
   
    }

    bool PipelineServer::ListenToClient()
    {        
        #ifdef _WIN32
            if (Pipeline::GetHandle() == INVALID_HANDLE_VALUE)
            {
                return false;
            }
            Pipeline::SetConnectStatus(ConnectNamedPipe(Pipeline::GetHandle(), nullptr) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED));

        #elif __linux__

            std::string server_send = "/tmp/" + Pipeline::GetPipelineName() + "serversend";
            std::string server_recv = "/tmp/" + Pipeline::GetPipelineName() + "serverrecv";

            int fd_send = -1;
            int fd_recv = -1;

            fd_send = open(server_send.data(), O_WRONLY); /* open as write-only */

            fd_recv = open(server_recv.data(), O_RDONLY);

            Pipeline::SetConnect(int fd_send, int fd_recv);

        #endif

        return Pipeline::IsActive();

    }

    PipelineServer::~PipelineServer()
    {
        Pipeline::Close();
    }
}
