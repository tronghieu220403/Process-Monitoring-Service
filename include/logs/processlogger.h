#ifndef PROCESSMONITORING_LOGGER_PROCESSLOGGER_H_
#define PROCESSMONITORING_LOGGER_PROCESSLOGGER_H_

#define UNICODE
#define _UNICODE

#include <logs/logger.h>
#include <process/processcontroller.h>

namespace pm
{
    enum ProcessLoggerType
    {
        kProcessLoggerCpu,
        kProcessLoggerMem,
        kProcessLoggerDisk,
        kProcessLoggerNet
    };

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