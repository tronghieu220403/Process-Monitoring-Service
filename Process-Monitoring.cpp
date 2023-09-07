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
    mc.cpu_usage = 1.3;
    mc.mem_usage = 100.0;
    mc.disk_usage = 1000.0;
    mc.network_usage = 1.0;

    ps.SetMaxUsage(mc);

    while (true)
    {
        ps.UpdateProcessStats();
        ps.CheckProcessStats();
    }
}

#endif