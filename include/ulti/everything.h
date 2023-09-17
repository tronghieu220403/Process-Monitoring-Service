#ifdef _VISUAL_STUDIO_WORKSPACE
#pragma once
#endif

#ifdef _VISUAL_STUDIO_WORKSPACE
#define _CRT_SECURE_NO_DEPRECATE
#endif

#ifndef PROCESSMONITORING_ULTI_EVERYTHING_H_
#define PROCESSMONITORING_ULTI_EVERYTHING_H_

#ifndef _VISUAL_STUDIO_WORKSPACE
#define UNICODE
#define _UNICODE
#endif

#include <string.h>
#include <time.h>

#ifdef _WIN32
    #include <WinSock2.h>
    #include <iphlpapi.h>
	#include <WS2tcpip.h>
	#include <TlHelp32.h>
    #include <Psapi.h>
    #include <direct.h>
    #include <strsafe.h>
    #include <tchar.h>
    #include <Windows.h>

    #include <winrt/windows.foundation.collections.h>
    #include <winrt/windows.data.json.h>
    #define json_char TCHAR

#elif __linux__

	#include <sys/wait.h>
	#include <dirent.h>
    #include <unistd.h>
    #include <fcntl.h> 
    #include <sys/stat.h> 
    #include <sys/types.h> 
    #include <semaphore.h> 
    #include <errno.h>
#endif

#include <memory>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <filesystem>
#include <thread>

#ifdef __linux__

#include "include/nlohmann/json.hpp"

#endif // __linux__

#ifdef _VISUAL_STUDIO_WORKSPACE
#define _CRT_SECURE_NO_DEPRECATE
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")
#endif


#ifdef _WIN32
    #define ZeroMem(data,size) ZeroMemory(data,size)
#elif __linux__
    #define ZeroMem(data,size) memset(data, 0, size)
#endif

#ifdef _WIN32
    
#elif __linux__
    #define Sleep(ms) usleep(ms*1000)
#endif

#ifdef _WIN32
    #define GetCurrentDir _getcwd
#else
    #define GetCurrentDir getcwd
 #endif

namespace pm
{
    inline std::string CharVectorToString(std::vector<char> v)
    {
        return std::string(v.begin(), v.end());
    }

    #ifdef __linux__
        inline std::string GetCurrentUserPath()
        {
            std::string path;
            path.resize(10000);
            int sz = readlink("/proc/self/exe", path.data(), path.size());
            path.resize(sz);
            int pos = path.find("/", path.find("/", path.find("/") + 1) + 1);
            return path.substr(0, pos);
        }

        inline bool CreateFolder(std::string path)
        {
            struct stat st;
            if(stat(path.data(),&st) != 0 || st.st_mode & S_IFDIR == 0)
            {
                if (mkdir(path.data(),0777) != 0)
                {
                    return false;
                }
            }
            return true;
        }
    #endif
}

#endif
