#include <include/communication/pipeline/server.h>

namespace pm
{
    PipelineServer::PipelineServer(const std::string& pipe_name)
        : server_name_(pipe_name)
    {

    };

    PipelineServer::PipelineServer(const std::string& pipe_name, int buf_size)
        : server_name_(pipe_name), buf_size_(buf_size_)
    {

    };

    void PipelineServer::SetServerName(const std::string& pipe_name)
    {
        server_name_ = pipe_name;
    }

    void PipelineServer::SetBufferSize(int buf_size)
    {
        buf_size_ = buf_size;
    }

    std::string PipelineServer::GetServername()
    {
        return server_name_;
    }

    int PipelineServer::GetBufferSize()
    {
        return buf_size_;
    }

    unsigned long int PipelineServer::CreateServer()
    {
        handle_pipe_ = CreateNamedPipe(
            (std::wstring(server_name_.begin(), server_name_.end())).data(),             // pipe name 
            PIPE_ACCESS_DUPLEX,       // read/write access 
            PIPE_TYPE_MESSAGE |       // message type pipe 
            PIPE_READMODE_MESSAGE |   // message-read mode 
            PIPE_WAIT,                // blocking mode 
            1, // max. instances  
            buf_size_,                  // output buffer size 
            buf_size_,                  // input buffer size 
            0,                        // client time-out 
            nullptr);                    // default security attribute 
        
        if (handle_pipe_ == INVALID_HANDLE_VALUE)
        {
            return GetLastError();
        }
        
    }

    unsigned long int ListenToClient()
    {
        
    }
}