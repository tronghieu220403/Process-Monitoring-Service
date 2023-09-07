#ifdef _VISUAL_STUDIO_WORKSPACE
#pragma once
#endif


#ifndef PROCESSMONITORING_PROCESSMONITORING
#define PROCESSMONITORING_PROCESSMONITORING

#ifdef _VISUAL_STUDIO_WORKSPACE
#include "include/ulti/everything.h"
#else
#include "include/process/processinfo.h"
#endif

#include <iostream> 
 
int main()
{
    std::cout << "OKE" << std::endl;
}

#endif