#pragma once

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
    #include <Windows.h>
    #include <direct.h>
#elif __linux__
	#include <sys/wait.h>
	#include <dirent.h>
    #include <unistd.h>
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

#ifdef _WIN32
    
#elif __linux__
    #define Sleep(ms) usleep(ms*1000)
#endif

#ifdef _WIN32
    #define GetCurrentDir _getcwd
#else
    #define GetCurrentDir getcwd
 #endif


#endif