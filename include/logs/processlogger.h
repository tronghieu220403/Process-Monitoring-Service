#ifndef PROCESSMONITORING_LOGGER_PROCESSLOGGER_H_
#define PROCESSMONITORING_LOGGER_PROCESSLOGGER_H_

#define UNICODE
#define _UNICODE

#include <logs/logger.h>
#include <process/processcontrol.h>

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
        ProcessControl process_;
    public:

        using Logger::SetMessage;

        ProcessLogger() = default;
        
        explicit ProcessLogger(ProcessControl p);
        
        void SetProcessInfo(ProcessControl p);

        void SetMessage(ProcessLoggerType type);

    };
}

#endif