#ifdef _VISUAL_STUDIO_WORKSPACE
#pragma once
#endif


#ifndef PROCESSMONITORING_PROCESSMONITORING
#define PROCESSMONITORING_PROCESSMONITORING

#ifdef _VISUAL_STUDIO_WORKSPACE
#include "include/process/processsupervision.h"
#else
#include "include/process/processinfo.h"
#endif

#include <iostream> 
#include <vector>

int main()
{
    pm::ProcessSupervision ps("Messenger.exe");
    std::shared_ptr<pm::ProcessController>gg = ps.GetProcessController();
    std::cout << ps.GetProcessController()->GetPid() << std::endl;
}

#endif