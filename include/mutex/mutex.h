#ifdef _VISUAL_STUDIO_WORKSPACE
#pragma once
#endif

#ifndef PROCESSMONITORING_COMMUNICATION_MUTEX_MUTEX_H_
#define PROCESSMONITORING_COMMUNICATION_MUTEX_MUTEX_H_

#define _CRT_SECURE_NO_DEPRECATE

#ifndef _VISUAL_STUDIO_WORKSPACE
#define UNICODE
#define _UNICODE
#endif


#include <string.h>
#include <stdio.h>

#ifdef _WIN32
#include <Windows.h> 
#elif __linux__

#endif

#include <string>

namespace pm
{
    class NamedMutex
    {
    private:
        std::string mutex_name_;

        #ifdef _WIN32
            HANDLE handle_mutex_;
        #elif __linux__

        #endif
    public:
        NamedMutex() = default;
        NamedMutex(const std::string& mutex_name);
        
        void SetMutex(const std::string& mutex_name);

        std::string GetMutexName() const;

        void Lock();
        void Unlock();

        void Close();

        ~NamedMutex();
    };

}

#endif