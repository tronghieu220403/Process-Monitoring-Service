#include <include/communication/pipeline/server.h>
#include "server.h"

namespace pm
{
    PipelineServer::PipelineServer(const std::string& pipe_name)
    {
        server_name_ = pipe_name;
    };


    void PipelineServer::SetPipeName(const std::string& pipe_name)
    {
        server_name_ = pipe_name;
    }

    std::string PipelineServer::GetPipeName()
    {
        return server_name_;
    }

#ifdef _WIN32

    PipelineServer::PipelineServer(const std::string& pipe_name, int buf_size)
        : buf_size_(buf_size_)
    {
        server_name_ = pipe_name;
    };

    void PipelineServer::SetBufferSize(int buf_size)
    {
        buf_size_ = buf_size;
    }

    bool PipelineServer::SetMaxConnection(int max_connection)
    {
        #ifdef _WIN32
            if (max_connection_ || max_connection > PIPE_UNLIMITED_INSTANCES)
            {
                return false;
            }
        #elif __linux__

        #endif
        max_connection_ = max_connection;
        return true;
    }

    int PipelineServer::GetBufferSize()
    {
        last_receive_.resize(buf_size_);
        return buf_size_;
    }
#endif

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

            if (fd_recv_ == 0 || fd_send_ == 0)
            {
                PipelineServer::Close();
                return false;
            }
            unsigned long bytes_read = 0;
            int success = 0;
            int n_bytes = 0;
            
            success = read(fd_recv_, &n_bytes, 0);
            if (success == -1)
            {   
                PipelineServer::Close();
                return false;
            }

            success = write(fd_send_, &n_bytes, 0);
            if (success == -1)
            {   
                PipelineServer::Close();
                return false;
            }

        #endif

        return true;

    }

    unsigned long int PipelineServer::CreateServer()
    {
        #ifdef _WIN32

            std::string server_pipe = "\\\\.\\pipe\\" + server_name_;
            handle_pipe_ = CreateNamedPipe(
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
            
            if (handle_pipe_ == INVALID_HANDLE_VALUE)
            {
                return false;
            }

        #elif __linux__

            std::string server_send = "/tmp/" + server_name_ + "serversend";

            mkfifo(server_send.data(), 0666);                      /* read/write for user/group/others */
            fd_send_ = open(server_send.data(), O_CREAT | O_WRONLY); /* open as write-only */

            std::string server_recv = "/tmp/" + server_name_ + "serverrecv";

            mkfifo(server_recv.data(), 0666);                      /* read/write for user/group/others */
            fd_recv_ = open(server_recv.data(), O_CREAT | O_RDONLY); /* open as read-only */

            if (fd_send_ == -1 || fd_recv_ == -1)
            {
                Close();
                return false;
            }

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
        int success = 0;
        std::vector<char> cur_receive_;
        cur_receive_.clear();
        int type;

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

        #elif __linux__

            if (fd_send_ == 0)
            {
                return false;
            }

            int n_bytes = 0;
            int cur_ptr = 0;

            while(cur_ptr < 4)
            {
                success = read(fd_recv_, &n_bytes + cur_ptr, sizeof(int) - cur_ptr);
                if (success == -1)
                {
                    PipelineServer::Close();
                    return false;
                }
                else if (success == 0)
                {
                    return false;
                }
                cur_ptr += success;
            }

            cur_ptr = 0;
            
            while(cur_ptr < 4)
            {
                success = read(fd_recv_, &type + cur_ptr, sizeof(int) - cur_ptr);
                if (success == -1)
                {
                    PipelineServer::Close();
                    return false;
                }
                else if (success == 0)
                {
                    return false;
                }
                cur_ptr += success;
            }

            cur_receive_.resize(n_bytes);
            cur_ptr = 0;

            while(cur_ptr < 4)
            {
                success = read(fd_recv_, &cur_receive_[cur_ptr], n_bytes - cur_ptr);
                if (success == -1)
                {
                    PipelineServer::Close();
                    return false;
                }
                else if (success == 0)
                {
                    return false;
                }
                cur_ptr += success;
            }

        #endif

        last_receive_ = cur_receive_;
        last_message_type_ = type;

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
        #elif __linux__

            if (fd_send_ == 0)
            {
                return false;
            }

        #endif

            unsigned long bytes_written = 0;
            int success = 0;
            std::vector<int> send;

            #ifdef __WIN32
                if (data.size() > buf_size_){
                    return false;
                }
            #endif
            int sz = data.size();
            send.resize(4 + 4 + sz);
            memcpy(&send[0], &sz, 4);
            memcpy(&send[4], &type, 4);
            memcpy(&send[4], &data[0], sz);
        
        #ifdef _WIN32

            success = WriteFile(handle_pipe_, &send[0], send.size(), &bytes_written, nullptr);

            if (!success || bytes_written != send.size())
            {
                return false;
            }
    
        #elif __linux__

            bytes_written = write(fd_send_, &send[0], send.size());
            
            if (bytes_written != send.size())
            {
                return false;
            }

        #endif

        return true;
    }

    void PipelineServer::Close()
    {
        #ifdef _WIN32
            if (handle_pipe_ != nullptr)
            {
                CloseHandle(handle_pipe_);
                handle_pipe_ = nullptr;
            }
        #elif __linux__
            if (fd_send_ != 0 && fd_send_ != -1)
            {
                close(fd_send_);
                fd_send_ = 0;
            }
            if (fd_recv_ != 0 && fd_recv_ != -1)
            {
                close(fd_recv_);
                fd_recv_ = 0;
            }
            fd_send_ = 0;
            fd_recv_ = 0;
        #endif
    }

    PipelineServer::~PipelineServer()
    {
        Close();
    }
}
