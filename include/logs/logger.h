#ifdef _VISUAL_STUDIO_WORKSPACE
#pragma once
#endif

#ifndef PROCESSMONITORING_LOGGER_LOGGER_H_
#define PROCESSMONITORING_LOGGER_LOGGER_H_

#define _CRT_SECURE_NO_DEPRECATE

#ifndef _VISUAL_STUDIO_WORKSPACE
#define UNICODE
#define _UNICODE
#endif

#include "ulti/everything.h"
#include "process/processinfo.h"

namespace pm
{
    class Logger
    {
    private:
        std::string message_;
        std::string folder_path_;
    public:
        Logger() = default;
        
        Logger(const std::string& message, const std::string& folder_path);
                
        explicit Logger(const std::string& message);
        
        void SetMessage(const std::string_view& message);
        void AddMessage(const std::string_view& message);
        void SetFolderPath(const std::string_view& folder_path);

        std::string GetMessage() const;
        std::string GetFolderPath() const;

        void WriteLog() const;

        virtual ~Logger() = default;

    };
        
}

#endif