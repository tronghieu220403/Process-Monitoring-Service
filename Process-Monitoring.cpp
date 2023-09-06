#pragma once

#ifndef PROCESSMONITORING_PROCESSMONITORING
#define PROCESSMONITORING_PROCESSMONITORING

#ifdef _VISUAL_STUDIO_WORKSPACE
#include "include/ulti/everything.h"
#else
#include "include/process/processinfo.h"
#endif

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")


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