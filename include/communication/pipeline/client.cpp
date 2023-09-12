#include <include/communication/pipeline/client.h>
#include "client.h"

namespace pm
{
    PipelineClient::PipelineClient(const std::string& server_pipe_name)
        : server_name_(server_pipe_name)
    {

    };

    void PipelineClient::SetServerPipeName(const std::string& server_pipe_name)
    {
        server_name_ = server_pipe_name;
    }

    void PipelineClient::SetBufferSize(int buf_size)
    {
        buf_size_ = buf_size;
    }

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

        #endif

        return true;

    }

    bool PipelineClient::ConnectToPipeServer()
    {
        #ifdef _WIN32

            handle_pipe_ = CreateFile(
                (std::wstring(server_name_.begin(), server_name_.end())).data(),             // pipe name 
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

        #endif

        return true;
        
    }

    bool PipelineClient::TryGetMessage()
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
                        PipelineClient::Close();
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
                        PipelineClient::Close();
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
                        PipelineClient::Close();
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


    bool PipelineClient::TrySendMessage(int type, std::vector<char> data)
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

    void PipelineClient::Close()
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