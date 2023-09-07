#ifdef _VISUAL_STUDIO_WORKSPACE
#pragma once
#endif

#ifndef PROCESSMONITORING_FILE_FILE_H_
#define PROCESSMONITORING_FILE_FILE_H_

#ifndef _VISUAL_STUDIO_WORKSPACE
#define UNICODE
#define _UNICODE
#endif


#include <string.h>

#ifdef _WIN32
#include <Windows.h>

#elif __linux__

#endif

#include <string>

namespace pm
{
    class File
    {
        private:

        std::wstring content_;
        long long file_size_;

        #ifdef _WIN32
            HANDLE file_handle_;
        #elif __linux__

        #endif

        public:

        File() = default;

        bool Set(std::wstring name);
        
        std::wstring ReadAll();

        bool Append();

        bool Delete();


    };

}

#endif