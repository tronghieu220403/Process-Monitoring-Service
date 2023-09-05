#ifndef PROCESSMONITORING_LOGGER_PROCESSLOGGER_H_
#define PROCESSMONITORING_LOGGER_PROCESSLOGGER_H_

#define UNICODE
#define _UNICODE

#include "include/logs/logger.h"
#include "include/process/processcontroller.h"
#include "include/ulti./collections.h"

namespace pm
{

    class ProcessLogger: public Logger
    {
    private:
        ProcessController process_controller_;
    public:

        using Logger::SetMessage;

        ProcessLogger() = default;
        
        explicit ProcessLogger(ProcessController& p);
        
        void SetProcessInfo(ProcessController& p);

        void SetMessage(ProcessLoggerType type);

    };
}

#endif