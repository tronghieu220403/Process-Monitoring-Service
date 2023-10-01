
#ifndef PROCESSMONITORING_CONFIGURATION_STARTUPROGRAM_H_
#define PROCESSMONITORING_CONFIGURATION_STARTUPROGRAM_H_

#include "ulti/everything.h"
#include "file/file.h"

namespace pm
{
    class StartUpProgram
    {
    private:
        std::string exe_full_path_;
    public:

    StartUpProgram() = default;
    explicit StartUpProgram(const std::string& exe_full_path);

    void SetProgram(const std::string_view& exe_full_path);
    void SetThisProgram();

    bool Register();
    bool IsRegistered();
    
    };
}

#endif