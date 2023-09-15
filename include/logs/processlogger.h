#ifndef PROCESSMONITORING_LOGGER_PROCESSLOGGER_H_
#define PROCESSMONITORING_LOGGER_PROCESSLOGGER_H_

#ifdef _VISUAL_STUDIO_WORKSPACE
#include "E:/Code/Github/Process-Monitoring/include/ulti/everything.h"
#else
#include "include/ulti/everything.h"
#endif

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
        std::shared_ptr<ProcessController> process_controller_;
    public:

        using Logger::SetMessage;

        ProcessLogger();
        explicit ProcessLogger(const std::shared_ptr<ProcessController>& pc);

        ProcessLogger& operator=(const ProcessLogger& pl);

        void SetProcessController(const std::shared_ptr<ProcessController>& pc);

        std::string GetAlert(ProcessLoggerType type);

        bool operator==(const ProcessLogger& other) = delete;
        ~ProcessLogger() final = default;
    };
}

#endif