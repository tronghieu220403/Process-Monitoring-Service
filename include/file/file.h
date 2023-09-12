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
#include <vector>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <filesystem>

namespace pm
{
    class File
    {
        private:

        std::string file_path_;
        long long file_size_;

        #ifdef _WIN32
            HANDLE file_handle_;
        #elif __linux__

        #endif

        public:

        File() = default;
        File(std::string file_path);

        bool Set(std::string file_path);
        
        std::vector<char> ReadAll();

        bool Append(const std::vector<char>& chr);
        bool AppendFromFile(const std::string& src_file_path);

        bool SelfDelete();


    };

}

#endif