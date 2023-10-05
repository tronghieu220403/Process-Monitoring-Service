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
        std::string process_name_;
    public:

        ProcessLogger() = default;

        using Logger::SetMessage;

        explicit ProcessLogger(std::string& process_name);

        std::string GetAlert(ProcessLoggerType type, UsageData usage_data);

        ~ProcessLogger() = default;
    };
}

#endif