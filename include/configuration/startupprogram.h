
#ifndef PROCESSMONITORING_CONFIGURATION_STARTUPROGRAM_H_
#define PROCESSMONITORING_CONFIGURATION_STARTUPROGRAM_H_

#ifdef _VISUAL_STUDIO_WORKSPACE
#include "E:/Code/Github/Process-Monitoring/include/ulti/everything.h"
#else
#include "include/ulti/everything.h"
#endif


#ifdef _VISUAL_STUDIO_WORKSPACE
#include "E:/Code/Github/Process-Monitoring/include/file/file.h"

#else

#include "include/file/file.h"

#endif

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