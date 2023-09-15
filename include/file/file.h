#ifdef _VISUAL_STUDIO_WORKSPACE
#pragma once
#endif

#ifndef PROCESSMONITORING_FILE_FILE_H_
#define PROCESSMONITORING_FILE_FILE_H_

#ifndef _VISUAL_STUDIO_WORKSPACE
#define UNICODE
#define _UNICODE
#endif

#ifdef _VISUAL_STUDIO_WORKSPACE
#include "E:/Code/Github/Process-Monitoring/include/ulti/everything.h"
#else
#include "include/ulti/everything.h"
#endif

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
        explicit File(std::string file_path);

        void Set(std::string file_path);
        
        std::vector<char> ReadAll();

        void Append(const std::vector<char>& chr);
        void AppendFromFile(const std::string& src_file_path);

        bool SelfDelete();


    };

}

#endif