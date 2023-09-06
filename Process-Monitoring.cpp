#pragma once

#ifndef PROCESSMONITORING_PROCESSMONITORING
#define PROCESSMONITORING_PROCESSMONITORING

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

#include <iostream>
#include <vector>
#include <sstream>

/*
#ifdef _WIN32
#include <Windows.h>
#include <iphlpapi.h>
#endif
*/

#ifdef _VISUAL_STUDIO_WORKSPACE

//#include "include/process/process.h"

#else

//#include "include/process/processsupervision.h"

#endif
int main()
{
    std::cout << "OKE" << std::endl;
}

#endif