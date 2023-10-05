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
        explicit ProcessLogger(const std::string& process_name);

        std::string GetProcessName() const;
        void SetProcessName(const std::string& name);

        using Logger::SetMessage;

        std::string GetAlert(ProcessLoggerType type, UsageData usage_data);

        ~ProcessLogger() = default;
    };
}

#endif