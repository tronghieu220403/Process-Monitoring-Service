#ifndef PROCESSMONITORING_COMMUNICATION_PIPELINE_SERVER_H_
#define PROCESSMONITORING_COMMUNICATION_PIPELINE_SERVER_H_

#include "ulti/everything.h"
#include "communication/pipeline/pipe.h"

namespace pm
{
    class PipelineServer: public Pipeline
    {
    private:

        #ifdef _WIN32
            int buf_size_ = 0;
            int max_connection_ = 0;
        #elif __linux__
        #endif
    public:
        PipelineServer() = default;
        PipelineServer(const std::string& pipe_name);
        
        #ifdef _WIN32
            PipelineServer(const std::string& pipe_name, int buf_size);
            PipelineServer(const std::string& pipe_name, int buf_size, int max_connection);
            bool SetMaxConnection(int max_connection);
            void SetBufferSize(int buf_size);
            int GetMaximumConnection();
            int GetBufferSize();
        #endif

        bool CreateServer();
        bool ListenToClient();

        ~PipelineServer();
    };
        
}

#endif