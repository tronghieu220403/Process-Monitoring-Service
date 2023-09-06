#ifndef PROCESSMONITORING_LOGGER_PROCESSLOGGER_H_
#define PROCESSMONITORING_LOGGER_PROCESSLOGGER_H_


#ifdef _VISUAL_STUDIO_WORKSPACE

    #include "E:/Code/Github/Process-Monitoring/include/logs/logger.h"
    #include "E:/Code/Github/Process-Monitoring/include/process/processcontroller.h"
    #include "E:/Code/Github/Process-Monitoring/include/ulti/collections.h"

#else

    #include "include/logs/logger.h"
    #include "include/process/processcontroller.h"
    #include "include/ulti/collections.h"

#endif


namespace pm
{

    class ProcessLogger: public Logger
    {
    private:
        ProcessController process_controller_;
    public:

        using Logger::SetMessage;

        ProcessLogger();
        ProcessLogger(ProcessController& pc);

        ProcessLogger& operator=(const ProcessLogger& pl);

        void SetProcessController(ProcessController& pc);

        void SetMessage(ProcessLoggerType type);

    };
}

#endif