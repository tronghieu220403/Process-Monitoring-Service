#include "server.h"

namespace pm
{

#ifdef _WIN32

    Pipeline::Pipeline(HANDLE handle_pipe)
        : handle_pipe_(handle_pipe), connected_(true)
    {
        
    };

#elif __linux__

    Pipeline::Pipeline(int fd_send, int fd_recv)
        : fd_send_(fd_send), fd_recv_(fd_recv), connected_(true)
    {
        
    };

#endif


    void Pipeline::SetPipelineName(const std::string& pipe_name)
    {
        name_ = pipe_name;
    }

    void Pipeline::SetConnectStatus(bool status)
    {
        connected_ = status;
    }

    #ifdef _WIN32
        void Pipeline::SetPipelineHandle(HANDLE handle_pipe)
        {
            handle_pipe_ = handle_pipe;
        }
    #elif __linux__

        void Pipeline::SetConnect(int fd_send, int fd_recv)
        {
            fd_send_ = fd_send;
            fd_recv_ = fd_recv;
            SetConnectStatus(true);
        }

        void Pipeline::SetFdSend(int fd_send)
        {
            fd_send_ = fd_send;
        }

        void Pipeline::SetFdRecv(int fd_recv)
        {
            fd_recv_ = fd_recv;
        }

    #endif

    std::string Pipeline::GetPipelineName()
    {
        return name_;
    }

    std::vector<char> Pipeline::GetLastMessage()
    {
        return last_receive_;
    }

    int Pipeline::GetLastMessageType()
    {
        return last_message_type_;
    }

    #ifdef _WIN32
        HANDLE Pipeline::GetHandle()
        {
            return handle_pipe_;
        }
    #elif __linux__
        int Pipeline::GetFdSend()
        {
            return fd_send_;
        }
        int Pipeline::GetFdRecv()
        {
            return fd_recv_;
        }
    #endif


    bool Pipeline::IsActive()
    {
        #ifdef _WIN32
            if (handle_pipe_ == nullptr || connected_ == false)
        #elif __linux__
            if (fd_recv_ == 0 || fd_send_ == 0 || fd_recv_ == -1 || fd_send_ == -1 || connected_ == false)
        #endif
            {
                //Close();
                return false;
            }
        return true;
    }

    bool Pipeline::TryGetMessage()
    {
        int success = 0;
        std::vector<char> cur_receive_;
        cur_receive_.clear();
        int type = 0;

        #ifdef _WIN32
            if (handle_pipe_ == nullptr || connected_ == false)
            {
                return false;
            }
            DWORD bytes_read = 0;

            int n_bytes = 0;
            int cur_ptr = 0;
            while(cur_ptr < 4)
            {
                std::osyncstream(std::cout) << "Start reading" << std::endl;
                success = ReadFile(handle_pipe_, &n_bytes + cur_ptr, 4 - cur_ptr, &bytes_read, nullptr);
                if (!success && bytes_read == 0)
                {   
                    DWORD error = GetLastError();
                     std::osyncstream(std::cout) << error << std::endl;
                    if (error == ERROR_BROKEN_PIPE)
                    {
                        //Close();
                        return false;
                    }
                }
                cur_ptr += bytes_read;
            }
            
             std::osyncstream(std::cout) << "Must receive: " << " " << n_bytes << std::endl;

            cur_ptr = 0;
            bytes_read = 0;
            while(cur_ptr < 4)
            {
                std::osyncstream(std::cout) << "Start receiving" << std::endl;

                success = ReadFile(handle_pipe_, &type + cur_ptr, 4 - cur_ptr, &bytes_read, NULL);
                if (!success && bytes_read == 0)
                {
                    DWORD error = GetLastError();
                     std::osyncstream(std::cout) << error << std::endl;
                    if (error == ERROR_BROKEN_PIPE)
                    {
                        //Close();
                        return false;
                    }
                }
                cur_ptr += bytes_read;
            }

             std::osyncstream(std::cout) << "Protocol: " << " " << type << std::endl;

            cur_receive_.resize(n_bytes);
            cur_ptr = 0;

            while (cur_ptr < n_bytes)
            {
                std::osyncstream(std::cout) << "Start receiving" << std::endl;

                success = ReadFile(handle_pipe_, &cur_receive_[cur_ptr], n_bytes - cur_ptr, &bytes_read, nullptr);

                if (!success && bytes_read == 0)
                {
                    DWORD error = GetLastError();
                     std::osyncstream(std::cout) << error << std::endl;
                    if (GetLastError() == ERROR_BROKEN_PIPE)
                    {
                        //Close();
                        return false;
                    }
                }
                cur_ptr += bytes_read;
            }

             std::osyncstream(std::cout) << "Start receiving: " << std::endl;

        #elif __linux__

            if (fd_recv_ == -1 || fd_recv_ == -1 || connected_ == false)
            {
                Close();
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
                cur_ptr += success;
            }

            cur_receive_.resize(n_bytes);
            cur_ptr = 0;

            while(cur_ptr < n_bytes)
            {
                success = read(fd_recv_, &cur_receive_[cur_ptr], n_bytes - cur_ptr);
                if (success == -1)
                {
                    Close();
                    return false;
                }
                cur_ptr += success;
            }

        #endif

        last_receive_ = cur_receive_;
        last_message_type_ = type;

        return true;

    }


    bool Pipeline::TrySendMessage(int type, std::vector<char> data)
    {
        // Write the reply to the pipe. 
        #ifdef _WIN32
            if (handle_pipe_ == nullptr || connected_ == false)
            {
                return false;
            }
        #elif __linux__

            if (fd_send_ == -1 || fd_send_ == -1 || connected_ == false)
            {
                Close();
                return false;
            }

        #endif

            unsigned long bytes_written = 0;
            int success = 0;
            std::vector<char> send;

            auto sz = static_cast<int>(data.size());
            send.resize(4 + 4 + sz);
            memcpy(&send[0], &sz, 4);
            memcpy(&send[4], &type, 4);
            if (sz != 0)
            {
                memcpy(&send[8], &data[0], sz);
            }
        
        #ifdef _WIN32

            int cur_ptr = 0;
            bytes_written = 0;
            while (cur_ptr < send.size())
            {
                success = WriteFile(handle_pipe_, &send[0 + cur_ptr], send.size() - cur_ptr, &bytes_written, nullptr);
                if (!success)
                {
                    //Close();
                    return false;
                }
                cur_ptr += bytes_written;
            }

    
        #elif __linux__

            int cur_ptr = 0;
            while (cur_ptr < send.size())
            {
                bytes_written = write(fd_send_, &send[0 + cur_ptr], send.size() - cur_ptr);
                
                if (bytes_written == -1)
                {
                    Close();
                    return false;
                }

                cur_ptr += bytes_written;
            }

        #endif

        return true;
    }

    void Pipeline::Close()
    {
        #ifdef _WIN32
            if (handle_pipe_ != nullptr && handle_pipe_ != 0)
            {
                CloseHandle(handle_pipe_);
                connected_ = false;
                handle_pipe_ = nullptr;
            }
        #elif __linux__
            if (fd_send_ != 0 && fd_send_ != -1)
            {
                close(fd_send_);
                fd_send_ = -1;
            }
            if (fd_recv_ != 0 && fd_recv_ != -1)
            {
                close(fd_recv_);
                fd_recv_ = -1;
            }
            connected_ = false;
            fd_send_ = -1;
            fd_recv_ = -1;
        #endif
    }

    Pipeline::~Pipeline()
    {
        Close();
    }
}
