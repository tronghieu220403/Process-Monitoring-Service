#pragma once

#ifndef PROCESSMONITORING_ULTI_EVERYTHING_H_
#define PROCESSMONITORING_ULTI_EVERYTHING_H_

#include <string.h>
#include <time.h>

#ifdef _WIN32
    #include <Windows.h>
    #include <winrt/windows.data.json.h>
    #include <winrt/windows.foundation.collections.h>
    #define json_char TCHAR
    #include <WinSock2.h>
    #include <iphlpapi.h>
	#include <WS2tcpip.h>
	#include <TlHelp32.h>
    #include <Psapi.h>

#elif __linux__
	#include <sys/wait.h>
	#include <dirent.h>

#endif

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>

#ifdef _WIN32
    #define ZeroMem(data,size) ZeroMemory(data,size)
#elif __linux__
    #define ZeroMem(data,sz) memset(data,size)
#endif

#endif