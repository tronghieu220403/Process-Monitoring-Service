#include "client.h"

namespace pm
{
    PipelineClient::PipelineClient(const std::string& pipe_name)
    {
        Pipeline::SetPipelineName(pipe_name);
    };

#ifdef _WIN32

    void PipelineClient::SetBufferSize(int buf_size)
    {
        buf_size_ = buf_size;
    }

    int PipelineClient::GetBufferSize()
    {
        return buf_size_;
    }

#endif

    bool PipelineClient::ConnectToPipeServer()
    {
        #ifdef _WIN32
            std::string server_pipe = R"(\\.\pipe\)" + Pipeline::GetPipelineName();

            HANDLE handle_pipe = CreateFile(
                (std::wstring(server_pipe.begin(), server_pipe.end())).data(),             // pipe name 
                GENERIC_READ |  // read and write access 
                GENERIC_WRITE,
                0,              // no sharing 
                nullptr,           // default security attributes
                OPEN_EXISTING,  // opens existing pipe 
                0,              // default attributes 
                nullptr);          // no template file 

            if (handle_pipe != INVALID_HANDLE_VALUE)
            {
                Pipeline::SetPipelineHandle(handle_pipe);
                Pipeline::SetConnectStatus(true);
            }

            #ifdef _DEBUG
            /*
                int err = GetLastError();
                if (err != ERROR_PIPE_BUSY)
                {
                    Pipeline::SetConnectStatus(false);
                }
            */
            #endif

        #elif __linux__

            int fd_recv = -1;
            int fd_send = -1;

            std::string client_recv = "/tmp/" + server_name_ + "serversend";
            std::string client_send = "/tmp/" + server_name_ + "serverrecv";

            fd_recv = open(client_recv.data(), O_RDONLY);
            fd_send = open(client_send.data(), O_WRONLY);
            
            Pipeline::SetConnect(int fd_send, int fd_recv);

        #endif

        return Pipeline::IsActive();
        
    }
    
    PipelineClient::~PipelineClient()
    {
        PipelineClient::Close();
    }
}