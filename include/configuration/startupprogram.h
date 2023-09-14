#ifdef _VISUAL_STUDIO_WORKSPACE
#pragma once
#endif

#ifndef PROCESSMONITORING_CONFIGURATION_STARTUPROGRAM_H_
#define PROCESSMONITORING_CONFIGURATION_STARTUPROGRAM_H_

#include <string.h>

#ifndef _VISUAL_STUDIO_WORKSPACE
#define UNICODE
#define _UNICODE
#endif

#ifdef _WIN32
#include <WS2tcpip.h>
#include <strsafe.h>
#include <Windows.h>
#elif __linux__
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

#ifdef _VISUAL_STUDIO_WORKSPACE
#include "E:/Code/Github/Process-Monitoring/include/file/file.h"
#else
#include "include/file/file.h"
#endif

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

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