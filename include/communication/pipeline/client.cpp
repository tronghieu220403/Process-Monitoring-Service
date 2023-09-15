#include <include/communication/pipeline/client.h>
#include "client.h"

namespace pm
{
    PipelineClient::PipelineClient(const std::string& server_pipe_name)
    {
        server_name_ = server_pipe_name;
    };

    void PipelineClient::SetServerPipeName(const std::string& server_pipe_name)
    {
        server_name_ = server_pipe_name;
    }

#ifdef _WIN32
    void PipelineClient::SetBufferSize(int buf_size)
    {
        buf_size_ = buf_size;
    }
#endif
    std::string PipelineClient::GetPipeName()
    {
        return server_name_;
    }

    std::vector<char> PipelineClient::GetLastMessage()
    {
        return last_receive_;
    }

    int PipelineClient::GetLastMessageType()
    {
        return last_message_type_;
    }

    bool PipelineClient::IsActive()
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
                    PipelineClient::Close();
                    return false;
                }
            }
        #elif __linux__

            if (fd_recv_ == 0 || fd_send_ == 0)
            {
                Close();
                return false;
            }
            unsigned long bytes_read = 0;
            int success = 0;
            int n_bytes = 0;
            
            success = read(fd_recv_, &n_bytes, 0);
            if (success == -1)
            {   
                Close();
                return false;
            }

            success = write(fd_send_, &n_bytes, 0);
            if (success == -1)
            {   
                Close();
                return false;
            }

        #endif

        return true;

    }

    bool PipelineClient::ConnectToPipeServer()
    {
        #ifdef _WIN32
            std::string server_pipe = "\\\\.\\pipe\\" + server_name_;

            handle_pipe_ = CreateFile(
                (std::wstring(server_pipe.begin(), server_pipe.end())).data(),             // pipe name 
                GENERIC_READ |  // read and write access 
                GENERIC_WRITE,
                0,              // no sharing 
                nullptr,           // default security attributes
                OPEN_EXISTING,  // opens existing pipe 
                0,              // default attributes 
                nullptr);          // no template file 

            if (handle_pipe_ == INVALID_HANDLE_VALUE || GetLastError() != ERROR_PIPE_BUSY || !WaitNamedPipe((std::wstring(server_name_.begin(), server_name_.end())).data(), 0))
            {
                return false;
            }

        #elif __linux__

            std::string client_recv = "/tmp/" + server_name_ + "serversend";

            int fd_recv_ = open(client_recv.data(), O_RDONLY);

            std::string client_send = "/tmp/" + server_name_ + "serverrecv";

            int fd_send_ = open(client_send.data(), O_WRONLY);
            if (fd_send_ < 0 || fd_recv_ < 0)
            {
                Close();
                return false;
            }
        
        #endif

        return true;
        
    }

    bool PipelineClient::TryGetMessage()
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
                        Close();
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
                        Close();
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
                        Close();
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
                    Close();
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
                    Close();
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
                    Close();
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


    bool PipelineClient::TrySendMessage(int type, std::vector<char> data)
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

            #ifdef _WIN32
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

    void PipelineClient::Close()
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

    PipelineClient::~PipelineClient()
    {
        Close();
    }
}