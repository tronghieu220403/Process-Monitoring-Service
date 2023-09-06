#pragma once

#ifndef PROCESSMONITORING_CONFIGURATION_STARTUPROGRAM_H_
#define PROCESSMONITORING_CONFIGURATION_STARTUPROGRAM_H_

#include <string.h>

#ifdef _WIN32
#include <Windows.h>
#include <WS2tcpip.h>
#include <strsafe.h>
#elif __linux__

#endif

#include <string>

namespace pm
{
    class StartUpProgram
    {
    private:
        std::string exe_full_path_;
    public:

    StartUpProgram() = default;
    explicit StartUpProgram(const std::string exe_full_path);

    void SetProgram(std::string exe_full_path);
    void SetThisProgram();

    bool Register();
    bool IsRegistered();
    
    };
}

#endif