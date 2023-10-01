#ifndef PROCESSMONITORING_LOGGER_PROCESSLOGGER_H_
#define PROCESSMONITORING_LOGGER_PROCESSLOGGER_H_

#include "ulti/everything.h"
#include "logs/logger.h"
#include "process/processcontroller.h"
#include "ulti/collections.h"

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