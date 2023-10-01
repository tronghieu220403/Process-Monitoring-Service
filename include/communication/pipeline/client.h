#ifndef PROCESSMONITORING_COMMUNICATION_PIPELINE_CLIENT_H_
#define PROCESSMONITORING_COMMUNICATION_PIPELINE_CLIENT_H_

#include "ulti/everything.h"
#include "communication/pipeline/pipe.h"

namespace pm
{
    class PipelineClient: public Pipeline
    {
    private:

        #ifdef _WIN32
            int buf_size_ = 0;
        #endif

    public:
        PipelineClient() = default;
        PipelineClient(const std::string& pipe_name);
        
        #ifdef _WIN32
            void SetBufferSize(int buf_size);
            int GetBufferSize();
        #endif
        
        bool ConnectToPipeServer();

        ~PipelineClient();
    };

}

#endif