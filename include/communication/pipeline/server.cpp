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

    void PipelineServer::SetPipeName(const std::string& pipe_name)
    {
        server_name_ = pipe_name;
    }

    void PipelineServer::SetBufferSize(int buf_size)
    {
        buf_size_ = buf_size;
    }

    bool PipelineServer::SetMaxConnection(int max_connection)
    {
        if (max_connection_ || max_connection > PIPE_UNLIMITED_INSTANCES)
        {
            return false;
        }
        max_connection_ = max_connection;
        return true;
    }

    std::string PipelineServer::GetPipeName()
    {
        return server_name_;
    }

    int PipelineServer::GetBufferSize()
    {
        last_receive_.resize(buf_size_);
        return buf_size_;
    }

    std::vector<char> PipelineServer::GetLastMessage()
    {
        return last_receive_;
    }

    int PipelineServer::GetLastMessageType()
    {
        return last_message_type_;
    }

    bool PipelineServer::IsActive()
    {
        #ifdef _WIN32
            if (handle_pipe_ == nullptr)
            {
                return false;
            }
            DWORD bytes_read = 0;
            BOOL success = FALSE;
            int n_bytes = 0;
            
            success = ReadFile(handle_pipe_, &n_bytes, 0, &bytes_read, NULL);
            if (!success || bytes_read == 0)
            {   
                if (GetLastError() == ERROR_BROKEN_PIPE)
                {
                    PipelineServer::Close();
                    return false;
                }
            }
        #elif __linux__

        #endif

        return true;

    }

    unsigned long int PipelineServer::CreateServer()
    {
        #ifdef _WIN32

            handle_pipe_ = CreateNamedPipe(
                (std::wstring(server_name_.begin(), server_name_.end())).data(),             // pipe name 
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
            
            if (handle_pipe_ == INVALID_HANDLE_VALUE)
            {
                return false;
            }

        #elif __linux__

        #endif

        return true;
        
    }

    unsigned long int PipelineServer::ListenToClient()
    {        
        #ifdef _WIN32

            return ConnectNamedPipe(handle_pipe_, nullptr) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

        #elif __linux__
            return 0;
        #endif

    }

    bool PipelineServer::TryGetMessage()
    {
        #ifdef _WIN32
            if (handle_pipe_ == nullptr)
            {
                return false;
            }
            DWORD bytes_read = 0;
            BOOL success = FALSE;
            std::vector<char> cur_receive_;
            cur_receive_.clear();
            int type;

            int n_bytes = 0;
            int cur_ptr = 0;
            while(cur_ptr < 4)
            {
                success = ReadFile(handle_pipe_, &n_bytes + cur_ptr, 4 - cur_ptr, &bytes_read, NULL);
                if (!success || bytes_read == 0)
                {   
                    if (GetLastError() == ERROR_BROKEN_PIPE)
                    {
                        PipelineServer::Close();
                        return false;
                    }
                    else
                    {
                        return false;
                    }
                }
                cur_ptr += bytes_read;
            }
            
            cur_ptr = 0;
            bytes_read = 0;
            while(cur_ptr < 4)
            {
                success = ReadFile(handle_pipe_, &type + cur_ptr, 4 - cur_ptr, &bytes_read, NULL);
                if (!success || bytes_read == 0)
                {   
                    if (GetLastError() == ERROR_BROKEN_PIPE)
                    {
                        PipelineServer::Close();
                        return false;
                    }
                    else
                    {
                        return false;
                    }
                }
                cur_ptr += bytes_read;
            }

            cur_receive_.resize(n_bytes);
            cur_ptr = 0;

            while (cur_ptr < n_bytes)
            {
                success = ReadFile(handle_pipe_, &cur_receive_[cur_ptr], n_bytes - cur_ptr, &bytes_read, NULL);
                if (!success || bytes_read == 0)
                {   
                    if (GetLastError() == ERROR_BROKEN_PIPE)
                    {
                        PipelineServer::Close();
                        return false;
                    }
                    else
                    {
                        return false;
                    }
                }
                cur_ptr += bytes_read;
            }

            last_receive_ = cur_receive_;
            last_message_type_ = type;
        #elif __linux__

        #endif

        return true;

    }


    bool PipelineServer::TrySendMessage(int type, std::vector<char> data)
    {
        // Write the reply to the pipe. 
        #ifdef _WIN32
            if (handle_pipe_ == nullptr)
            {
                return false;
            }

            DWORD bytes_written = 0;
            BOOL success = FALSE;
            std::vector<int> send;

            if (data.size() > buf_size_){
                return false;
            }

            int sz = data.size();
            send.resize(4 + 4 + sz);
            memcpy(&send[0], &sz, 4);
            memcpy(&send[4], &type, 4);
            memcpy(&send[4], &data[0], sz);

            success = WriteFile(handle_pipe_, &send[0], send.size(), &bytes_written, nullptr);

            if (!success || bytes_written != send.size())
            {
                return false;
            }
        #elif __linux__

        #endif

        return false;
    }

    void PipelineServer::Close()
    {
        #ifdef _WIN32
            if (handle_pipe_ != nullptr)
            {
                CloseHandle(handle_pipe_);
                handle_pipe_ = nullptr;
            }
        #endif
    }

}