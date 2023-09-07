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
    pm::MonitoringComponent mc;
    mc.cpu_usage = 0;
    mc.mem_usage = 0;
    mc.disk_usage = 0;
    mc.network_usage = 1.0;

    ps.SetMaxUsage(mc);

    for (int i = 0; i < 2; i++)
    {
        Sleep(5000);
        ps.UpdateProcessStats();
        ps.CheckProcessStats();
    }
}

#endif