#ifdef _VISUAL_STUDIO_WORKSPACE
#pragma once
#endif

#ifndef PROCESSMONITORING_LOGGER_PROCESSLOGGER_H_
#define PROCESSMONITORING_LOGGER_PROCESSLOGGER_H_

#define _CRT_SECURE_NO_DEPRECATE

#ifndef _VISUAL_STUDIO_WORKSPACE
#define UNICODE
#define _UNICODE
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

        void SetMessage(ProcessLoggerType type);

        bool operator==(const ProcessLogger& other) = delete;
        ~ProcessLogger() final = default;
    };
}

#endif